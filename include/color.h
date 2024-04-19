#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include "../deps/handmademath/HandmadeMath.h"

#include "./ray.h"

void write_color(FILE* file, HMM_Vec3* color) {
  HMM_Vec3 new_vec = HMM_MulV3F(*color, 255.999);
  fprintf(file, "%d %d %d\n", (int)new_vec.R, (int)new_vec.G, (int)new_vec.B);
}

HMM_Vec3 unit_dir(HMM_Vec3* vec) {
  return HMM_DivV3F(*vec, HMM_Len(*vec));
}

HMM_Vec3 ray_color(Ray* ray) {
  HMM_Vec3 test_sphere_center = { .X = 0, .Y = 0, .Z = -1 };
  float t = hit_sphere(&test_sphere_center, 0.5, ray);
  
  if(t > -1) {
    HMM_Vec3 N_vec = HMM_SubV3(ray_at(ray, t), test_sphere_center);
    HMM_Vec3 N = unit_dir(&N_vec);
    HMM_Vec3 color = { .R = N.X+1, .G = N.Y+1, .B = N.Z+1 };
    return HMM_MulV3F(color, 0.5);
  }

  HMM_Vec3 unit = unit_dir(&ray->dir);
  float a = 0.5*(unit.Y + 1);
  HMM_Vec3 b = { .X = 1.0, .Y = 1.0, .Z = 1 };
  HMM_Vec3 c = { .X = 0.5, .Y = 0.7, .Z = 1 };
  return HMM_AddV3(HMM_MulV3F(b, 1-a), HMM_MulV3F(c, a));
}

#endif
