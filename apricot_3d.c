#ifndef APRICOT_3D_C_
#define APRICOT_3D_C_

#ifndef APRICOT3DDEF
#define APRICOT3DDEF extern
#endif

typedef struct {
  float x, y, z;
} Apricot3DVector;

typedef struct {
  float pitch;
  float yaw;
  float roll;
} ApricotRotation;

typedef enum {
  APRICOT_RECT_CENTER,
  APRICOT_RECT_TOP_LEFT_FRONT,
  APRICOT_RECT_BOTTOM_LEFT_FRONT
} Apricot3DRectAnchor;

APRICOT3DDEF void
apricot_3d_render_wireframe(const ApricotCanvas *canvas,
                            Apricot3DVector reference, float length,
                            float width, float height, ApricotRotation rotation,
                            Apricot3DRectAnchor anchor, ApricotColor color);

#endif /* APRICOT_3D_C_ */

#ifdef APRICOT_IMPLEMENTATION

#include <math.h>

APRICOT3DDEF void
apricot_3d_render_wireframe(const ApricotCanvas *canvas,
                            Apricot3DVector reference, float length,
                            float width, float height, ApricotRotation rotation,
                            Apricot3DRectAnchor anchor, ApricotColor color) {
  if (!canvas)
    return;

  float hx = length * 0.5f;
  float hy = height * 0.5f;
  float hz = width * 0.5f;

  float offsetX = 0, offsetY = 0, offsetZ = 0;

  switch (anchor) {
  case APRICOT_RECT_CENTER:
    offsetX = 0;
    offsetY = 0;
    offsetZ = 0;
    break;
  case APRICOT_RECT_TOP_LEFT_FRONT:
    offsetX = hx;
    offsetY = -hy;
    offsetZ = -hz;
    break;
  case APRICOT_RECT_BOTTOM_LEFT_FRONT:
    offsetX = hx;
    offsetY = hy;
    offsetZ = -hz;
    break;
  }

  Apricot3DVector v[8] = {{-hx - offsetX, -hy - offsetY, -hz - offsetZ},
                          {hx - offsetX, -hy - offsetY, -hz - offsetZ},
                          {hx - offsetX, hy - offsetY, -hz - offsetZ},
                          {-hx - offsetX, hy - offsetY, -hz - offsetZ},

                          {-hx - offsetX, -hy - offsetY, hz - offsetZ},
                          {hx - offsetX, -hy - offsetY, hz - offsetZ},
                          {hx - offsetX, hy - offsetY, hz - offsetZ},
                          {-hx - offsetX, hy - offsetY, hz - offsetZ}};

  float cp = cosf(rotation.pitch), sp = sinf(rotation.pitch);
  float cy = cosf(rotation.yaw), sy = sinf(rotation.yaw);
  float cr = cosf(rotation.roll), sr = sinf(rotation.roll);

  int px[8], py[8];

  for (int i = 0; i < 8; i++) {
    Apricot3DVector p = v[i];

    float y = p.y * cp - p.z * sp;
    float z = p.y * sp + p.z * cp;
    p.y = y;
    p.z = z;

    float x = p.x * cy + p.z * sy;
    z = -p.x * sy + p.z * cy;
    p.x = x;
    p.z = z;

    x = p.x * cr - p.y * sr;
    y = p.x * sr + p.y * cr;
    p.x = x;
    p.y = y;

    p.x += reference.x;
    p.y += reference.y;
    p.z += reference.z;

    px[i] = (int)p.x;
    py[i] = (int)p.y;
  }

  static const int edges[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0},
                                   {4, 5}, {5, 6}, {6, 7}, {7, 4},
                                   {0, 4}, {1, 5}, {2, 6}, {3, 7}};

  for (int i = 0; i < 12; i++) {
    int a = edges[i][0];
    int b = edges[i][1];
    apricot_draw_line(canvas, px[a], py[a], px[b], py[b], color);
  }
}

#endif /* APRICOT_IMPLEMENTATION */
