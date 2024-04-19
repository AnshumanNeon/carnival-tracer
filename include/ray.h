#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include "../deps/handmademath/HandmadeMath.h"

typedef struct Ray {
  HMM_Vec3 dir;
  HMM_Vec3 origin;
} Ray;

HMM_Vec3 ray_at(Ray* ray, float t) {
  return HMM_AddV3(ray->origin, HMM_MulV3F(ray->dir, t));
}

float hit_sphere(HMM_Vec3* center, float radius, Ray* ray) {
  HMM_Vec3 oc = HMM_SubV3(*center, ray->origin);
  float a = HMM_Len(ray->dir) * HMM_Len(ray->dir);
  float h = HMM_DotV3(ray->dir, oc);
  float oc_sqr = HMM_Len(oc) * HMM_Len(oc);
  float c = oc_sqr - (radius*radius);
  float discriminant = h*h - a*c;

  if(discriminant < 0) {
    return -1;
  } else {
    return (h - sqrt(discriminant)) / a;
  }
}

#endif
