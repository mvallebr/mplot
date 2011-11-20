#include <fconv.h>
#include <string.h>

int fconv_read_cdwrite(FILE *f) {
  char buffer[1000];
  int i, num_read;
  int int1, int2, int3;
  int p1, p2, p3, p4;
  int num_elem, num_mat;
  float x, y;
  int actual;
  int col[10];
  num_nodes=0; num_elems=0;  
  //  elem *elemento;
  //First, read the header
  for (i=0; i<4; i++)
    fgets(buffer, sizeof(buffer), f);    

  //Read the nodes
  do {
    fgets(buffer, sizeof(buffer), f);        
    int1=0;
    x=0.0;
    y=0.0;
    num_read =sscanf(buffer, " %d %d %d %f %f ", &int1, &int2, &int3, &x, &y);
    //    printf("Node read: %d \tx=%f\ty=%f   \tnum_read=%d\n", int1, x, y, num_read);
    if (num_read>0) {
      actual=num_nodes;
      num_nodes++;
      nos[actual].x = x;
      nos[actual].y = y;
      nos[actual].num = int1;
    }
    //printf("no   %3d\t%f\t%f\t%f\n", int1, x, y, 0.0);
  } while((strncmp(buffer, "N", 1)!=0)&&(!feof(f))&&(num_read!=0)); 

  //Skip the propaganda
  for (i=0; i<2; i++)
    fgets(buffer, sizeof(buffer), f);    

  //Read the elements 
  do {
    fgets(buffer, sizeof(buffer), f);        
    int1=0;
    x=0.0;
    y=0.0;
    num_read =sscanf(buffer, " %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &num_mat, &(col[1]), &(col[2]), 
		     &num_elem, &(col[4]), &(col[5]), &(col[6]), &(col[7]), &(col[8]), &(col[9]), 
		      &p1, &p2, &p3, &p4);
    if (num_read==14) {
      actual=num_elems;
      num_elems++;
      elems[actual].num = num_elem;
      elems[actual].material = num_mat;
      elems[actual].p[0] = p1;
      elems[actual].p[1] = p2;
      elems[actual].p[2] = p3;
      elems[actual].p[3] = p4;
      elems[actual].has_cond = 0;
      elems[actual].cond = 0;
      elems[actual].type = -1;
      elems[actual].prop = 0;
      //printf("elem  mat=%d\tnum=%d\tp1=%d\tp2=%d\tp3=%d\tp4=%d\n", num_mat, num_elem, p1, p2, p3, p4);
    }
  } while((strncmp(buffer, "/GO", 3)!=0)&&(!feof(f))&&(num_read==14)); 

  return 1;
}
