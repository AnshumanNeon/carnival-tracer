#ifndef INTERVAL_H
#define INTERVAL_H

#include <math.h>

typedef struct Interval {
  float min, max;
} Interval;

float interval_size(Interval* interval) {
  return interval->max - interval->min;
}

bool contains(Interval* interval, float x) {
  return x <= interval->max && x >= interval->min;
}

bool surrounds(Interval* interval, float x) {
  return x < interval->max && x > interval->min;
}

const Interval empty = { .min = INFINITY, .max = -INFINITY };
const Interval universe = { .min = -INFINITY, .max = INFINITY };

#endif
