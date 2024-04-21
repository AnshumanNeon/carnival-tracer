#ifndef HIT_H
#define HIT_H

#include "./common.h"
#include "interval.h"

typedef struct hit_record {
  HMM_Vec3 position;
  HMM_Vec3 normal;
  float t;
  bool front_face;
} hit_record;

typedef struct Sphere {
  HMM_Vec3 center;
  float radius;
} Sphere;

typedef union hittable {
  Sphere sphere;
} hittable;

typedef struct hitlist {
  hittable hittables[100];
  int length;
} hitlist;

void add_to_hitlist(hitlist* list, hittable obj) {
  list->hittables[list->length] = obj;
  list->length++;
}

bool set_face_normal(Ray* r, HMM_Vec3* outward_normal) {
  return HMM_DotV3(r->dir, *outward_normal) < 0;
}

bool hit_sphere(Sphere* sphere, Ray* ray, Interval* interval, hit_record* record) {
  HMM_Vec3 oc = HMM_SubV3(sphere->center, ray->origin);
  float a = HMM_Len(ray->dir) * HMM_Len(ray->dir);
  float h = HMM_DotV3(oc, ray->dir);
  float oc_sqr = HMM_Len(oc) * HMM_Len(oc);
  float c = oc_sqr - (sphere->radius * sphere->radius);

  float discriminant = h*h - a*c;

  if(discriminant < 0) {
    return false;
  }

  float sqrtd = sqrt(discriminant);
  float root = (h - sqrtd) / a;

  if(!surrounds(interval, root)) {
    root = (h + sqrtd) / a;

    if(!surrounds(interval, root)) {
      return false;
    }
  }

  record->t = root;
  record->position = ray_at(ray, record->t);
  HMM_Vec3 normal = HMM_DivV3F(HMM_SubV3(record->position, sphere->center), sphere->radius);

  record->normal = normal;
  
  return true;
}

bool hit_any_hittable(hitlist* list, Ray* ray, Interval* interval, hit_record* record) {
  hit_record temp_record;
  bool hit_anything = false;

  float closet_yet = interval->max;

  for(int i = 0; i < list->length; i++) {
    Interval new_interval = { .min = interval->min, .max = closet_yet };
    if(hit_sphere(&list->hittables[i].sphere, ray, &new_interval, &temp_record)) {
      hit_anything = true;
      closet_yet = temp_record.t;
      *record = temp_record;
    }
  }

  return hit_anything;
}

#endif
