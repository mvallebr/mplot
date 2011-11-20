//This file has declarations of constants
#ifndef __MPLOT_MPLOT_C_H__
#define __MPLOT_MPLOT_C_H__

#define DEBUG
//#define DEBUG2
//#define DEBUG3
//#define DEBUG4

//Public definitions and functions
#define NUM_ANGLES     16   //Should be multipe of 4
#define NUM_TANGENTS   (NUM_ANGLES/2)
#define PI             3.1415927

#define MPLOT_GRAPHTYPE_SHADE    0
#define MPLOT_GRAPHTYPE_CONTOUR  1
#define MPLOT_NODETECT    0
#define MPLOT_AUTODETECT  1

#define DEFAULT_NUM_LEVELS 10
#define SCREEN_W 800
#define SCREEN_H 500
#define COLORS_WIDTH  20
#define COLORS_OFFSET 15
#define AXE_SIZE_BIG 10
#define AXE_SIZE_SMALL 5
#define AXE_MASK "%.2fmm"
#define GRAPH_WIDTH  256
#define GRAPH_HEIGHT 300
#define LINES_BETWEEN_GRAPHS 35
#define DEFAULT_SMALL_AXE_DIV 10
#define DEFAULT_BIG_AXE_DIV   5
#define GRAPH_W (m->graph_width)
#define GRAPH_H (m->graph_height)
#define LINES_TO_SKIP_ON_START (SCREEN_H-GRAPH_HEIGHT)/2
#define GRAPH_OFFSET  (SCREEN_W-GRAPH_W)
#define GRAPH_INIT_LINE_OFFSET ((LINES_TO_SKIP_ON_START*SCREEN_W))
#define GRAPH1_X (3*COLORS_OFFSET +3*COLORS_WIDTH+4*AXE_SIZE_BIG)
#define GRAPH1_Y (LINES_TO_SKIP_ON_START)
#define GRAPH2_X (GRAPH1_X+GRAPH_WIDTH+LINES_BETWEEN_GRAPHS)
#define GRAPH2_Y (LINES_TO_SKIP_ON_START)
#define BOXCOLOR 1
#define BACKCOLOR 0
#define TITLECOLOR 2
#define LABELCOLOR 3
#define AXE1COLOR 4
#define AXE2COLOR 5
#define COLOR_FINIT 0
#define FONT_SIZE     14
#define DEFAULT_TITLE "Distribution of electric impedance"
#define DEFAULT_HTTITLE1 "h t left"
#define DEFAULT_HTTITLE2 "h t right"
#define DEFAULT_HBTITLE1 "h b left"
#define DEFAULT_HBTITLE2 "h b right"
#define DEFAULT_VLTITLE "v left"
#define DEFAULT_VMTITLE ""
#define DEFAULT_VRTITLE "v right"

#define DEFAULT_CONTOUR_W 64 //128
#define DEFAULT_CONTOUR_H 64 //128
#define CONTOUR_W (m->contour_w)
#define CONTOUR_H (m->contour_h)
#define CONTOUR_TOP  20
#define CONTOUR_BOT  10
#define CONTOUR_RANGE  (CONTOUR_TOP-CONTOUR_BOT)
//#define CONTOUR_GRID 5
#define FIXED_NUMBER float
#define MAX_ITEMS 1000


#endif // __MPLOT_MPLOT_C_H__
