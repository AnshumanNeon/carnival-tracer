#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <handmademath/HandmadeMath.h>

typedef struct Ray {
  HMM_Vec3 dir;
  HMM_Vec3 origin;
} Ray;

HMM_Vec3 ray_at(Ray* ray, float t) {
  return HMM_AddV3(ray->origin, HMM_MulV3F(ray->dir, t));
}

#endif
