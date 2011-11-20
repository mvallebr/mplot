#ifndef __MPLOT_MPLOT_H__
#define __MPLOT_MPLOT_H__

#include <mplot_p.h>

enum {
  METHOD_BARNES=0,
  METHOD_BOXCAR
};

////////////////////////////////////////////////////////////////
//Public functions
/* Allocate memory */
extern mplot *mplot_new(void);
/* Initialize */
extern int  mplot_init   (mplot *m);
/* finish and free memory  */
extern void mplot_destroy(mplot *m);
/* update screen and return 1 on normal operation or 0 when window is closed */
extern int  mplot_update (
		   mplot *m,        //Pointer to mplot struct 
		   FIXED_NUMBER *cx, //x column
		   FIXED_NUMBER *cy, //y column
		   FIXED_NUMBER *cz, //z column
		   int num_items,   //number of points (x, y, z)
		   FIXED_NUMBER *cfex, //finite element mesh x column
		   FIXED_NUMBER *cfey, //finite element mesh y column
		   int fenum_items, //number of finite element mesh points (x, y)
		   int method,      //Method to be used
		   int numlevels,   //Number of contour levels
		   int autodetect,  //Autodetect top and bootom values (1) or use it
		                    //from mplot->top# mplot->bot# (0)
		   int graph_type   //Shading or contour?
		   );
int mplot_draw_text(mplot *m, char *text, int x, int y, int color);
int mplot_set_values(mplot *m, 
		     FIXED_NUMBER botx, FIXED_NUMBER topx, 
		     FIXED_NUMBER boty, FIXED_NUMBER topy, 
		     FIXED_NUMBER botz, FIXED_NUMBER topz);
int mplot_draw_axes(mplot *m); //Draw the axes on the graphs
int mplot_draw_colors(mplot *m, int sx, int sy);
int mplot_set_axe_div(mplot *m, int num_div_x1, int num_div_y1, //Big
		      int num_div_x2, int num_div_y2);        //Small
int mplot_set_titles(mplot *m, char *title, 
		     char *httitle1, char *httitle2,
		     char *hbtitle1, char *hbtitle2,
		     char *vltitle, char *vmtitle, char *vrtitle);
int mplot_clear_screen(mplot *m);
int mplot_refresh_screen(mplot *m);
int mplot_set_autoclear(mplot *m, int autoclear); //if 1, auto clear screen on update
int mplot_set_autorefresh(mplot *m, int autorefresh);//if 1, auto refresh screen on update
//Auto detect op and bottom values fro x, y and z
int mplot_autodetect_values(mplot *m, FIXED_NUMBER *x, 
			    FIXED_NUMBER *y, FIXED_NUMBER *z, 
			    int num_items,
			    FIXED_NUMBER *fex, FIXED_NUMBER *fey, 
			    int num_items_2
			    );
//Draw a line using bresenham algorithm
int mplot_draw_line(mplot *m, int x1, int y1, int x2, int y2, Uint8 color);
int mplot_draw_box (mplot *m, int x1, int y1, int x2, int y2, Uint8 color);
int mplot_draw_titles (mplot *m);

//////////////////////////////////
//Calculate the grid using the barnes point method 
int mplot_gengrid_barnes(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *z, int num_items); 
//Box method
int mplot_gengrid_box(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, 
		      FIXED_NUMBER *z, int num_items);
//Make and show contour curve on (sx, sy) screen coord -requires grid generated
int mplot_do_contour(mplot *m, int num_levels, int sx, int sy);
//Make and show shade graph on (sx, sy) screen coord -requires grid generated
int mplot_show_shade(mplot *m, int sx, int sy); //Show Shade Graph
//Detect the finite circuit
int mplot_detect_finit(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, int num_items);
//Draw the finite circuit
int mplot_draw_finit(mplot *m, int sx, int sy, int color);
//Cut the graph
int mplot_detect_cut_graph(mplot *m);
//Erase the cut part on the graph
int mplot_draw_cut_graph(mplot *m, int sx, int sy);
void mplot_change_contour_resolution(mplot *m, int newresx, int newresy);
//////////////////////////////////
//Functions to be used by labview
int mplot_initgraph (mplot *m);
int mplot_closegraph(mplot *m);
mplot * mplot_open(void);
int mplot_close(mplot *m);
int mplot_gengrid(mplot *m, FIXED_NUMBER *cx, FIXED_NUMBER *cy, FIXED_NUMBER *cz,
                        int num_items, FIXED_NUMBER *cfex, FIXED_NUMBER *cfey,
                        int fenum_items, int method,
                        int resx, int resy,
                        FIXED_NUMBER *gridz, //Alredy alocated array
                        int autodetect_ranges,
                        FIXED_NUMBER botx, FIXED_NUMBER topx,
                        FIXED_NUMBER boty, FIXED_NUMBER topy,
                        FIXED_NUMBER botz, FIXED_NUMBER topz
                        );
int mplot_detect_graph_size(mplot *m);
int mplot_show_dots(mplot *m, int sx, int sy, FIXED_NUMBER *cx, FIXED_NUMBER *cy, FIXED_NUMBER *cz, int num_items);
#endif // __MPLOT_MPLOT_H__
