#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdlib.h>

#include "./ray.h"

#include "../deps/handmademath/HandmadeMath.h"
#include "./interval.h"

float random_float() {
  return rand() / (RAND_MAX + 1.0);
}

float random_float_interval(float min, float max) {
  return min + (max-min)*random_float();
}

#endif
