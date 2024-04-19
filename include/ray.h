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

bool hit_sphere(HMM_Vec3* center, float radius, Ray* ray) {
  HMM_Vec3 oc = HMM_SubV3(*center, ray->origin);
  float a = HMM_DotV3(ray->dir, ray->dir);
  float b = -2.0 * HMM_DotV3(ray->dir, oc);
  float c = HMM_DotV3(oc, oc) - (radius*radius);
  float discriminant = b*b - 4*a*c;

  return (discriminant >= 0);
}

#endif
