#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

Vec3 write_color(Vec3 &color) { return mul_vector(color, 255.999); }

#endif
