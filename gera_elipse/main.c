#include <stdio.h>
#include <math.h>

#define INIT (-0.2)
#define END  0.0
#define INC  0.01
int main (int argc, char *argv[]) {
  FILE *f;
  double x, y;
  int i;

  if (argc<2) {
    fprintf(stderr, "Usage: %s FILE_OUTPUT\n", argv[0]);
    return 1;
  }
  f = fopen(argv[1], "wb");
  if (!f) {
    fprintf(stderr, "Couldn't write file '%s'!\n", argv[1]);
    return 2;
  }
  printf("Writing output...");
  x = INIT;
  i=0;
  while(x<END) {
    i++;
    y= sqrt(0.13*0.13*(1 - (x*x/(0.2*0.2))) );
    fprintf(f, "K, %d, %f, %f\n", i, x+0.2, y);
    x+=INC;
  }

  printf(" Done.\n");
  
  fclose(f);
  return 0;
}









