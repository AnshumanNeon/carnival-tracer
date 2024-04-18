#include "../include/color.h"

int main() {
  FILE* file = fopen("./image.ppm", "w+");

  int img_width = 256, img_height = 256;
  
  fprintf(file, "P3\n%d %d\n255\n", img_width, img_height);

  for(int i = 0; i < img_height; i++) {
    printf("\rScanlines remaining: %d\n", img_height-i);
    HMM_Vec3 pixel_color = {.R = 0, (double)i / (img_height - 1), 0};
    
    for(int j = 0; j < img_width; j++) {
      pixel_color.R = (double)j / (img_width - 1);
      
      write_color(file, &pixel_color);
    }
  }

  printf("Done!");
  return 0;
}
