#ifndef MAT_H
#define MAT_H

#include "./common.h"

typedef struct Material {
  HMM_Vec3 albedo;
} Material;

typedef struct Lambertian {
  HMM_Vec3 albedo;
} Lambertian;

#endif
