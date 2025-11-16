#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.h"

int main() {
  // Create two canvases
  const int w = 200, h = 150;
  uint32_t *pixels1 = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  uint32_t *pixels2 = (uint32_t *)malloc(w * h * sizeof(uint32_t));

  ApricotCanvas canvas1 = apricot_canvas(pixels1, w, h);
  ApricotCanvas canvas2 = apricot_canvas(pixels2, w, h);

  // Fill the first canvas with red
  apricot_clear(&canvas1, apricot_color(255, 0, 0, 255));

  // Fill the second canvas with blue
  apricot_clear(&canvas2, apricot_color(0, 0, 255, 255));

  // Blit the second canvas onto the first at offset (50, 30)
  apricot_blit(&canvas1, &canvas2, 50, 30);

  // Save the result
  apricot_save_bmp("example/images/blit.bmp", &canvas1);

  free(pixels1);
  free(pixels2);
  return 0;
}
