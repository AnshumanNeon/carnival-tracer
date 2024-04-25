#ifndef MAT_H
#define MAT_H

#include "./common.h"

typedef struct Metal {
  HMM_Vec3 albedo;
} Metal;

typedef struct Lambertian {
  HMM_Vec3 albedo;
} Lambertian;

#endif
