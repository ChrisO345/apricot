#include <stdio.h>
#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.h"
#include "../apricot_rotation.h"

int main() {
  const int w = 400, h = 300;
  uint32_t *pixels = malloc(w * h * sizeof(uint32_t));
  if (!pixels) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }

  ApricotCanvas canvas = apricot_canvas(pixels, w, h);

  // Clear canvas to white
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  // Draw some basic shapes
  apricot_set_line_thickness(3);

  // Red line
  apricot_set_line_style(APRICOT_LINE_SOLID);
  apricot_draw_line(&canvas, 20, 20, 380, 50, apricot_color(255, 0, 0, 255));

  // Green rectangle (outlined)
  apricot_set_rotation(-30.0f); // Rotate -30 degrees
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_draw_rect(&canvas, 50, 80, 120, 60, apricot_color(0, 200, 0, 255));

  // Blue filled rectangle
  apricot_set_rotation(15.0f); // Rotate 15 degrees
  apricot_fill_rect(&canvas, 200, 70, 150, 80, apricot_color(0, 0, 255, 255));

  // Save result
  if (apricot_save_bmp("example/images/rotation.bmp", &canvas) != 0) {
    fprintf(stderr, "Failed to save BMP\n");
  }

  free(pixels);
  return 0;
}
