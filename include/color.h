#ifndef COLOR_H
#define COLOR_H

#include <math.h>
#include <stdio.h>
#include "./ray.h"
#include "./hit.h"
#include "interval.h"

void write_color(FILE* file, HMM_Vec3* color) {
  HMM_Vec3 new_vec = HMM_MulV3F(*color, 255.999);
  fprintf(file, "%d %d %d\n", (int)new_vec.R, (int)new_vec.G, (int)new_vec.B);
}

HMM_Vec3 unit_dir(HMM_Vec3* vec) {
  return HMM_DivV3F(*vec, HMM_Len(*vec));
}

HMM_Vec3 ray_color(Ray* ray, hitlist* world) {
  hit_record rec;
  Interval interval = { .min = 0, .max = INFINITY };
  if(hit_any_hittable(world, ray, &interval, &rec)) {
    HMM_Vec3 color = { .R = 1, .G = 1, .B = 1 };
    HMM_Vec3 a = HMM_AddV3(unit_dir(&rec.normal), color);
    return HMM_MulV3F(a, 0.5);
  }

  HMM_Vec3 unit = unit_dir(&ray->dir);
  float a = 0.5*(unit.Y + 1);
  HMM_Vec3 b = { .X = 1.0, .Y = 1.0, .Z = 1.0 };
  HMM_Vec3 c = { .X = 0.2, .Y = 0.8, .Z = 1.0 };
  return HMM_AddV3(HMM_MulV3F(b, 1.0-a), HMM_MulV3F(c, a));
}

#endif
