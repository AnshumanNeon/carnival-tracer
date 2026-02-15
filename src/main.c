#include "../include/color.h"
#include "../include/common.h"

#ifdef GPU
#include <omp.h>
#endif

static inline float degree_to_rad(float deg) { return deg * HMM_PI / 180.0; }

static inline HMM_Vec3 random_in_unit_disk() {
  while(true) {
    HMM_Vec3 p = { .X = random_float_interval(-1, 1), .Y = random_float_interval(-1, 1), .Z = 0 };
    if(HMM_LenSqrV3(p) < 1) return p;
  }
}

HMM_Vec3 defocus_disk_sample(HMM_Vec3 camera_center, HMM_Vec3 defocus_u, HMM_Vec3 defocus_v) {
  HMM_Vec3 p = random_in_unit_disk();
  return HMM_AddV3(camera_center, HMM_AddV3(HMM_MulV3F(defocus_u, p.X), HMM_MulV3F(defocus_v, p.Y)));
}

int main() {
  FILE *file = fopen("./image.ppm", "w+");

  const float aspect_ratio = 16.0f / 9.0f;
  const int img_width = 720;

  int img_height = (int)(img_width / aspect_ratio);
  if (img_height < 1) {
    img_height = 1;
  }

  hitlist world;

  Sphere sph_1, sph_2, sph_3, sph_4;

  HMM_Vec3 c_1 = {.X = 0.0, .Y = -100.5, .Z = -1.0};
  HMM_Vec3 c_2 = {.X = 0.0, .Y = 0.0, .Z = -1.2};
  HMM_Vec3 c_3 = {.X = -2.0, .Y = 0.5, .Z = -2.0};
  HMM_Vec3 c_4 = {.X = 1.0, .Y = 0.0, .Z = -1.0};

  HMM_Vec3 gournd_color = {.R = 0.8, .G = 0.8, .B = 0.0};
  HMM_Vec3 center_color = {.R = 0.1, .G = 0.2, .B = 0.5};
  HMM_Vec3 left_color = {.R = 0.8, .G = 0.8, .B = 0.8};
  HMM_Vec3 right_color = {.R = 0.8, .G = 0.2, .B = 0.1};

  Lambertian mat_ground = {.albedo = gournd_color};
  Lambertian mat_center = {.albedo = center_color};
  Metal mat_left = {.albedo = left_color, .fuzz = 0.3};
  Metal mat_right = {.albedo = right_color, .fuzz = 1.0};

  sph_1 = init_sphere(&sph_1, &c_1, 100, &mat_ground, NULL, false);
  sph_2 = init_sphere(&sph_2, &c_2, 0.5, &mat_center, NULL, false);
  sph_3 = init_sphere(&sph_3, &c_3, 1.0, NULL, &mat_left, true);
  sph_4 = init_sphere(&sph_4, &c_4, 0.5, NULL, &mat_right, true);

  const hittable h_1 = {.sphere = sph_1};
  const hittable h_2 = {.sphere = sph_2};
  const hittable h_3 = {.sphere = sph_3};
  const hittable h_4 = {.sphere = sph_4};

  add_to_hitlist(&world, h_2);
  add_to_hitlist(&world, h_3);
  add_to_hitlist(&world, h_4);
  add_to_hitlist(&world, h_1);
  
  HMM_Vec3 lookfrom = { .X = 0, .Y = 0, .Z = 0 };
  HMM_Vec3 lookat = { .X= 0, .Y = 0, .Z = -1 };
  HMM_Vec3 vup = { .X= 0, .Y = 1, .Z = 0 };

  HMM_Vec3 u, v, w;

  const float defocus_angle = 2.0;
  const float focus_dist = 2;

  HMM_Vec3 defocus_disk_u, defocus_disk_v;

  float vfov = 90;
  float theta = degree_to_rad(vfov);
  float h = tan(theta / 2);
  const float viewport_height = 2 * h * focus_dist;

  const float viewport_width = viewport_height * ((double)img_width / img_height);
  
  w = HMM_NormV3(HMM_SubV3(lookfrom, lookat));
  u = HMM_NormV3(HMM_Cross(vup, w));
  v = HMM_Cross(w, u);

  const HMM_Vec3 camera_center = lookfrom;

  HMM_Vec3 viewport_u = HMM_MulV3F(u, viewport_width);
  HMM_Vec3 viewport_v = HMM_MulV3F(v, viewport_height * -1);

  HMM_Vec3 delta_u = HMM_DivV3F(viewport_u, img_width);
  HMM_Vec3 delta_v = HMM_DivV3F(viewport_v, img_height);

  HMM_Vec3 viewport_upper_left = HMM_SubV3(camera_center, HMM_AddV3(HMM_MulV3F(w, focus_dist), HMM_AddV3(HMM_DivV3F(viewport_u, 2), HMM_DivV3F(viewport_v, 2))));
  /* HMM_Vec3 focal_vec = {.X = 0, .Y = 0, .Z = focus_dist}; */

  HMM_Vec3 pixel100_loc = HMM_AddV3(viewport_upper_left, HMM_MulV3F(HMM_AddV3(delta_u, delta_v), 0.5f));

  float defocus_radius = focus_dist * tan(degree_to_rad(defocus_angle / 2));
  defocus_disk_u = HMM_MulV3F(u, defocus_radius);
  defocus_disk_v = HMM_MulV3F(v, defocus_radius);

  fprintf(file, "P3\n%d %d\n255\n", img_width, img_height);

  const int sample_per_pixel = 50;

  const float pixel_smaples_scale = 1.0 / sample_per_pixel;

  const int max_depth = 20;

#ifndef GPU
  for (int j = 0; j < img_height; j++) {
    printf("\rScanlines remaining: %d\n", img_height - j);

    for (int i = 0; i < img_width; i++) {
      HMM_Vec3 pixel_color = {.R = 0, .G = 0, .B = 0};

      for (int sample = 0; sample < sample_per_pixel; sample++) {
        HMM_Vec3 offset = { .X = random_float() - 0.5, .Y = random_float() - 0.5, .Z = 0};

        HMM_Vec3 pixel_sample = HMM_AddV3(pixel100_loc, HMM_AddV3(HMM_MulV3F(delta_u, i + offset.X), HMM_MulV3F(delta_v, j + offset.Y)));
        HMM_Vec3 ray_dir = HMM_SubV3(pixel_sample, camera_center);

        HMM_Vec3 ray_origin;

	if(defocus_angle <= 0) ray_origin = camera_center;
	else ray_origin = defocus_disk_sample(camera_center, defocus_disk_u, defocus_disk_v);

        Ray ray = {.dir = ray_dir, .origin = ray_origin};
        pixel_color = HMM_AddV3(pixel_color, ray_color(&ray, &world, max_depth));
      }

      HMM_Vec3 out_color = HMM_MulV3F(pixel_color, pixel_smaples_scale);
      write_color(file, &out_color);
    }
  }
#endif

  // actual calculations
#ifdef GPU
  /* omp_set_num_threads(4); */
#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < img_height; j++) {
    printf("\rScanlines remaining (gpu acceleration apparently): %d\n", img_height - j);

    for (int i = 0; i < img_width; i++) {
      HMM_Vec3 pixel_color = {.R = 0, .G = 0, .B = 0};

      for (int sample = 0; sample < sample_per_pixel; sample++) {
        HMM_Vec3 offset = { .X = random_float() - 0.5, .Y = random_float() - 0.5, .Z = 0 };

        HMM_Vec3 pixel_sample = HMM_AddV3(pixel100_loc, HMM_AddV3(HMM_MulV3F(delta_u, i + offset.X), HMM_MulV3F(delta_v, j + offset.Y)));
        HMM_Vec3 ray_dir = HMM_SubV3(pixel_sample, camera_center);

        HMM_Vec3 ray_origin;

	if(defocus_angle <= 0) ray_origin = camera_center;
	else ray_origin = defocus_disk_sample(camera_center, defocus_disk_u, defocus_disk_v);

        Ray ray = {.dir = ray_dir, .origin = ray_origin};
        pixel_color = HMM_AddV3(pixel_color, ray_color(&ray, &world, max_depth));
      }

      HMM_Vec3 out_color = HMM_MulV3F(pixel_color, pixel_smaples_scale);
      write_color(file, &out_color);
    }
  }
#endif

  printf("Done!");
  return 0;
}
