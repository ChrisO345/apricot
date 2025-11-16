#ifndef APRICOT_BEZIER_C_
#define APRICOT_BEZIER_C_

#ifndef APRICOT_BEZIER_DEF
#define APRICOT_BEZIER_DEF extern
#endif

// === Arcs ===
APRICOT_BEZIER_DEF void apricot_fill_arc(const ApricotCanvas *canvas, int cx,
                                         int cy, int radius, float start_angle,
                                         float end_angle, ApricotColor color);

APRICOT_BEZIER_DEF void apricot_draw_arc(const ApricotCanvas *canvas, int cx,
                                         int cy, int radius, float start_angle,
                                         float end_angle, ApricotColor color);
APRICOT_BEZIER_DEF void apricot_draw_slice(const ApricotCanvas *canvas, int cx,
                                           int cy, int radius,
                                           float start_angle, float end_angle,
                                           ApricotColor color);

// === Beziers ====
APRICOT_BEZIER_DEF void
apricot_draw_quadratic_bezier(const ApricotCanvas *canvas, int x0, int y0,
                              int x1, int y1, int x2, int y2,
                              ApricotColor color, int segments);

APRICOT_BEZIER_DEF void
apricot_draw_cubic_bezier(const ApricotCanvas *canvas, int x0, int y0, int x1,
                          int y1, int x2, int y2, int x3, int y3,
                          ApricotColor color, int segments);

APRICOT_BEZIER_DEF void apricot_draw_catmull_rom(const ApricotCanvas *canvas,
                                                 int *x, int *y, int n,
                                                 ApricotColor color,
                                                 int segments);

// === Splines ===
APRICOT_BEZIER_DEF void
apricot_draw_quadratic_spline(const ApricotCanvas *canvas, int *points, int n,
                              ApricotColor color, int segments);

APRICOT_BEZIER_DEF void apricot_draw_cubic_spline(const ApricotCanvas *canvas,
                                                  int *points, int n,
                                                  ApricotColor color,
                                                  int segments);

#endif /* APRICOT_BEZIER_C_ */

#ifdef APRICOT_IMPLEMENTATION

#include <math.h>

// === Arcs ===
APRICOT_BEZIER_DEF void apricot_fill_arc(const ApricotCanvas *canvas, int cx,
                                         int cy, int radius, float start_angle,
                                         float end_angle, ApricotColor color) {
  if (radius <= 0)
    return;
  if (start_angle > end_angle) {
    float temp = start_angle;
    start_angle = end_angle;
    end_angle = temp;
  }
  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      if (x * x + y * y <= radius * radius) {
        float angle = atan2f((float)y, (float)x);
        if (angle < 0)
          angle += 2.0f * 3.14159265358979323846f;
        if (angle >= start_angle && angle <= end_angle) {
          int px = cx + x;
          int py = cy + y;
          if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height) {
            APRICOT_SET_PIXEL(canvas, px, py, color);
          }
        }
      }
    }
  }
}

APRICOT_BEZIER_DEF void apricot_draw_arc(const ApricotCanvas *canvas, int cx,
                                         int cy, int radius, float start_angle,
                                         float end_angle, ApricotColor color) {
  if (radius <= 0)
    return;
  int segments = (int)(radius * fabsf(end_angle - start_angle) / 4.0f);
  if (segments < 1)
    segments = 1;
  int prev_x = cx + (int)(radius * cosf(start_angle) + 0.5f);
  int prev_y = cy + (int)(radius * sinf(start_angle) + 0.5f);

  for (int i = 1; i <= segments; i++) {
    float t = (float)i / segments;
    float angle = start_angle + t * (end_angle - start_angle);
    int x = cx + (int)(radius * cosf(angle) + 0.5f);
    int y = cy + (int)(radius * sinf(angle) + 0.5f);
    apricot_draw_line(canvas, prev_x, prev_y, x, y, color);
    prev_x = x;
    prev_y = y;
  }
}

APRICOT_BEZIER_DEF void apricot_draw_slice(const ApricotCanvas *canvas, int cx,
                                           int cy, int radius,
                                           float start_angle, float end_angle,
                                           ApricotColor color) {
  if (radius <= 0)
    return;
  apricot_draw_arc(canvas, cx, cy, radius, start_angle, end_angle, color);
  int x_start = cx + (int)(radius * cosf(start_angle) + 0.5f);
  int y_start = cy + (int)(radius * sinf(start_angle) + 0.5f);
  int x_end = cx + (int)(radius * cosf(end_angle) + 0.5f);
  int y_end = cy + (int)(radius * sinf(end_angle) + 0.5f);
  apricot_draw_line(canvas, cx, cy, x_start, y_start, color);
  apricot_draw_line(canvas, cx, cy, x_end, y_end, color);
}

// === Beziers ====
APRICOT_BEZIER_DEF void
apricot_draw_quadratic_bezier(const ApricotCanvas *canvas, int x0, int y0,
                              int x1, int y1, int x2, int y2,
                              ApricotColor color, int segments) {
  int prev_x = x0, prev_y = y0;
  int global_step = 0;

  for (int i = 1; i <= segments; i++) {
    float t = (float)i / segments;
    float u = 1.0f - t;
    int x = (int)(u * u * x0 + 2 * u * t * x1 + t * t * x2 + 0.5f);
    int y = (int)(u * u * y0 + 2 * u * t * y1 + t * t * y2 + 0.5f);

    // Bresenham line with style/thickness
    int dx = abs(x - prev_x), sx = prev_x < x ? 1 : -1;
    int dy = -abs(y - prev_y), sy = prev_y < y ? 1 : -1;
    int err = dx + dy, e2;
    int cx = prev_x, cy = prev_y;
    int step = global_step;
    int dash_len = apricot_line_thickness * 3;
    int dot_spacing = apricot_line_thickness * 5;

    while (1) {
      int draw_pixel = 1;
      if (apricot_line_style == APRICOT_LINE_DASHED)
        draw_pixel = (step / dash_len) % 2 == 0;
      else if (apricot_line_style == APRICOT_LINE_DOTTED)
        draw_pixel = (step % dot_spacing) == 0;

      if (draw_pixel) {
        int half = apricot_line_thickness / 2;
        for (int tx = -half; tx <= half; tx++)
          for (int ty = -half; ty <= half; ty++) {
            int px = cx + tx, py = cy + ty;
            if (px >= 0 && px < (int)canvas->width && py >= 0 &&
                py < (int)canvas->height)
              APRICOT_SET_PIXEL(canvas, px, py, color);
          }
      }

      if (cx == x && cy == y)
        break;
      e2 = 2 * err;
      if (e2 >= dy) {
        err += dy;
        cx += sx;
      }
      if (e2 <= dx) {
        err += dx;
        cy += sy;
      }
      step++;
    }

    global_step = step;
    prev_x = x;
    prev_y = y;
  }
}

APRICOT_BEZIER_DEF void
apricot_draw_cubic_bezier(const ApricotCanvas *canvas, int x0, int y0, int x1,
                          int y1, int x2, int y2, int x3, int y3,
                          ApricotColor color, int segments) {
  int prev_x = x0, prev_y = y0;
  int global_step = 0;

  for (int i = 1; i <= segments; i++) {
    float t = (float)i / segments;
    float u = 1.0f - t;
    int x = (int)(u * u * u * x0 + 3 * u * u * t * x1 + 3 * u * t * t * x2 +
                  t * t * t * x3 + 0.5f);
    int y = (int)(u * u * u * y0 + 3 * u * u * t * y1 + 3 * u * t * t * y2 +
                  t * t * t * y3 + 0.5f);

    int dx = abs(x - prev_x), sx = prev_x < x ? 1 : -1;
    int dy = -abs(y - prev_y), sy = prev_y < y ? 1 : -1;
    int err = dx + dy, e2;
    int cx = prev_x, cy = prev_y;
    int step = global_step;
    int dash_len = apricot_line_thickness * 3;
    int dot_spacing = apricot_line_thickness * 5;

    while (1) {
      int draw_pixel = 1;
      if (apricot_line_style == APRICOT_LINE_DASHED)
        draw_pixel = (step / dash_len) % 2 == 0;
      else if (apricot_line_style == APRICOT_LINE_DOTTED)
        draw_pixel = (step % dot_spacing) == 0;

      if (draw_pixel) {
        int half = apricot_line_thickness / 2;
        for (int tx = -half; tx <= half; tx++)
          for (int ty = -half; ty <= half; ty++) {
            int px = cx + tx, py = cy + ty;
            if (px >= 0 && px < (int)canvas->width && py >= 0 &&
                py < (int)canvas->height)
              APRICOT_SET_PIXEL(canvas, px, py, color);
          }
      }

      if (cx == x && cy == y)
        break;
      e2 = 2 * err;
      if (e2 >= dy) {
        err += dy;
        cx += sx;
      }
      if (e2 <= dx) {
        err += dx;
        cy += sy;
      }
      step++;
    }

    global_step = step;
    prev_x = x;
    prev_y = y;
  }
}

APRICOT_BEZIER_DEF void apricot_draw_catmull_rom(const ApricotCanvas *canvas,
                                                 int *x, int *y, int n,
                                                 ApricotColor color,
                                                 int segments) {
  if (n < 4)
    return;

  int global_step = 0;

  for (int i = 0; i < n - 3; i++) {
    int prev_x = x[i + 1], prev_y = y[i + 1];

    for (int j = 1; j <= segments; j++) {
      float t = (float)j / segments;
      float t2 = t * t, t3 = t * t * t;

      float fx =
          0.5f * ((2 * x[i + 1]) + (-x[i] + x[i + 2]) * t +
                  (2 * x[i] - 5 * x[i + 1] + 4 * x[i + 2] - x[i + 3]) * t2 +
                  (-x[i] + 3 * x[i + 1] - 3 * x[i + 2] + x[i + 3]) * t3);

      float fy =
          0.5f * ((2 * y[i + 1]) + (-y[i] + y[i + 2]) * t +
                  (2 * y[i] - 5 * y[i + 1] + 4 * y[i + 2] - y[i + 3]) * t2 +
                  (-y[i] + 3 * y[i + 1] - 3 * y[i + 2] + y[i + 3]) * t3);

      int x1 = (int)(fx + 0.5f);
      int y1 = (int)(fy + 0.5f);

      int dx = abs(x1 - prev_x), sx = prev_x < x1 ? 1 : -1;
      int dy = -abs(y1 - prev_y), sy = prev_y < y1 ? 1 : -1;
      int err = dx + dy, e2;
      int cx = prev_x, cy = prev_y;
      int step = global_step;
      int dash_len = apricot_line_thickness * 3;
      int dot_spacing = apricot_line_thickness * 5;

      while (1) {
        int draw_pixel = 1;
        if (apricot_line_style == APRICOT_LINE_DASHED)
          draw_pixel = (step / dash_len) % 2 == 0;
        else if (apricot_line_style == APRICOT_LINE_DOTTED)
          draw_pixel = (step % dot_spacing) == 0;

        if (draw_pixel) {
          int half = apricot_line_thickness / 2;
          for (int tx = -half; tx <= half; tx++)
            for (int ty = -half; ty <= half; ty++) {
              int px = cx + tx, py = cy + ty;
              if (px >= 0 && px < (int)canvas->width && py >= 0 &&
                  py < (int)canvas->height)
                APRICOT_SET_PIXEL(canvas, px, py, color);
            }
        }

        if (cx == x1 && cy == y1)
          break;
        e2 = 2 * err;
        if (e2 >= dy) {
          err += dy;
          cx += sx;
        }
        if (e2 <= dx) {
          err += dx;
          cy += sy;
        }
        step++;
      }

      global_step = step;
      prev_x = x1;
      prev_y = y1;
    }
  }
}

APRICOT_BEZIER_DEF void
apricot_draw_quadratic_spline(const ApricotCanvas *canvas, int *points, int n,
                              ApricotColor color, int segments) {
  if (n < 3)
    return;

  // Chain quadratic segments: 0-1-2, 2-3-4, 4-5-6...
  for (int i = 0; i <= n - 3; i += 2) {
    apricot_draw_quadratic_bezier(canvas, points[i], points[i + 1],
                                  points[i + 2], points[i + 3], points[i + 4],
                                  points[i + 5], color, segments);
  }
}

APRICOT_BEZIER_DEF void apricot_draw_cubic_spline(const ApricotCanvas *canvas,
                                                  int *points, int n,
                                                  ApricotColor color,
                                                  int segments) {
  if (n < 4)
    return;

  // Chain cubic segments: 0-1-2-3, 3-4-5-6, 6-7-8-9...
  for (int i = 0; i <= n - 4; i += 3) {
    apricot_draw_cubic_bezier(canvas, points[i], points[i + 1], points[i + 2],
                              points[i + 3], points[i + 4], points[i + 5],
                              points[i + 6], points[i + 7], color, segments);
  }
}

#endif /* APRICOT_IMPLEMENTATION */
