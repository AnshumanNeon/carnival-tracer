#ifndef HIT_H
#define HIT_H
#include <stdio.h>
#include "./common.h"
#include "interval.h"
#include "./material.h"

typedef struct hit_record {
  HMM_Vec3 position;
  HMM_Vec3 normal;
  float t;
  bool front_face;
  Lambertian* mat;
  Metal* metal;
  bool is_metal;
} hit_record;

typedef struct Sphere {
  HMM_Vec3 center;
  float radius;
  Lambertian* mat;
  Metal* metal;
  bool is_metal;
} Sphere;

typedef union hittable {
  Sphere sphere;
} hittable;

typedef struct hitlist {
  hittable hittables[100];
  int length;
} hitlist;

HMM_Vec3 unit_dir(HMM_Vec3 *vec) { return HMM_DivV3F(*vec, HMM_Len(*vec)); }

HMM_Vec3 vec_reflect(HMM_Vec3* vec, HMM_Vec3* n) {
  HMM_Vec3 x = HMM_MulV3F(*n, 2 * HMM_Dot(*vec, *n));

  return HMM_SubV3(*vec, x);
}

HMM_Vec3 random_in_unit_sphere() {
  while(true) {
    HMM_Vec3 p = random_vector_interval(-1, 1);
    float p_sqr = HMM_Len(p) * HMM_Len(p);

    if(p_sqr < 1) return p;
  }
}

HMM_Vec3 random_unit_vector() {
  HMM_Vec3 r = random_in_unit_sphere();

  return unit_dir(&r);
}

void add_to_hitlist(hitlist* list, hittable obj) {
  list->hittables[list->length] = obj;
  list->length++;
}

bool set_face_normal(Ray* r, HMM_Vec3* outward_normal) {
  return HMM_DotV3(r->dir, *outward_normal) < 0;
}

bool near_zero(HMM_Vec3* vector) {
  double s = 1e-8;

  return (fabsf(vector->X) < s) && (fabsf(vector->Y) < s) && (fabsf(vector->Z) < s);
}

Sphere init_sphere(Sphere* sphere, HMM_Vec3* center, float radius, Lambertian* mat, Metal* metal, bool is_metal) {
  sphere->center = *center;
  sphere->radius = fmax(0, radius);
  sphere->mat = mat;
  sphere->metal = metal;
  sphere->is_metal = is_metal;

  return *sphere;
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

  record->is_metal = sphere->is_metal;

  if(record->is_metal) {
    record->metal = sphere->metal;
  } else {
    record->mat = sphere->mat;
  }
  
  return true;
}

bool hit_any_hittable(hitlist* list, Ray* ray, Interval* interval, hit_record* record) {
  hit_record temp_record;

  float closet_yet = interval->max;

  for(int i = 0; i < list->length; i++) {
    Interval new_interval = { .min = interval->min, .max = closet_yet };
    if(hit_sphere(&list->hittables[i].sphere, ray, &new_interval, &temp_record)) {
      closet_yet = temp_record.t;
      *record = temp_record;

      return true;
    }
  }

  return false;
}

bool scatter_metal(Metal* metal, Ray* ray, hit_record* rec, HMM_Vec3* color, Ray* scattered_ray) {
  HMM_Vec3 reflected = vec_reflect(&ray->dir, &rec->normal);
  reflected = HMM_AddV3(unit_dir(&reflected), HMM_MulV3F(random_unit_vector(), metal->fuzz));
  
  Ray r = { .dir = reflected, .origin = rec->position };

  *scattered_ray = r;
  *color = metal->albedo;
  
  return (HMM_Dot(scattered_ray->dir, rec->normal) > 0);
}

bool scatter_lambertian(Lambertian* lambertian, Ray* ray, hit_record* rec, HMM_Vec3* attenuation, Ray* scattered_ray) {
  HMM_Vec3 scatter_dir = HMM_AddV3(rec->normal, random_unit_vector());

  if(near_zero(&scatter_dir)) {
    scatter_dir = rec->normal;
  }

  Ray new_ray = { .origin = rec->position, .dir = scatter_dir };

  *scattered_ray = new_ray;
  *attenuation = lambertian->albedo;

  return true;
}

#endif
