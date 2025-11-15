#define APRICOT_IMPLEMENTATION
#include "../apricot.c"
#include "../apricot_bezier.c"

int main() {
  const int w = 400, h = 300;
  uint32_t *pixels = (uint32_t *)malloc(w * h * sizeof(uint32_t));
  ApricotCanvas canvas = apricot_canvas(pixels, w, h);
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  // --- Single quadratic Bezier ---
  apricot_set_line_thickness(2);
  apricot_set_line_style(APRICOT_LINE_SOLID);
  apricot_draw_quadratic_bezier(&canvas, 10, 10, 50, 80, 100, 10,
                                apricot_color(255, 0, 0, 255), 50);

  // --- Single cubic Bezier ---
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_draw_cubic_bezier(&canvas, 120, 10, 150, 80, 200, 80, 230, 10,
                            apricot_color(0, 255, 0, 255), 50);

  // --- Catmull-Rom spline ---
  apricot_set_line_style(APRICOT_LINE_DOTTED);
  int cr_x[] = {250, 270, 290, 270, 250};
  int cr_y[] = {10, 50, 10, 90, 130};
  apricot_draw_catmull_rom(&canvas, cr_x, cr_y, 5,
                           apricot_color(0, 0, 255, 255), 50);

  // --- Quadratic spline (chained) ---
  apricot_set_line_style(APRICOT_LINE_SOLID);
  apricot_set_line_thickness(3);
  int quad_points[] = {50, 150, 90, 190, 130, 150, 170, 190, 210, 150};
  apricot_draw_quadratic_spline(&canvas, quad_points, 10,
                                apricot_color(255, 128, 0, 255), 30);

  // --- Cubic spline (chained) ---
  apricot_set_line_style(APRICOT_LINE_DASHED);
  int cubic_points[] = {250, 150, 270, 190, 310, 190, 330, 150,
                        350, 190, 370, 150, 390, 190, 410, 150};
  apricot_draw_cubic_spline(&canvas, cubic_points, 16,
                            apricot_color(128, 0, 255, 255), 40);

  apricot_save_bmp("example/images/bezier_splines.bmp", &canvas);
  free(pixels);
  return 0;
}
