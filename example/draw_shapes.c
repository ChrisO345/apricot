#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.h"

int main() {
  const int w = 300, h = 200;
  uint32_t *pixels = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  ApricotCanvas canvas = apricot_canvas(pixels, w, h);
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  apricot_set_line_thickness(3);
  apricot_draw_line(&canvas, 10, 10, 200, 50, apricot_color(255, 0, 0, 255));
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_draw_rect(&canvas, 50, 60, 100, 50, apricot_color(0, 255, 0, 255));
  apricot_set_line_style(APRICOT_LINE_DOTTED);
  apricot_draw_circle(&canvas, 200, 100, 40, apricot_color(0, 0, 255, 255));

  int tri_x[] = {50, 150, 100};
  int tri_y[] = {120, 120, 180};
  apricot_draw_triangle(&canvas, tri_x[0], tri_y[0], tri_x[1], tri_y[1],
                        tri_x[2], tri_y[2], apricot_color(255, 0, 255, 255));

  int poly_x[] = {200, 220, 260, 240};
  int poly_y[] = {120, 100, 140, 160};
  apricot_draw_polygon(&canvas, poly_x, poly_y, 4,
                       apricot_color(0, 255, 255, 255));

  apricot_save_bmp("example/images/draw_shapes.bmp", &canvas);
  free(pixels);
  return 0;
}
