#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdlib.h>

#include "./ray.h"

#include <handmademath/HandmadeMath.h>
#include "./interval.h"

float random_float() {
  return rand() / (RAND_MAX + 1.0);
}

float random_float_interval(float min, float max) {
  return min + (max-min)*random_float();
}

HMM_Vec3 random_vector() {
  HMM_Vec3 random = { .X = random_float(), .Y = random_float(), .Z = random_float() };
  return random;
}

HMM_Vec3 random_vector_interval(float min, float max) {
  HMM_Vec3 random = { .X = random_float_interval(min, max), .Y = random_float_interval(min, max), .Z = random_float_interval(min, max) };
  return random;
}

#endif
