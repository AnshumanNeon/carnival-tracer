#ifndef COLOR_H
#define COLOR_H

#include <math.h>
#include <stdio.h>
#include "./ray.h"
#include "./hit.h"
#include "./interval.h"
#include "common.h"

float linear_to_gamma(float linear) {
  if(linear > 0) return sqrt(linear);

  return 0;
}

void write_color(FILE* file, HMM_Vec3* color) {
  HMM_Vec3 new_vec = *color;

  new_vec.R = linear_to_gamma(new_vec.R);
  new_vec.G = linear_to_gamma(new_vec.G);
  new_vec.B = linear_to_gamma(new_vec.B);
  
  Interval intensity = { .min=0.000, .max=0.999 };
  new_vec.R = 256 * clamp(&intensity, new_vec.R);
  new_vec.G = 256 * clamp(&intensity, new_vec.G);
  new_vec.B = 256 * clamp(&intensity, new_vec.B);
  fprintf(file, "%d %d %d\n", (int)new_vec.R, (int)new_vec.G, (int)new_vec.B);
}

HMM_Vec3 unit_dir(HMM_Vec3 *vec) { return HMM_DivV3F(*vec, HMM_Len(*vec)); }

HMM_Vec3 random_in_unit_sphere() {
  while(true) {
    HMM_Vec3 p = random_vector_interval(-1, 1);
    float p_sqr = HMM_Len(p) * HMM_Len(p);

    if(p_sqr < 1) return p;
  }
}

HMM_Vec3 random_unit_vector() {
  HMM_Vec3 r = random_in_unit_sphere();

  return unit_dir(&r);
}

HMM_Vec3 random_on_hemisphere(HMM_Vec3* normal) {
  HMM_Vec3 vec = random_unit_vector();

  if(HMM_Dot(vec, *normal) > 0.0) return vec;
  else return HMM_MulV3F(vec, -1.0);
}

HMM_Vec3 ray_color(Ray* ray, hitlist* world, int depth) {
  if(depth <= 0) {
    HMM_Vec3 color = { .R = 0, .G = 0, .B = 0 };
    return color;
  }
  
  hit_record rec;
  Interval interval = { .min = 0.001, .max = INFINITY };
  
  if(hit_any_hittable(world, ray, &interval, &rec)) {
    HMM_Vec3 dir = HMM_AddV3(rec.normal, random_unit_vector());
    Ray ray = { .dir = dir, .origin = rec.position };

    return HMM_MulV3F(ray_color(&ray, world, depth - 1), 0.5);
  }

  HMM_Vec3 unit = unit_dir(&ray->dir);
  float a = 0.5*(unit.G + 1);
  HMM_Vec3 b = { .R = 1.0, .G = 1.0, .B = 1.0 };
  HMM_Vec3 c = { .R = 0.5, .G = 0.7, .B = 1.0 };
  return HMM_AddV3(HMM_MulV3F(b, 1.0-a), HMM_MulV3F(c, a));
}

#endif
