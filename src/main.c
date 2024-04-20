#include "../include/color.h"

int main() {
  FILE* file = fopen("./image.ppm", "w+");

  float aspect_ratio = 16.0f / 9.0f;
  int img_width = 400;

  int img_height = (int)(img_width / aspect_ratio);
  if(img_height < 1) {
    img_height = 1;
  }

  hitlist world;

  HMM_Vec3 c_1 = { .X = 0, .Y = 0, .Z = -1 };
  HMM_Vec3 c_2 = { .X = 0, .Y = -100.5, .Z = -1 };
  Sphere sph_1 = { .center = c_1, .radius = 0.5 };
  Sphere sph_2 = { .center = c_2, .radius = 100 };

  hittable h_1 = { .sphere = sph_1 };
  hittable h_2 = { .sphere = sph_2 };

  add_to_hitlist(&world, h_1);
  add_to_hitlist(&world, h_2);

  float focal_length = 1;
  float viewport_height = 2;
  float viewport_width = viewport_height * ((double)img_width / img_height);

  HMM_Vec3 camera_center = { .X = 0, .Y = 0, .Z = 0 };

  HMM_Vec3 viewport_u = { .X = viewport_width, .Y = 0, .Z = 0 };
  HMM_Vec3 viewport_v = { .X = 0, .Y = -viewport_height, .Z = 0 };

  HMM_Vec3 delta_u = HMM_DivV3F(viewport_u , img_width);
  HMM_Vec3 delta_v = HMM_DivV3F(viewport_v , img_height);

  HMM_Vec3 focal_vec = { .X = 0, .Y = 0, .Z = focal_length };

  HMM_Vec3 viewport_upper_left = HMM_SubV3(camera_center, HMM_AddV3(focal_vec, HMM_AddV3(HMM_DivV3F(viewport_u, 2), HMM_DivV3F(viewport_v, 2))));

  HMM_Vec3 pixel100_loc = HMM_AddV3(viewport_upper_left, HMM_MulV3F(HMM_AddV3(delta_u, delta_v), 0.5f));
  
  fprintf(file, "P3\n%d %d\n255\n", img_width, img_height);
  
  for(int i = 0; i < img_height; i++) {
    printf("\rScanlines remaining: %d\n", img_height-i);
    
    for(int j = 0; j < img_width; j++) {
      HMM_Vec3 pixel_center = HMM_AddV3(pixel100_loc, HMM_AddV3(HMM_MulV3F(delta_u, j), HMM_MulV3F(delta_v, i)));
      HMM_Vec3 ray_dir = HMM_SubV3(pixel_center, camera_center);

      Ray ray = { .dir = ray_dir, .origin = camera_center };

      HMM_Vec3 pixel_color = ray_color(&ray, &world);
      
      write_color(file, &pixel_color);
    }
  }

  printf("Done!");
  return 0;
}
