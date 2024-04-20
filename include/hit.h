#ifndef HIT_H
#define HIT_H

#include "./common.h"

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

void add_to_hitlist(hitlist* list, hittable* obj) {
  list->hittables[list->length] = *obj;
  list->length++;
}

bool set_face_normal(Ray* ray, HMM_Vec3* outward_normal) {
  return HMM_Dot(ray->dir, *outward_normal) < 0;
}

bool hit_sphere(Sphere* sphere, Ray* ray, float t_min, float t_max, hit_record* record) {
  HMM_Vec3 oc = HMM_SubV3(sphere->center, ray->origin);
  float a = HMM_Len(ray->dir) * HMM_Len(ray->dir);
  float h = HMM_DotV3(ray->dir, oc);
  float oc_sqr = HMM_Len(oc) * HMM_Len(oc);
  float c = oc_sqr - (sphere->radius * sphere->radius);

  float discriminant = h*h - a*c;

  if(discriminant < 0) {
    return false;
  }

  float sqrtd = sqrt(discriminant);
  float root = (h - sqrtd) / a;

  if(root <= t_min || t_max <= root) {
    root = (h + sqrtd) / a;

    if(root <= t_min || t_max <= root) {
      return false;
    }
  }

  record->t = root;
  record->position = ray_at(ray, record->t);
  HMM_Vec3 normal = HMM_DivV3F(HMM_SubV3(record->position, sphere->center), sphere->radius);

  if(set_face_normal(ray, &normal)) {
    record->normal = normal;
  } else {
    record->normal = -normal;
  }

  return true;
}

bool hit_any_hittable(hitlist* list, Ray* ray, float t_min, float t_max, hit_record* record) {
  hit_record temp_record;
  bool hit_anything = false;

  float closet_yet = t_max;

  for(int i = 0; i < list->length; i++) {
    if(hit_sphere(&list->hittables[i].sphere, ray, t_min, t_max, record)) {
      hit_anything = true;
      closet_yet = temp_record.t;
      *record = temp_record;
    }
  }

  return hit_anything;
}

#endif
