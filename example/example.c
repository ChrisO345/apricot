#define APRICOT_IMPLEMENTATION
#include "../apricot.c"

#include <stdio.h>
#include <stdlib.h>

int main() {
  const size_t width = 400, height = 400;
  uint32_t *pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!pixels)
    return 1;

  ApricotCanvas canvas = apricot_canvas(pixels, width, height);

  // Clear canvas
  apricot_clear(&canvas, apricot_color(0, 0, 0, 255));

  // Draw filled rectangle
  apricot_set_line_thickness(5);
  apricot_fill_rect(&canvas, 50, 50, 100, 80, apricot_color(255, 0, 0, 255));
  apricot_draw_rect(&canvas, 50, 50, 100, 80, apricot_color(0, 0, 255, 255));

  // Draw filled circle
  apricot_fill_circle(&canvas, 200, 100, 50, apricot_color(0, 255, 0, 255));
  apricot_draw_circle(&canvas, 200, 100, 50, apricot_color(255, 0, 0, 255));

  // Draw triangle
  apricot_fill_triangle(&canvas, 50, 200, 150, 200, 100, 300,
                        apricot_color(0, 0, 255, 255));
  apricot_draw_triangle(&canvas, 50, 200, 150, 200, 100, 300,
                        apricot_color(0, 255, 0, 255));

  // Draw polygon (pentagon)
  int px[] = {250, 300, 350, 325, 275};
  int py[] = {200, 210, 250, 300, 300};
  apricot_fill_polygon(&canvas, px, py, 5, apricot_color(255, 165, 0, 255));

  // Draw rectangle gradient
  apricot_fill_rect_gradient(
      &canvas, 50, 320, 300, 60, apricot_color(255, 0, 255, 255),
      apricot_color(0, 255, 255, 255), APRICOT_GRADIENT_LEFT_RIGHT);

  // Lines
  apricot_set_line_thickness(3);
  apricot_draw_line(&canvas, 0, 0, 400, 400, apricot_color(255, 255, 255, 255));
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_draw_line(&canvas, 400, 0, 0, 400, apricot_color(255, 255, 255, 255));
  apricot_set_line_style(APRICOT_LINE_DOTTED);
  apricot_draw_line(&canvas, 200, 0, 200, 400, apricot_color(255, 255, 0, 255));

  // Save images
  if (apricot_save_ppm("example/images/output.ppm", &canvas) != 0) {
    fprintf(stderr, "Failed to save PPM\n");
  }

  free(pixels);
  return 0;
}
