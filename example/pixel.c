#define APRICOT_IMPLEMENTATION
#include "../apricot.h"

int main() {
  const int w = 200, h = 150;
  uint32_t *pixels = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  ApricotCanvas canvas = apricot_canvas(pixels, w, h);

  // Fill background white
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  // Draw a diagonal line with set_pixel
  for (int i = 0; i < 100; i++) {
    apricot_set_pixel(&canvas, i, i, apricot_color(255, 0, 0, 255));
  }

  // Draw a grid of dots
  for (int y = 0; y < h; y += 20) {
    for (int x = 0; x < w; x += 20) {
      apricot_set_line_thickness((int)(x / 20) + 1);
      apricot_dot(&canvas, x, y, apricot_color(0, 0, 255, 255));
    }
  }

  apricot_save_bmp("example/images/pixel.bmp", &canvas);
  free(pixels);
  return 0;
}
