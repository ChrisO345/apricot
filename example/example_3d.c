#define APRICOT_IMPLEMENTATION

#include "../apricot.h"
#include "../apricot_3d.h"

#include <stdio.h>

int main() {
  const size_t width = 400, height = 400;
  uint32_t *pixels = (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!pixels)
    return 1;

  ApricotCanvas canvas = apricot_canvas(pixels, 400, 400);
  apricot_clear(&canvas, apricot_color(0, 0, 0, 255));

  ApricotRotation rot = {0.2f, 0.8f, 0.1f};
  ApricotColor white = apricot_color(255, 255, 255, 255);
  ApricotColor red = apricot_color(255, 0, 0, 255);
  ApricotColor green = apricot_color(0, 255, 0, 255);

  apricot_3d_render_wireframe(&canvas, (Apricot3DVector){200, 200, 0}, 100, 80,
                              150, rot, APRICOT_RECT_CENTER, white);

  apricot_set_line_thickness(5);
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_3d_render_wireframe(&canvas, (Apricot3DVector){100, 100, 0}, 30, 180,
                              50, rot, APRICOT_RECT_TOP_LEFT_FRONT, red);

  apricot_set_line_thickness(3);
  apricot_set_line_style(APRICOT_LINE_DOTTED);
  apricot_3d_render_wireframe(&canvas, (Apricot3DVector){300, 300, 0}, 150, 40,
                              70, rot, APRICOT_RECT_BOTTOM_LEFT_FRONT, green);

  // Save to PPM
  if (apricot_save_ppm("example/images/output_3d.ppm", &canvas) != 0) {
    fprintf(stderr, "Failed to save PPM\n");
  }

  free(pixels);
  return 0;
}
