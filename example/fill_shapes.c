#include <stdlib.h>

#define APRICOT_ENABLE_ALPHA_BLENDING
#define APRICOT_IMPLEMENTATION
#include "../apricot.h"

int main() {
  const int w = 300, h = 200;
  uint32_t *pixels = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  ApricotCanvas canvas = apricot_canvas(pixels, w, h);
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  apricot_fill_rect(&canvas, 10, 10, 100, 60, apricot_color(255, 0, 0, 128));
  apricot_fill_circle(&canvas, 100, 50, 30, apricot_color(0, 255, 0, 128));
  apricot_fill_ellipse(&canvas, 250, 50, 40, 20, apricot_color(0, 0, 255, 128));
  apricot_fill_triangle(&canvas, 50, 120, 150, 180, 20, 170,
                        apricot_color(255, 255, 0, 128));

  int poly_x[] = {200, 250, 280, 220};
  int poly_y[] = {120, 110, 160, 180};
  apricot_fill_polygon(&canvas, poly_x, poly_y, 4,
                       apricot_color(255, 0, 255, 128));

  apricot_save_bmp("example/images/fill_shapes.bmp", &canvas);
  free(pixels);
  return 0;
}
