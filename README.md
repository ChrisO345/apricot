# apricot

*apricot* is a lightweight, single-file* C library for general-purpose 2D raster graphics.

---

## Features

* General-purpose 2D raster graphics.
* Draw points, lines, and basic shapes (rectangles, circles, triangles, polygons).
* Various line styles: solid, dashed, dotted.
* Optional alpha blending for transparency effects.
* Save images to BMP or PPM formats.

---

## Installation

Copy the sources into your project and include them:

```c
#define APRICOT_IMPLEMENTATION
#include "apricot.c"
```

---

## Extensions

Apricot supports optional extensions for additional functionality.
* **apricot_bezier.c**: Bezier curve drawing.
* **apricot_3d.c**: Basic 3D rendering capabilities.

To use an extension, include its source file after including `apricot.c`:

```c
#define APRICOT_IMPLEMENTATION
#include "apricot.c"
#include "apricot_bezier.c"  // For Bezier curve support
#include "apricot_3d.c"      // For basic 3D rendering
```

---

## Usage

```c
#include <stdio.h>
#include <stdlib.h>

#define APRICOT_IMPLEMENTATION
#include "../apricot.c"

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
  apricot_draw_rect(&canvas, 50, 80, 120, 60, apricot_color(0, 200, 0, 255));

  // Blue filled rectangle
  apricot_fill_rect(&canvas, 200, 70, 150, 80, apricot_color(0, 0, 255, 255));

  // Yellow circle
  apricot_fill_circle(&canvas, 100, 200, 40, apricot_color(255, 255, 0, 255));
  apricot_draw_circle(&canvas, 100, 200, 40, apricot_color(0, 0, 0, 255));

  // Magenta triangle
  apricot_fill_triangle(&canvas, 250, 180, 300, 250, 200, 250,
                        apricot_color(255, 0, 255, 255));
  apricot_draw_triangle(&canvas, 250, 180, 300, 250, 200, 250,
                        apricot_color(0, 0, 0, 255));

  // Gradient-filled rectangle
  apricot_fill_rect_gradient(
      &canvas, 50, 260, 300, 30, apricot_color(255, 128, 0, 255),
      apricot_color(0, 128, 255, 255), APRICOT_GRADIENT_LEFT_RIGHT);

  // Inverted color line
  ApricotColor red = apricot_color(255, 0, 0, 255);
  ApricotColor inv = apricot_color_invert(red);
  apricot_set_line_style(APRICOT_LINE_DASHED);
  apricot_draw_line(&canvas, 20, 280, 380, 290, inv);

  // Save result
  if (apricot_save_bmp("example/images/readme.bmp", &canvas) != 0) {
    fprintf(stderr, "Failed to save BMP\n");
  }

  free(pixels);
  return 0;
}```

Other examples can be found in the [examples](example/) folder.

---

## License

Licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
