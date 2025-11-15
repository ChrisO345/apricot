#define APRICOT_IMPLEMENTATION
#define APRICOT_ENABLE_DEFAULT_ROTATION
#include "apricot.c"

#ifndef APRICOT_ROT_C_
#define APRICOT_ROT_C_

#ifndef APRICOT_ROT_DEF
#define APRICOT_ROT_DEF static inline
#endif

#ifdef APRICOT_ENABLE_DEFAULT_ROTATION
#define apricot_fill_rect apricot_fill_rect_rotation
#define apricot_draw_rect apricot_draw_rect_rotation
#endif

static float apricot_rotation_deg = 0.0f;
APRICOT_ROT_DEF void apricot_set_rotation(float degrees);
APRICOT_ROT_DEF float apricot_get_rotation(void);

// === Rotated Functions ===
APRICOT_ROT_DEF void apricot_fill_rect_rotation(const ApricotCanvas *canvas,
                                                int x, int y, int w, int h,
                                                ApricotColor color);

// === Helpers ===
APRICOT_ROT_DEF void apricot_rotate_point_around(int cx, int cy, int x, int y,
                                                 int *out_x, int *out_y);
APRICOT_ROT_DEF void apricot_rotate_points_around(int cx, int cy, const int *x,
                                                  const int *y, int *out_x,
                                                  int *out_y, int n);
APRICOT_ROT_DEF void apricot_compute_rect_center(int x, int y, int w, int h,
                                                 int *cx, int *cy);
APRICOT_ROT_DEF void apricot_compute_points_center(const int *x, const int *y,
                                                   int n, int *cx, int *cy);

#endif /* APRICOT_ROT_C_ */

#ifdef APRICOT_IMPLEMENTATION

#include <math.h>

APRICOT_ROT_DEF void apricot_set_rotation(float degrees) {
  apricot_rotation_deg = degrees;
}

APRICOT_ROT_DEF float apricot_get_rotation(void) {
  return apricot_rotation_deg;
}

// === Rotated Functions ===
APRICOT_ROT_DEF void apricot_fill_rect_rotation(const ApricotCanvas *canvas,
                                                int x, int y, int w, int h,
                                                ApricotColor color) {
  if (w <= 0 || h <= 0)
    return;

  int cx, cy;
  apricot_compute_rect_center(x, y, w, h, &cx, &cy);

  int xpts[4] = {x, x + w - 1, x + w - 1, x};
  int ypts[4] = {y, y, y + h - 1, y + h - 1};
  int rx[4], ry[4];

  apricot_rotate_points_around(cx, cy, xpts, ypts, rx, ry, 4);

  apricot_fill_polygon(canvas, rx, ry, 4, color);
}

APRICOT_DEF void apricot_draw_rect_rotation(const ApricotCanvas *canvas, int x,
                                            int y, int w, int h,
                                            ApricotColor color) {
  if (w <= 0 || h <= 0)
    return;

  int cx, cy;
  apricot_compute_rect_center(x, y, w, h, &cx, &cy);

  int xpts[4] = {x, x + w - 1, x + w - 1, x};
  int ypts[4] = {y, y, y + h - 1, y + h - 1};
  int rx[4], ry[4];

  apricot_rotate_points_around(cx, cy, xpts, ypts, rx, ry, 4);

  apricot_draw_polygon(canvas, rx, ry, 4, color);
}

// === Helpers ===
APRICOT_ROT_DEF void apricot_rotate_point_around(int cx, int cy, int x, int y,
                                                 int *out_x, int *out_y) {
  if (apricot_rotation_deg == 0.0f) {
    *out_x = x;
    *out_y = y;
    return;
  }
  float theta = apricot_rotation_deg * 3.14159265358979323846f / 180.0f;
  float c = cosf(theta);
  float s = sinf(theta);
  float tx = (float)(x - cx);
  float ty = (float)(y - cy);
  float rx = tx * c - ty * s;
  float ry = tx * s + ty * c;
  *out_x = (int)roundf(rx + cx);
  *out_y = (int)roundf(ry + cy);
}

APRICOT_ROT_DEF void apricot_rotate_points_around(int cx, int cy, const int *x,
                                                  const int *y, int *out_x,
                                                  int *out_y, int n) {
  if (apricot_rotation_deg == 0.0f) {
    for (int i = 0; i < n; ++i) {
      out_x[i] = x[i];
      out_y[i] = y[i];
    }
    return;
  }
  for (int i = 0; i < n; ++i)
    apricot_rotate_point_around(cx, cy, x[i], y[i], &out_x[i], &out_y[i]);
}

APRICOT_ROT_DEF void apricot_compute_rect_center(int x, int y, int w, int h,
                                                 int *cx, int *cy) {
  *cx = x + w / 2;
  *cy = y + h / 2;
}

APRICOT_ROT_DEF void apricot_compute_points_center(const int *x, const int *y,
                                                   int n, int *cx, int *cy) {
  long sx = 0, sy = 0;
  for (int i = 0; i < n; ++i) {
    sx += x[i];
    sy += y[i];
  }
  *cx = (int)roundf((float)sx / n);
  *cy = (int)roundf((float)sy / n);
}

#endif /* APRICOT_IMPLEMENTATION */
