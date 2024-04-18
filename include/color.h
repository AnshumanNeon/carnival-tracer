#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include "../deps/handmademath/HandmadeMath.h"

#include "vec3.h"

void write_color(FILE* file, HMM_Vec3* color) {
  HMM_Vec3 new_vec = HMM_MulV3F(*color, 255.999);
  fprintf(file, "%d %d %d\n", (int)new_vec.R, (int)new_vec.G, (int)new_vec.B);
}

#endif
