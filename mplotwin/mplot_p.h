//This file has private declarations

#ifndef __MPLOT_MPLOT_P_H__
#define __MPLOT_MPLOT_P_H__

#include <SDL.h>     // All SDL App's need this 
#include <SDL_image.h>
#include <SFont.h>
//#include <SDL_ttf.h> //Needed to have fonts
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL_endian.h> /* Used for the endian-dependent 24 bpp mode */
#include <mplot_c.h>

typedef struct _contour_curve {
  int x1, y1;
  int x2, y2;
  Uint8 ignore;
  Uint8 color;
} contour_curve;

typedef struct _mplot {
  //SDL variables need by graphics
  //TTF_Font *font;        //True type font to have text
  SDL_Surface *font;     //SFont
  SDL_Surface *text;     //Pointer to font surface
  SDL_Rect dstrect;      //Rect where the font will be draw
  SDL_Surface *screen;   //Pointer to screen
  SDL_Surface *image;    //Memory image
  SDL_Event event;       //To receive events from SDL
  SDL_Color colors[256]; //Palette
  Uint8 *bufp, *bufpf;   //Pointers to screen regions
  //  int z[1000];
  //  FIXED_NUMBER **gridzm;
  FIXED_NUMBER *gridz;
  FIXED_NUMBER *gridz2;
  FIXED_NUMBER *dist;
  FIXED_NUMBER topx, topy, topz;
  FIXED_NUMBER botx, boty, botz;
  FIXED_NUMBER *xmatrix;
  FIXED_NUMBER *ymatrix;
  FIXED_NUMBER w[1000]; //weights for the barnes method

  //Variables to contour plot
  SDL_Surface *contour_screen;   //Pointer to screen where contour will be made
  contour_curve *curve; //Curve to be draw
  //Grid of boolean to detect which values are within contour values
  Uint8 *contour_grid;
  //Grid of boolean to detect which contour values are changing from one level to another
  Uint8 *contour_grid2;
  int num_curves;
  int numlevels;
  double root_of_2;
  int graph_width, graph_height;
  
  int num_div_x1, num_div_y1;  //Big divisions of axes
  int num_div_x2, num_div_y2;  //Small divisions of axes
  char title[100]; //Top
  char httitle1[100], httitle2[100]; //horizontal top left and right
  char hbtitle1[100], hbtitle2[100]; //horizontal bottom left and right
  char vltitle[100], vmtitle[100], vrtitle[100]; //vertical left, middle and right
  int autoclear; //if set, clear screen on update
  int autorefresh; //if set, refresh screen on update

  int graph1_x, graph1_y, graph2_x, graph2_y;
  int contour_w, contour_h;

  //To detect and draw the finite elements circuit
  FIXED_NUMBER tangent[NUM_TANGENTS];
  FIXED_NUMBER fex[NUM_ANGLES], fey[NUM_ANGLES], fedistance[NUM_ANGLES];
  int fedetected[NUM_ANGLES];  //Indicates the angles that have values detected
  FIXED_NUMBER xm, ym;
  //used to cut the part of graph we don't want
  int cut_graph[2*GRAPH_HEIGHT];
  int *cut_contour;
} mplot;


////////////////////////////////////////////////////////////////
//Private functions
// Initialize SDL 
int  mplot_sdl_init   (mplot *m);
//Calculate the grid using the nearest point method
//int mplot_gengrid_nearest(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *z, int num_items);
//Used by gengrid_barnes
int mplot_create_grid_barnes (mplot *m, FIXED_NUMBER *x, 
			  FIXED_NUMBER *y, FIXED_NUMBER *z, 
			  FIXED_NUMBER *w, 
			  int num_items, 
			  double xr,    double yr,
			  double gamma, unsigned int iter
			  );
//Used by create_grid_barnes
double mplot_interpolate_barnes (double xx, double yy, double zz, 
			  int skip,  unsigned int n_k, 
			  FIXED_NUMBER *x, FIXED_NUMBER *y,
			  FIXED_NUMBER *z, FIXED_NUMBER *weight,
			  FIXED_NUMBER *z_last, 
			  double xr,  double yr
			  );
//Used by barnes method functions
int mplot_value_i_j(mplot *m, unsigned int ii, unsigned int jj, double xx, double yy, double *value);
//Used by barnes method functions
int mplot_nearest(mplot *mp, double x, double g[], unsigned int ng, int *b, double *f);

//Box method
int mplot_create_grid_box(  mplot *m, 
			  double xr, 
			  double yr,
			  FIXED_NUMBER *x, FIXED_NUMBER *y, 
			  FIXED_NUMBER *z, int num_items);
int
mplot_gr_grid_box( mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *f,
		   int num_items, double x0, double y0, double xr, double yr,
		   //int method,
		   unsigned int neighbors, int enlargements, double *fOut);

//Functions used to draw the line contour curve (not shade contour)
int mplot_add_curve(mplot *m, int x1, int y1, int x2, int y2, Uint8 color);
int mplot_init_curve(mplot *m);
int mplot_draw_curve(mplot *m, int sx, int sy);

#endif // __MPLOT_MPLOT_P_H__
