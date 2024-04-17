#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE* file = fopen("./image.ppm", "w+");

  int img_width = 256, img_height = 256;
  
  fprintf(file, "P3\n%d %d\n255\n", img_width, img_height);

  for(int i = 0; i < img_height; i++) {
    printf("\rScanlines remaining: %d\n", img_height-i);
    
    for(int j = 0; j < img_width; j++) {
      double r = (double)i / (img_width-1);
      double g = (double)j / (img_height-1);
      double b = 0;

      int ir = (int)(255.99 * r);
      int ig = (int)(255.999 * g);
      int ib = (int)(255.999 * b);

      fprintf(file, "%d %d %d\n", ir, ig, ib);
    }
  }

  printf("Done!");
  return 0;
}
