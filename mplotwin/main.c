#include "mplot.h"

#define NUMITEMS 1300

//#define SIMULATE_LABVIEW

#ifdef SIMULATE_LABVIEW

int main(int argc, char *argv[]){
  mplot *m;
  FIXED_NUMBER x[NUMITEMS], y[NUMITEMS], z[NUMITEMS], fex[NUMITEMS], fey[NUMITEMS];
  FIXED_NUMBER gridz[4*DEFAULT_CONTOUR_W*DEFAULT_CONTOUR_H];
  int i, kk;
  FILE *f;
  int num_items, num_items_2, number_read;
  m = mplot_open();
  if (!m) return -1;
  mplot_initgraph(m);
  f = fopen("./raul/data.dat", "r");
  if (!f) return -1;
  //Read data
  i=0;
  num_items=0;
  number_read=3;
  while((!feof(f)) && (number_read==3) ) {
    number_read=fscanf(f, " %f %f %f  \n", (float *)&(x[i]), (float *)&(y[i]), (float *)&(z[i]));////, &emp);
    //printf("%f\t\t%f\t\t%f\t%d\n", (float)x[i], (float)y[i], (float)z[i], number_read);
    i++;
    num_items++;
  }
  fclose(f);
  //////////////////////////////////////////////
  f = fopen("./raul/cd.dat", "r");
  if (!f) return -1;
  i=0;
  num_items_2=0;
  number_read=2;
  while((!feof(f))  && (number_read==2)) {
    number_read=fscanf(f, " %f %f \n", (float *)&(fex[i]), (float *)&(fey[i]));
    i++;
    num_items_2++;
  }
  fclose(f); 


//  mplot_change_contour_resolution(m, 32, 32);
  kk=0;
  while (1)    {
        if (kk==0) {
                mplot_gengrid(m, x, y, z, num_items, x, y, num_items, METHOD_BARNES,
                        DEFAULT_CONTOUR_W, DEFAULT_CONTOUR_H, gridz, MPLOT_AUTODETECT,
                                0, 0, 0, 0, 0, 0);
                                printf("---64\n");
        } else                                    {
                mplot_gengrid(m, x, y, z, num_items, x, y, num_items, METHOD_BARNES,
                        32, 32, gridz, MPLOT_AUTODETECT,
                                0, 0, 0, 0, 0, 0);
                                printf("---32\n");
                                }
        mplot_clear_screen( m);
        mplot_draw_colors(m, 10, GRAPH1_Y);
        mplot_show_shade(m, GRAPH1_X, GRAPH1_Y);
        mplot_refresh_screen(m);
        while( SDL_PollEvent( &(m->event) ) ){
            switch( m->event.type ){
            case SDL_KEYUP:
                 return 0;
                 break;
            case SDL_QUIT:
                 return 0;
                 break;
            default:
                    break;
            }
        }
        //mplot_change_contour_resolution(m, 32, 32);
        kk=1;
  }




  //Finish mplot
  mplot_close(m);
  return 0;
}



#endif //SIMULATE_LABVIEW

#ifndef SIMULATE_LABVIEW
//#define USEGEN
#define USEFILE


int main(int argc, char *argv[]){
  mplot *m;
  FIXED_NUMBER x[NUMITEMS], y[NUMITEMS], z[NUMITEMS], fex[NUMITEMS], fey[NUMITEMS];
//  int exp1, exp2;
  int i;
#ifdef USEFILE
  FILE *f, *f2;
  int num_items, num_items_2, number_read;
#endif //USEFILE
  m = mplot_new();
  if (!m) return -1;
  mplot_init(m);
  mplot_initgraph(m);
#ifdef USEGEN
  //Initialize data
  x[0]=-4;
  y[0]=-7;
  z[0]=5;
  for (i=1; i<NUMITEMS/2; i++) {
    x[i] = x[i-1] + 3;
    y[i] = y[i-1] + 1;
    z[i] = z[i-1] + 1.4;
    if (z[i] > 50) z[i]=5;
    //printf("%d %f\n" , i, z[i]);
  }
  x[NUMITEMS/2]=-4;
  y[NUMITEMS/2]=-7;
  z[NUMITEMS/2]=5;
  for (i=1+ (NUMITEMS/2); i<NUMITEMS; i++) {
    x[i] = x[i-1] + 1;
    y[i] = y[i-1] + 3;
    z[i] = z[i-1] + 1.4;
    if (z[i] > 50) z[i]=5;
    //printf("%d %f\n" , i, z[i]);
  }

  //Update screen while windows isn't closed
  do {
    //Generate data
   for (i=0; i<NUMITEMS; i++) {
     //x[i] = (x[i] + 0.001<0.01) ? x[i] + 0.001 : 0;
     //y[i] = (y[i] + 0.003<0.01) ? y[i] + 0.003 : 0;
     if ((z[i] +0.8)<50) 
       z[i]+=1.6;
     else
       z[i]=  0;
     //z[i] = ((z[i] + 0.003)<0.01) ? (z[i]+0.003) : 0;
      //      if (z[i] > 0.09) z[i]=0.09;;
    }    

   //Update the screen
   } while (mplot_update(m, x, y, z,NUMITEMS, x, y, NUMITEMS, METHOD_BOXCAR, 10, MPLOT_AUTODETECT, MPLOT_GRAPHTYPE_SHADE));
  
#endif //USEGEN

#ifdef USEFILE
  f = fopen("./raul/data.dat", "r");
  f2 = fopen("./raul/saida.txt", "w");
  if (!f) return -1;
  i=0;
  num_items=0;
  number_read=3;
  while((!feof(f)) && (number_read==3) ) {
    number_read=fscanf(f, " %f %f %f  \n", (float *)&(x[i]), (float *)&(y[i]), (float *)&(z[i]));////, &temp);
    //    z[i]=0;
    //    printf("%f\t\t%f\t\t%f\t%d\n", (float)x[i], (float)y[i], (float)z[i], number_read);
    //fprintf(f2, "%f\t\t%f\t\t%f\t%d\n", x[i], y[i], z[i], number_read);
    i++;
    num_items++;
  }
  fclose(f);
  //////////////////////////////////////////////
  /*
  f = fopen("./raul/cd.dat", "r");
  if (!f) return -1;
  i=0;
  num_items_2=0;
  number_read=2;
  while((!feof(f))  && (number_read==2)) {
    //number_read=fscanf(f, "%fd%d %fd%d \n", &(fex[i]), &exp1, &(fey[i]), &exp2);
    number_read=fscanf(f, " %f %f \n", (float *)&(fex[i]), (float *)&(fey[i]));
    //    z[i]=0;
//    fex[i]*=(FIXED_NUMBER)pow((double)10, (double)exp1);
//    fey[i]*=(FIXED_NUMBER)pow((double)10, (double)exp2);
    //printf("%f\t\t%f\t%d\n", (float) fex[i], (float) fey[i], number_read);
    //fprintf(f2, "%f\t\t%f\t%d\n", (float) fex[i], (float) fey[i], number_read);
    i++;
    num_items_2++;
  }
  fclose(f); 
  */
  //////////////////////////////////////////////

  while (mplot_update(m, x, y, z, num_items, x, y, num_items, METHOD_BARNES, 10, MPLOT_AUTODETECT, MPLOT_GRAPHTYPE_SHADE)); 
  //return -1;
  fclose(f2);
#endif //USEFILE

  //Finish mplot
  mplot_destroy(m);
  return 0;
}

#endif //SIMULATE_LABVIEW
