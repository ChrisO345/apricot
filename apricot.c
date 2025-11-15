#ifndef APRICOT_C_
#define APRICOT_C_

#include <stdlib.h>
#ifndef APRICOTCDEF
#define APRICOTCDEF static inline
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define APRICOT_VERSION "0.0.0"

typedef struct {
  uint32_t *pixels;
  size_t width;
  size_t height;
} ApricotCanvas;

typedef union {
  uint32_t value;
  struct {
    uint8_t r, g, b, a;
  };
} ApricotColor;

typedef enum {
  APRICOT_GRADIENT_TOP_BOTTOM,
  APRICOT_GRADIENT_LEFT_RIGHT,
  APRICOT_GRADIENT_TOPLEFT_BOTTOMRIGHT,
  APRICOT_GRADIENT_TOPRIGHT_BOTTOMLEFT
} ApricotGradientDirection;

// === Instantiation ===
APRICOTCDEF ApricotCanvas apricot_canvas(uint32_t *pixels, size_t width,
                                         size_t height);
APRICOTCDEF void apricot_clear(const ApricotCanvas *canvas, ApricotColor color);

APRICOTCDEF ApricotColor apricot_color(uint8_t r, uint8_t g, uint8_t b,
                                       uint8_t a);

// === Drawing Functions ===
APRICOTCDEF void apricot_fill_rect(const ApricotCanvas *canvas, int x, int y,
                                   int w, int h, ApricotColor color);
APRICOTCDEF void apricot_fill_circle(const ApricotCanvas *canvas, int cx,
                                     int cy, int radius, ApricotColor color);
APRICOTCDEF void apricot_fill_triangle(const ApricotCanvas *canvas, int x0,
                                       int y0, int x1, int y1, int x2, int y2,
                                       ApricotColor color);
APRICOTCDEF void apricot_fill_polygon(const ApricotCanvas *canvas, int *x,
                                      int *y, int n, ApricotColor color);

APRICOTCDEF void apricot_draw_rect(const ApricotCanvas *canvas, int x, int y,
                                   int w, int h, int thickness,
                                   ApricotColor color);
APRICOTCDEF void apricot_draw_line(const ApricotCanvas *canvas, int x0, int y0,
                                   int x1, int y1, int thickness,
                                   ApricotColor color);
APRICOTCDEF void apricot_draw_circle(const ApricotCanvas *canvas, int cx,
                                     int cy, int radius, int thickness,
                                     ApricotColor color);
APRICOTCDEF void apricot_draw_triangle(const ApricotCanvas *canvas, int x0,
                                       int y0, int x1, int y1, int x2, int y2,
                                       int thickness, ApricotColor color);

APRICOTCDEF void apricot_fill_rect_gradient(const ApricotCanvas *canvas, int x,
                                            int y, int w, int h,
                                            ApricotColor c1, ApricotColor c2,
                                            ApricotGradientDirection dir);

// === Image Functions ===
APRICOTCDEF int apricot_save_ppm(const char *filename,
                                 const ApricotCanvas *canvas);
APRICOTCDEF int apricot_load_ppm(const char *filename, ApricotCanvas *canvas);
APRICOTCDEF int apricot_save_bmp(const char *filename,
                                 const ApricotCanvas *canvas);
APRICOTCDEF int apricot_load_bmp(const char *filename, ApricotCanvas *canvas);

#endif /* APRICOT_C_ */

#ifndef APRICOT_IMPLEMENTATION_
#define APRICOT_IMPLEMENTATION_

// === Instantiation ===
APRICOTCDEF ApricotCanvas apricot_canvas(uint32_t *pixels, size_t width,
                                         size_t height) {
  ApricotCanvas canvas;
  canvas.pixels = pixels;
  canvas.width = width;
  canvas.height = height;
  return canvas;
}

APRICOTCDEF void apricot_clear(const ApricotCanvas *canvas,
                               ApricotColor color) {
  size_t total_pixels = canvas->width * canvas->height;
  for (size_t i = 0; i < total_pixels; ++i) {
    canvas->pixels[i] = color.value;
  }
}

APRICOTCDEF ApricotColor apricot_color(uint8_t r, uint8_t g, uint8_t b,
                                       uint8_t a) {
  ApricotColor color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  return color;
}

// === Drawing Functions ===
APRICOTCDEF void apricot_fill_rect(const ApricotCanvas *canvas, int x, int y,
                                   int w, int h, ApricotColor color) {
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      int px = x + i;
      int py = y + j;
      if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height) {
        canvas->pixels[py * canvas->width + px] = color.value;
      }
    }
  }
}

APRICOTCDEF void apricot_fill_circle(const ApricotCanvas *canvas, int cx,
                                     int cy, int radius, ApricotColor color) {
  for (int y = -radius; y <= radius; ++y) {
    for (int x = -radius; x <= radius; ++x) {
      if (x * x + y * y <= radius * radius) {
        int px = cx + x;
        int py = cy + y;
        if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height) {
          canvas->pixels[py * canvas->width + px] = color.value;
        }
      }
    }
  }
}

APRICOTCDEF void apricot_fill_triangle(const ApricotCanvas *canvas, int x0,
                                       int y0, int x1, int y1, int x2, int y2,
                                       ApricotColor color) {
  int minX = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
  int maxX = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
  int minY = y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2);
  int maxY = y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2);

  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      int w0 = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
      int w1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
      int w2 = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
      if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
        if (x >= 0 && x < canvas->width && y >= 0 && y < canvas->height) {
          canvas->pixels[y * canvas->width + x] = color.value;
        }
      }
    }
  }
}

APRICOTCDEF void apricot_fill_polygon(const ApricotCanvas *canvas, int *x,
                                      int *y, int n, ApricotColor color) {
  if (n < 3)
    return; // Not a polygon

  // Find vertical bounds
  int minY = y[0], maxY = y[0];
  for (int i = 1; i < n; ++i) {
    if (y[i] < minY)
      minY = y[i];
    if (y[i] > maxY)
      maxY = y[i];
  }

  // Clip to canvas
  if (minY < 0)
    minY = 0;
  if (maxY >= (int)canvas->height)
    maxY = canvas->height - 1;

  // Scanline fill
  for (int scanY = minY; scanY <= maxY; ++scanY) {
    int intersections[n];
    int count = 0;

    for (int i = 0; i < n; ++i) {
      int j = (i + 1) % n;
      int y0 = y[i], y1 = y[j];
      int x0 = x[i], x1 = x[j];

      if ((scanY >= y0 && scanY < y1) || (scanY >= y1 && scanY < y0)) {
        int intersectX = x0 + (scanY - y0) * (x1 - x0) / (y1 - y0);
        intersections[count++] = intersectX;
      }
    }

    // Sort intersections
    for (int i = 0; i < count - 1; ++i) {
      for (int j = i + 1; j < count; ++j) {
        if (intersections[i] > intersections[j]) {
          int temp = intersections[i];
          intersections[i] = intersections[j];
          intersections[j] = temp;
        }
      }
    }

    // Fill between pairs
    for (int i = 0; i < count; i += 2) {
      int startX = intersections[i];
      int endX = intersections[i + 1];
      if (startX < 0)
        startX = 0;
      if (endX >= (int)canvas->width)
        endX = canvas->width - 1;
      for (int xFill = startX; xFill <= endX; ++xFill) {
        canvas->pixels[scanY * canvas->width + xFill] = color.value;
      }
    }
  }
}

APRICOTCDEF void apricot_draw_rect(const ApricotCanvas *canvas, int x, int y,
                                   int w, int h, int thickness,
                                   ApricotColor color) {
  // Top edge
  apricot_draw_line(canvas, x, y, x + w - 1, y, thickness, color);
  // Bottom edge
  apricot_draw_line(canvas, x, y + h - 1, x + w - 1, y + h - 1, thickness,
                    color);
  // Left edge
  apricot_draw_line(canvas, x, y, x, y + h - 1, thickness, color);
  // Right edge
  apricot_draw_line(canvas, x + w - 1, y, x + w - 1, y + h - 1, thickness,
                    color);
}

APRICOTCDEF void apricot_draw_line(const ApricotCanvas *canvas, int x0, int y0,
                                   int x1, int y1, int thickness,
                                   ApricotColor color) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  while (1) {
    // Draw a square around (x0, y0) for thickness
    for (int tx = -thickness / 2; tx <= thickness / 2; ++tx) {
      for (int ty = -thickness / 2; ty <= thickness / 2; ++ty) {
        int px = x0 + tx;
        int py = y0 + ty;
        if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height) {
          canvas->pixels[py * canvas->width + px] = color.value;
        }
      }
    }

    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

APRICOTCDEF void apricot_draw_circle(const ApricotCanvas *canvas, int cx,
                                     int cy, int radius, int thickness,
                                     ApricotColor color) {
  if (radius <= 0 || thickness <= 0)
    return;

  int r_outer = radius + thickness / 2;
  int r_inner = radius - (thickness - 1) / 2;
  if (r_inner < 0)
    r_inner = 0;

  // Iterate bounding square
  for (int y = -r_outer; y <= r_outer; ++y) {
    for (int x = -r_outer; x <= r_outer; ++x) {
      int dist2 = x * x + y * y;
      if (dist2 >= r_inner * r_inner && dist2 <= r_outer * r_outer) {
        int px = cx + x;
        int py = cy + y;
        if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height) {
          canvas->pixels[py * canvas->width + px] = color.value;
        }
      }
    }
  }
}

APRICOTCDEF void apricot_draw_triangle(const ApricotCanvas *canvas, int x0,
                                       int y0, int x1, int y1, int x2, int y2,
                                       int thickness, ApricotColor color) {
  apricot_draw_line(canvas, x0, y0, x1, y1, thickness, color);
  apricot_draw_line(canvas, x1, y1, x2, y2, thickness, color);
  apricot_draw_line(canvas, x2, y2, x0, y0, thickness, color);
}

APRICOTCDEF void apricot_fill_rect_gradient(const ApricotCanvas *canvas, int x,
                                            int y, int w, int h,
                                            ApricotColor c1, ApricotColor c2,
                                            ApricotGradientDirection dir) {
  if (w <= 0 || h <= 0)
    return;

  int xEnd = x + w;
  int yEnd = y + h;

  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (xEnd > (int)canvas->width)
    xEnd = canvas->width;
  if (yEnd > (int)canvas->height)
    yEnd = canvas->height;

  for (int j = y; j < yEnd; ++j) {
    for (int i = x; i < xEnd; ++i) {
      float t = 0.0f;
      switch (dir) {
      case APRICOT_GRADIENT_TOP_BOTTOM:
        t = (float)(j - y) / (h - 1);
        break;
      case APRICOT_GRADIENT_LEFT_RIGHT:
        t = (float)(i - x) / (w - 1);
        break;
      case APRICOT_GRADIENT_TOPLEFT_BOTTOMRIGHT:
        t = ((float)(i - x) / (w - 1) + (float)(j - y) / (h - 1)) / 2.0f;
        break;
      case APRICOT_GRADIENT_TOPRIGHT_BOTTOMLEFT:
        t = ((float)(xEnd - 1 - i) / (w - 1) + (float)(j - y) / (h - 1)) / 2.0f;
        break;
      }

      uint8_t r = (uint8_t)((1 - t) * c1.r + t * c2.r);
      uint8_t g = (uint8_t)((1 - t) * c1.g + t * c2.g);
      uint8_t b = (uint8_t)((1 - t) * c1.b + t * c2.b);
      uint8_t a = (uint8_t)((1 - t) * c1.a + t * c2.a);
      canvas->pixels[j * canvas->width + i] = apricot_color(r, g, b, a).value;
    }
  }
}

// === Image Functions ===
APRICOTCDEF int apricot_save_ppm(const char *filename,
                                 const ApricotCanvas *canvas) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to open file for writing");
    return -1;
  }

  fprintf(file, "P6\n%zu %zu\n255\n", canvas->width, canvas->height);

  int has_alpha = 0;
  for (size_t y = 0; y < canvas->height; ++y) {
    for (size_t x = 0; x < canvas->width; ++x) {
      uint32_t pixel = canvas->pixels[y * canvas->width + x];
      uint8_t bytes[3] = {
          (pixel >> (8 * 0)) & 0xFF,
          (pixel >> (8 * 1)) & 0xFF,
          (pixel >> (8 * 2)) & 0xFF,
      };

      // Log error if alpha is not 255 for ppm
      uint8_t alpha = (pixel >> (8 * 3)) & 0xFF;
      if (alpha != 255 && pixel >> (8 * 3) != 0) {
        has_alpha = 1;
      }

      fwrite(bytes, sizeof(bytes), 1, file);
      if (ferror(file)) {
        fclose(file);
        return -1;
      }
    }
  }

  if (has_alpha == 1) {
    fprintf(stderr,
            "Warning: Some pixels have alpha != 255. PPM format does not "
            "support alpha channel.\n");
  }

  fclose(file);
  return 0;
}

APRICOTCDEF int apricot_load_ppm(const char *filename, ApricotCanvas *canvas) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return -1;

  char format[3];
  int width, height, maxval;
  if (fscanf(f, "%2s\n%d %d\n%d\n", format, &width, &height, &maxval) != 4) {
    fclose(f);
    return -1;
  }
  if (format[0] != 'P' || format[1] != '6' || maxval != 255) {
    fclose(f);
    return -1;
  }

  if ((size_t)(width * height) > canvas->width * canvas->height) {
    fclose(f);
    return -1;
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      uint8_t rgb[3];
      if (fread(rgb, 1, 3, f) != 3) {
        fclose(f);
        return -1;
      }
      canvas->pixels[y * canvas->width + x] =
          apricot_color(rgb[0], rgb[1], rgb[2], 255).value;
    }
  }

  fclose(f);
  return 0;
}

APRICOTCDEF int apricot_save_bmp(const char *filename,
                                 const ApricotCanvas *canvas) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to open file for writing");
    return -1;
  }

  // Each row must be aligned to 4 bytes
  uint32_t row_size = (canvas->width * 3 + 3) & ~3;
  uint32_t pixel_data_size = row_size * canvas->height;
  uint32_t file_size = 54 + pixel_data_size;

  // BMP Header
  uint8_t bmp_header[54] = {0};
  bmp_header[0] = 'B';
  bmp_header[1] = 'M';
  bmp_header[2] = file_size & 0xFF;
  bmp_header[3] = (file_size >> 8) & 0xFF;
  bmp_header[4] = (file_size >> 16) & 0xFF;
  bmp_header[5] = (file_size >> 24) & 0xFF;
  bmp_header[10] = 54; // Offset to pixel data
  bmp_header[14] = 40; // DIB header size
  bmp_header[18] = canvas->width & 0xFF;
  bmp_header[19] = (canvas->width >> 8) & 0xFF;
  bmp_header[20] = (canvas->width >> 16) & 0xFF;
  bmp_header[21] = (canvas->width >> 24) & 0xFF;
  bmp_header[22] = canvas->height & 0xFF;
  bmp_header[23] = (canvas->height >> 8) & 0xFF;
  bmp_header[24] = (canvas->height >> 16) & 0xFF;
  bmp_header[25] = (canvas->height >> 24) & 0xFF;
  bmp_header[26] = 1;  // Color planes
  bmp_header[28] = 24; // Bits per pixel
  bmp_header[34] = pixel_data_size & 0xFF;
  bmp_header[35] = (pixel_data_size >> 8) & 0xFF;
  bmp_header[36] = (pixel_data_size >> 16) & 0xFF;
  bmp_header[37] = (pixel_data_size >> 24) & 0xFF;

  fwrite(bmp_header, 1, 54, file);

  // Write pixels bottom-up
  uint8_t *row = (uint8_t *)malloc(row_size);
  if (!row) {
    fclose(file);
    return -1;
  }

  for (int y = canvas->height - 1; y >= 0; --y) {
    for (size_t x = 0; x < canvas->width; ++x) {
      uint32_t px = canvas->pixels[y * canvas->width + x];
      row[x * 3 + 2] = px & 0xFF;         // B
      row[x * 3 + 1] = (px >> 8) & 0xFF;  // G
      row[x * 3 + 0] = (px >> 16) & 0xFF; // R
    }
    // Pad the rest of the row with zeros if needed
    for (size_t p = canvas->width * 3; p < row_size; ++p) {
      row[p] = 0;
    }
    fwrite(row, 1, row_size, file);
  }

  free(row);
  fclose(file);
  return 0;
}

APRICOTCDEF int apricot_load_bmp(const char *filename, ApricotCanvas *canvas) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return -1;

  uint8_t header[54];
  if (fread(header, 1, 54, f) != 54) {
    fclose(f);
    return -1;
  }

  int width = *(int *)&header[18];
  int height = *(int *)&header[22];
  int row_padded = (width * 3 + 3) & (~3);

  if ((size_t)(width * height) > canvas->width * canvas->height) {
    fclose(f);
    return -1;
  }

  uint8_t *row = (uint8_t *)malloc(row_padded);
  if (!row) {
    fclose(f);
    return -1;
  }

  for (int y = height - 1; y >= 0; --y) {
    if (fread(row, 1, row_padded, f) != (size_t)row_padded) {
      free(row);
      fclose(f);
      return -1;
    }
    for (int x = 0; x < width; ++x) {
      uint8_t b = row[x * 3 + 0];
      uint8_t g = row[x * 3 + 1];
      uint8_t r = row[x * 3 + 2];
      canvas->pixels[y * canvas->width + x] = apricot_color(r, g, b, 255).value;
    }
  }

  free(row);
  fclose(f);
  return 0;
}

#endif /* APRICOT_IMPLEMENTATION */
