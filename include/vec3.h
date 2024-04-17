#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct {
  double x, y, z;
} Vec3;

Vec3 add_vector(Vec3& a, Vec3& b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

Vec3 sub_vector(Vec3& a, Vec3& b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}

Vec3 mul_vector(Vec3& a, Vec3& b) {
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  return a;
}


Vec3 mul_vector(Vec3& a, double b) {
  a.x *= b;
  a.y *= b;
  a.z *= b;
  return a;
}

Vec3 div_vector(Vec3& a, double b) {
  return mul_vector(a, (1/b));
}

Vec3 cross_vector(Vec3& a, Vec3& b) {
  Vec3 res;
  res.x = (a.y * b.z) - (b.z * a.y);
  res.y = (a.z * b.x) - (b.x * a.z);
  res.z = (a.x * b.y) - (b.y * a.x);
  return res;
}

double dot_vector(Vec3& a, Vec3& b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

double len_sqr(Vec3 &a) { return a.x * a.x + a.y * a.y + a.z * a.z; }

double len(Vec3 &a) { return sqrt(len_sqr(a)); }

Vec3 unit_vector(Vec3 &v) { return div_vector(v, len(v)); }

#endif
