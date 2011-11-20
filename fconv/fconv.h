#ifndef __FCONV_CONV_H__
#define __FCONV_CONV_H__

#include <hm.h>
#include <cdwrite.h>
#include <stdio.h>

#define NUM_ELETR 32

typedef struct _material {
  int num;
  int type;
  double cond1, cond2;
  double prop[4];
} material;
int num_materials;
material materials[1000];

typedef struct _no {
  double x, y;
  int num;
} no;
int num_nodes;
no nos[100000];    //List of: nodes

typedef struct _elem {
  int num;  //number of element
  int material;       //material of element
  int p[4]; //nodes
  int has_cond;
  double cond;
  int type;
  double prop;
} elem;
int num_elems;
elem elems[100000];  //elements

double xcenter, ycenter; //Coordinates of the central position
double celemx, celemy;   //Coordinates of the central position of elements of one material 
double xmin, xmax, ymin, ymax;

int    fconv_num_columns;
float fconv_cx[100000], fconv_cy[100000], fconv_cz[100000];

int num_cut_nodes; //Nodes on border of mesh
int cut_nodes[10000];
double dist_beetwen_eletr;
int num_eletr;
int eletr[10000];

double expr_data[10240]; //Experimental data
int num_expr_data;

int  fconv_scale_nos      (double xscale, double yscale);
int  fconv_detect_center   (void);
int  fconv_detect_center_elem   (int material);
int  fconv_write_hm       (FILE *f, int diam, double current, int comment);
int  fconv_read_cdwrite    (FILE *f);
void fconv_init          (void);
int  fconv_detect_cut  ();
int  fconv_write_cut_file  (FILE *f);
int  fconv_write_cdread  (FILE *f, double current, double voltage, int diam, int skip);
int  fconv_write_cdread_path  (FILE *f);
int  fconv_read_hm  (FILE *f);
int  fconv_read_dens  (FILE *f);
no  *find_node(int number);
elem *find_elem(int number);
material *find_material(int number);
int  fconv_fill_columns_by_elements();
int fconv_elem_center(elem e, double *x, double *y);
int fconv_add_extra_node();
int fconv_read_exprfile(FILE *f);
int fconv_last_node_num();
int fconv_last_elem_num();

#endif //__FCONV_CONV_H__











