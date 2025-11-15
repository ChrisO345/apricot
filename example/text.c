#include <stdio.h>
#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.c"
#include "../apricot_ttf.c"

int main() {
  const int w = 400, h = 300;
  uint32_t *pixels = malloc(w * h * sizeof(uint32_t));
  if (!pixels) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }

  ApricotCanvas canvas = apricot_canvas(pixels, w, h);
  apricot_clear(&canvas, apricot_color(255, 255, 255, 255));

  static ApricotFont apricot_default_font = {
      .glyphs = &apricot_default_glyphs[0][0][0], .width = 5, .height = 6};
  apricot_draw_text(&canvas, &apricot_default_font, "hello world", 10, 10,
                    apricot_color(0, 0, 0, 255));

  apricot_set_glyph_scale(4);
  apricot_draw_text(&canvas, &apricot_default_font, "foo bar", 10, 80,
                    apricot_color(0, 0, 255, 255));

  if (apricot_save_bmp("example/images/text.bmp", &canvas) != 0) {
    fprintf(stderr, "Failed to save BMP\n");
  }
  free(pixels);
  return 0;
}
