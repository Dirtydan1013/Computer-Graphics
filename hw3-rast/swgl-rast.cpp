#include "swgl.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>

float *swdepthbuffer = NULL;
int swbuffer_width = 0, swbuffer_height = 0;

void writepixel(int x, int y, float z, GLdouble r, GLdouble g, GLdouble b)
{
    if (x < 0 || x >= swbuffer_width || y < 0 || y >= swbuffer_height) return;
    int idx = y * swbuffer_width + x;
    if (z < swdepthbuffer[idx]) {
        swdepthbuffer[idx] = z;
        GLubyte map[1] = {255};
        glColor3d(r, g, b);
        glRasterPos2i(x, y);
        glBitmap(1, 1, 0, 0, 0, 0, map);
    }
}

bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1,
                   GLdouble x2, GLdouble y2, GLdouble z2,
                   GLdouble r, GLdouble g, GLdouble b)
{
    int x0 = static_cast<int>(x1);
    int y0 = static_cast<int>(y1);
    int x1i = static_cast<int>(x2);
    int y1i = static_cast<int>(y2);

    int dx = std::abs(x1i - x0), sx = x0 < x1i ? 1 : -1;
    int dy = -std::abs(y1i - y0), sy = y0 < y1i ? 1 : -1;
    int err = dx + dy, e2;

    int steps = std::max(dx, -dy);
    for (int i = 0; ; i++) {
        float t = (steps == 0) ? 0.0f : (float)i / steps;
        float z = z1 * (1 - t) + z2 * t;
        writepixel(x0, y0, z, r, g, b);
        if (x0 == x1i && y0 == y1i) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }

    return true;
}

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
                GLdouble x2, GLdouble y2, GLdouble z2,
                GLdouble x3, GLdouble y3, GLdouble z3,
                GLdouble r, GLdouble g, GLdouble b)
{
    writepixel(static_cast<int>(x1), static_cast<int>(y1), static_cast<float>(z1), r, g, b);
    writepixel(static_cast<int>(x2), static_cast<int>(y2), static_cast<float>(z2), r, g, b);
    writepixel(static_cast<int>(x3), static_cast<int>(y3), static_cast<float>(z3), r, g, b);
    
    BresenhamLine(x1, y1, z1, x2, y2, z2, r, g, b);
    BresenhamLine(x2, y2, z2, x3, y3, z3, r, g, b);
    BresenhamLine(x3, y3, z3, x1, y1, z1, r, g, b);

    return true;
}

bool swInitZbuffer(int width, int height)
{
    if (swdepthbuffer) {
        delete[] swdepthbuffer;
    }
    swdepthbuffer = new float[width * height];
    swbuffer_width = width;
    swbuffer_height = height;
    std::fill(swdepthbuffer, swdepthbuffer + width * height, 1.0f);
    return true;
}

bool swClearZbuffer()
{
    if (!swdepthbuffer) return false;
    std::fill(swdepthbuffer, swdepthbuffer + swbuffer_width * swbuffer_height, 1.0f);
    return true;
}
