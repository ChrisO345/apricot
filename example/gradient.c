#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.h"

int main() {
  const int w = 300, h = 200;
  uint32_t *pixels = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  ApricotCanvas canvas = apricot_canvas(pixels, w, h);
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  apricot_fill_rect_gradient(
      &canvas, 20, 20, 100, 100, apricot_color(255, 0, 0, 255),
      apricot_color(0, 0, 255, 255), APRICOT_GRADIENT_TOP_BOTTOM);

  apricot_fill_rect_gradient(
      &canvas, 150, 20, 100, 100, apricot_color(0, 255, 0, 255),
      apricot_color(255, 255, 0, 255), APRICOT_GRADIENT_LEFT_RIGHT);

  apricot_save_bmp("example/images/gradient.bmp", &canvas);
  free(pixels);
  return 0;
}
