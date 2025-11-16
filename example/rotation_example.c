#include <stdio.h>
#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#define APRICOT_ENABLE_DEFAULT_ROTATION
#define APRICOT_ENABLE_ALPHA_BLENDING
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

  // Rotation blit
  const int gw = 100, gh = 100;
  uint32_t *gpixels = malloc(gw * gh * sizeof(uint32_t));
  if (!gpixels) {
    fprintf(stderr, "Failed to allocate memory for gradient canvas\n");
    free(pixels);
    return 1;
  }
  ApricotCanvas grad_canvas = apricot_canvas(gpixels, gw, gh);
  apricot_fill_rect_gradient(
      &grad_canvas, 0, 0, gw, gh, apricot_color(255, 255, 0, 255),
      apricot_color(255, 0, 255, 255), APRICOT_GRADIENT_TOPLEFT_BOTTOMRIGHT);

  apricot_set_rotation(45.0f); // Rotate 45 degrees
  apricot_blit(&canvas, &grad_canvas, 250, 200);

  // Polygon on rotated canvas
  uint32_t *ppixels = malloc(gw * gh * sizeof(uint32_t));
  if (!ppixels) {
    fprintf(stderr, "Failed to allocate memory for polygon canvas\n");
    free(gpixels);
    free(pixels);
    return 1;
  }

  ApricotCanvas poly_canvas = apricot_canvas(ppixels, gw, gh);
  apricot_clear(&poly_canvas, apricot_color(255, 255, 255, 0)); // Transparent
  apricot_set_rotation(-20.0f); // Rotate -20 degrees
  int px[5] = {50, 80, 70, 30, 20};
  int py[5] = {10, 30, 70, 70, 30};
  apricot_fill_polygon(&poly_canvas, px, py, 5,
                       apricot_color(0, 150, 150, 255));
  apricot_set_rotation(0.0f); // Reset rotation
  apricot_blit(&canvas, &poly_canvas, 0, 0);
  apricot_set_rotation(15.0f); // Reset rotation
  apricot_blit(&canvas, &poly_canvas, 100, 0);

  // Save result
  if (apricot_save_bmp("example/images/rotation.bmp", &canvas) != 0) {
    fprintf(stderr, "Failed to save BMP\n");
  }

  free(gpixels);
  free(pixels);
  return 0;
}
