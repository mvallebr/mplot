/*
    MPLOT - A library using SDL to do shade and contour plots
    Copyright (C) 1999  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Marcelo Elias Del Valle
    marceloelias@iname.com
*/

#include "mplot.h"
#include <stdio.h>

mplot *mplot_new (void) {
  mplot *m;
  
  /* sllocate  mplot structure */
  m = (mplot *) malloc(sizeof(mplot));   

  return m;
}

int mplot_sdl_init(mplot *m) {
  int j;

  /* Initialize the SDL library */
  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr,
  	    "Couldn't initialize SDL: %s\n", SDL_GetError());
    return 0;
  }
  /* Clean up on exit */
  atexit(SDL_Quit);
  /* Initialize the display */
  m->screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 8, 
			       //SDL_SWSURFACE);
  			       SDL_HWPALETTE);
  if ( m->screen == NULL ) {
    fprintf(stderr, "Couldn't set video mode: %s\n",
  	    SDL_GetError());
    return 0;
  }
  /* Create memory image */
  m->image = SDL_CreateRGBSurface(SDL_HWPALETTE, (m->contour_w), (m->contour_h), 8,
				  0,0,0,0);
  if ( m->screen == NULL ) {
    fprintf(stderr, "Couldn't create memory image (backbuffer): %s\n",
  	    SDL_GetError());
    return 0;
  }
  SDL_WM_SetCaption("MPLOT", "MPLOT");
  
    m->colors[0].r = 0;
    m->colors[0].g = 0;
    m->colors[0].b = 0;
  /* Set palette */
/*  for (i=1; i<=5; i++) 
    for ( j=0; j<51; j++) {
      m->colors[1+((i-1)*51) +j].r = 0;
      m->colors[1+((i-1)*51) +j].g = (i>=3) ? (char)i*51           : 0;
      m->colors[1+((i-1)*51) +j].b = (i<=2) ? (char)255-((i-1)*51) : 0;
    }
*/
  for ( j=1; j<255; j++) {
      m->colors[j].r = 65;
      m->colors[j].g = (char)(j);
      m->colors[j].b = (char)(255-j);
    }
  //  for ( j=250; j<255; j++) {
    for ( j=CONTOUR_BOT; j<CONTOUR_TOP; j++) {
      m->colors[j].r = (char)(255-((j-CONTOUR_BOT)*255/CONTOUR_RANGE));
      m->colors[j].g = (char)(255-((j-CONTOUR_BOT)*255/CONTOUR_RANGE));
      m->colors[j].b = 255;
    }
    m->colors[1].r = 255;
    m->colors[1].g = 255;
    m->colors[1].b = 255;
    m->colors[2].r = 0;
    m->colors[2].g = 127;
    m->colors[2].b = 127;
    m->colors[3].r = 127;
    m->colors[3].g = 0;
    m->colors[3].b = 127;
    m->colors[4].r = 127;
    m->colors[4].g = 127;
    m->colors[4].b = 0;
    m->colors[5].r = 127;
    m->colors[5].g = 0;
    m->colors[5].b = 0;
    m->colors[6].r = 64;
    m->colors[6].g = 127;
    m->colors[6].b = 127;
    m->colors[7].r = 127;
    m->colors[7].g = 64;
    m->colors[7].b = 0;

    
    m->colors[255].r = 0;
    m->colors[255].g = 255;
    m->colors[255].b = 0;
    //  }
  
  SDL_SetPalette(m->screen, SDL_LOGPAL|SDL_PHYSPAL, m->colors, 0, 256);
  SDL_SetPalette(m->image,  SDL_LOGPAL|SDL_PHYSPAL, m->colors, 0, 256);

  //Initialize SDL_TTF  
/*  if ( TTF_Init() < 0 ) {
    fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
    exit(2);
  }
  atexit(TTF_Quit);

  m->font = TTF_OpenFont("mplotft.ttf", FONT_SIZE);
  if ( m->font == NULL ) {
    fprintf(stderr, "Couldn't load font from %s: %s\n",
	    "mplotft.ttf", SDL_GetError());
    exit(2);
  }
  TTF_SetFontStyle(m->font, TTF_STYLE_NORMAL);
  */

  //Initialize SFont
  m->font = IMG_Load("mplotft.bmp");
  InitFont(m->font);


  return 1;
}

int mplot_draw_text(mplot *m, char *text, int x, int y, int color) {
  /*m->text = TTF_RenderText_Solid(m->font, text, m->colors[color]);
  if ( m->text != NULL ) {
    m->dstrect.x = x;
    m->dstrect.y = y;
    m->dstrect.w = m->text->w;
    m->dstrect.h = m->text->h;
    SDL_BlitSurface(m->text, NULL, m->screen, &(m->dstrect));
    SDL_FreeSurface(m->text);
    SDL_UpdateRects(m->screen, 1, &(m->dstrect));
  }
  */
    //  PutString(m->screen, x, y, text);
    //  SDL_UpdateRects(m->screen, 1, &(m->dstrect));

  return 0;
}

int  mplot_init(mplot *m){
  int i;

  m->contour_w = DEFAULT_CONTOUR_W;
  m->contour_h = DEFAULT_CONTOUR_H;
  m->gridz= m->gridz2= m->dist= m->xmatrix= m->ymatrix= NULL;
  m->curve= NULL;
  m->contour_grid= m->contour_grid2= NULL;
  m->cut_contour= NULL;
  mplot_change_contour_resolution(m, m->contour_w, m->contour_h);
  for(i=0; i<1000; i++)
    m->w[i] = 1.0;
  m->root_of_2 = sqrt(2);

  m->num_div_x1=DEFAULT_BIG_AXE_DIV;
  m->num_div_y1=DEFAULT_BIG_AXE_DIV;  
  m->num_div_x2=DEFAULT_SMALL_AXE_DIV;
  m->num_div_y2=DEFAULT_SMALL_AXE_DIV;  
  strcpy(m->title, DEFAULT_TITLE);
  strcpy(m->httitle1, DEFAULT_HTTITLE1);
  strcpy(m->httitle2, DEFAULT_HTTITLE2);
  strcpy(m->hbtitle1, DEFAULT_HBTITLE1);
  strcpy(m->hbtitle2, DEFAULT_HBTITLE2);
  strcpy(m->vltitle, DEFAULT_VLTITLE);
  strcpy(m->vmtitle, DEFAULT_VMTITLE);
  strcpy(m->vrtitle, DEFAULT_VRTITLE);
  m->autoclear=1;
  m->autorefresh=1;
  m->graph1_x=m->graph1_y=m->graph2_x=m->graph2_y=0;
  m->numlevels = DEFAULT_NUM_LEVELS;
  //Initialize tangent table
  for (i=0; i<NUM_TANGENTS; i++) {
    m->tangent[i] = (FIXED_NUMBER)tan( (1.0+2.0*(double)i)*PI/NUM_ANGLES   );
#ifdef DEBUG
    printf("m->tangent[%d]=%f\n", i, m->tangent[i]);
#endif
  }
  
  return 1;
}
///////////////////////////////////////////////////////////////////////////
void mplot_change_contour_resolution(mplot *m, int newresx, int newresy) {
  int error=0;
     m->contour_w = newresx;
     m->contour_h = newresy;

     if (m->gridz) free(m->gridz);
     if (m->gridz2) free(m->gridz2);
     if (m->dist) free(m->dist);
     if (m->xmatrix) free(m->xmatrix);
     if (m->ymatrix) free(m->xmatrix);
     if (m->curve) free(m->curve);
     if (m->contour_grid) free(m->contour_grid);
     if (m->contour_grid2) free(m->contour_grid2);
     if (m->cut_contour) free(m->cut_contour);

     m->gridz=          (FIXED_NUMBER *) malloc(sizeof(FIXED_NUMBER)* ((m->contour_w)*(m->contour_h)) );
     if (!(m->gridz)) error=1;     
     m->gridz2=         (FIXED_NUMBER *)malloc(sizeof(FIXED_NUMBER)* ((m->contour_w)*(m->contour_h)));
     if (!(m->gridz2)) error=1;     
     m->dist=           (FIXED_NUMBER *)malloc(sizeof(FIXED_NUMBER)* ((m->contour_w)*(m->contour_h)));
     if (!(m->dist)) error=1;     
     m->xmatrix=        (FIXED_NUMBER *)malloc(sizeof(FIXED_NUMBER)* (m->contour_w));
     if (!(m->xmatrix)) error=1;     
     m->ymatrix=        (FIXED_NUMBER *)malloc(sizeof(FIXED_NUMBER)* (m->contour_h));
     if (!(m->ymatrix)) error=1;     
     m->curve=          (contour_curve *)malloc(sizeof(contour_curve)* ((m->contour_w)*(m->contour_h)) * 100);
     if (!(m->curve)) error=1;     
     m->contour_grid=   (Uint8 *)       malloc(sizeof(Uint8)* ((m->contour_w)*(m->contour_h)));
     if (!(m->contour_grid)) error=1;     
     m->contour_grid2=  (Uint8 *)       malloc(sizeof(Uint8)* ((m->contour_w)*(m->contour_h)));
     if (!(m->contour_grid2)) error=1;     
     m->cut_contour=    (int *)         malloc(sizeof(int)* (2*(m->contour_h)));
     if (!(m->cut_contour)) error=1;     
     //     printf("%p\n", m->cut_contour);
     //     fgetc(stdin);
     if(error) {
       fprintf(stderr, "Insuficient memory!!\n");       
       exit(1);
     }
}

///////////////////////////////////////////////////////////////////////////

mplot * mplot_open(void) {
      mplot *m;
      m = mplot_new();
      mplot_init(m);
      return m;
}
int mplot_initgraph (mplot *m) {
    mplot_sdl_init(m);
    return 1;
}
int mplot_close(mplot *m) {
     mplot_destroy(m);
     return 1;
}
int mplot_closegraph(mplot *m){
  /* Shutdown all SDL subsystems */
  //TTF_CloseFont(m->font);
  SDL_Quit();
  return 1;
}
int mplot_gengrid(mplot *m, FIXED_NUMBER *cx, FIXED_NUMBER *cy, FIXED_NUMBER *cz,
                        int num_items, FIXED_NUMBER *cfex, FIXED_NUMBER *cfey,
                        int fenum_items, int method,
                        int resx, int resy,
                        FIXED_NUMBER *gridz, int autodetect_ranges,
                        FIXED_NUMBER botx, FIXED_NUMBER topx,
                        FIXED_NUMBER boty, FIXED_NUMBER topy,
                        FIXED_NUMBER botz, FIXED_NUMBER topz
                        ) {
   static int oldresx=0, oldresy=0;
   static int number_grid, i;
   static FIXED_NUMBER x[MAX_ITEMS], y[MAX_ITEMS], z[MAX_ITEMS];
   static FIXED_NUMBER fex[MAX_ITEMS], fey[MAX_ITEMS];
   FILE *f;

   f = fopen("temp.txt", "w");
   fprintf(f, "oldresx=%d oldresy=%d\n", oldresx, oldresy);
   fflush(f);
   if ((resx!=oldresx) || (resy!=oldresy)) {
      oldresx = resx;
      oldresy = resy;
      mplot_change_contour_resolution(m, resx, resy);
   }
   fprintf(f, "resolucao setada\n");
   fflush(f);
   if (autodetect_ranges)
      mplot_autodetect_values(m, cx, cy, cz, num_items, cfex, cfey, fenum_items);
   else
       mplot_set_values(m, botx, topx, boty, topy, botz, topz);
   fprintf(f, "passei pelo autodetect\n");
   fflush(f);
   for (i=0; i<num_items; i++) {
       x[i]=m->botx + (m->topx- cx[i]);
       y[i]=m->botx + (m->topx- cy[i]);
       z[i]=cz[i];
   }
   for (i=0; i<fenum_items; i++) {
       fex[i]=m->botx + (m->topx- cfex[i]);
       fey[i]=m->botx + (m->topx- cfey[i]);
   }
   fprintf(f, "Itens invertidos\n");
   fflush(f);

   fprintf(f, "num_items = %d\n", num_items);
   for(i=0; i<num_items; i++) {
      fprintf(f, "cx[%d]=%f (%f) ", i, cx[i], x[i]);
      fprintf(f, "cy[%d]=%f (%f) ", i, cy[i], y[i]);
      fprintf(f, "cz[%d]=%f (%f)\n", i, cz[i], z[i]);
   }
   fflush(f);

   switch (method) {
   case METHOD_BARNES:
        fprintf(f, "Usando método de barnes...\n");
        fflush(f);
        mplot_gengrid_barnes(m, x, y, z, num_items);
        break;
   case METHOD_BOXCAR:
        fprintf(f, "Usando método boxcar...\n");
        fflush(f);
        mplot_gengrid_box(m, x, y, z, num_items);
        break;
   default:
           printf("Unknow method!\n");
           break;
   }
   fprintf(f, "Grade gerada\n");
   fflush(f);

   number_grid = (m->contour_w) * (m->contour_h);
   for (i=0; i<number_grid; i++)
       gridz[i] = m->gridz[i];

   fprintf(f, "Grade copiada\n");
   fflush(f);

   //Update Graph coord
   m->graph_width = GRAPH_WIDTH;
   m->graph_height = GRAPH_HEIGHT;
   if ( ((m->topx-m->botx)/((m->topy-m->boty))) > ((FIXED_NUMBER)GRAPH_WIDTH/((FIXED_NUMBER)GRAPH_HEIGHT))) {
      m->graph_height = GRAPH_WIDTH*(m->topy-m->boty)/(m->topx-m->botx);
   } else {
      m->graph_width  = GRAPH_HEIGHT*(m->topx-m->botx)/((m->topy-m->boty));
   }

   fprintf(f, "m=%p  decimal=(%d)\n", m, (int)m);
   fprintf(f, "cx[0]=%f\n", cx[0]);
   fprintf(f, "cy[0]=%f\n", cy[0]);
   fprintf(f, "cz[0]=%f\n", cz[0]);
   fprintf(f, "num_items = %d\n", num_items);
   fprintf(f, "cfex[0]=%f\n", cfex[0]);
   fprintf(f, "cfey[0]=%f\n", cfey[0]);
   fprintf(f, "fenum_items = %d\n", fenum_items);
   fprintf(f, "resx=%d resy=%d \n", resx, resy);
   fprintf(f, "gridz[0]=%f\n", gridz[0]);
   fprintf(f, "gridz[1]=%f\n", gridz[1]);
   fprintf(f, "gridz[2]=%f\n", gridz[2]);
   fprintf(f, "autodetect_ranges=%d\n", autodetect_ranges);
   fprintf(f, "botx=%f\n", botx);
   fprintf(f, "botx=%f\n", topx);
   fprintf(f, "boty=%f\n", boty);
   fprintf(f, "boty=%f\n", topy);
   fprintf(f, "botz=%f\n", botz);
   fprintf(f, "botz=%f\n", topz);
   fprintf(f, "size of float = %d  size of double = %d", sizeof(float), sizeof(double));
   fclose(f);

   return 1;
}

///////////////////////////////////////////////////////////////////////////
void mplot_destroy(mplot *m){
  mplot_closegraph(m);
  /* Free allocated memory */
  if (!m) {
    printf("Warning: NULL Pointer passed to mplot_destroy!\n");
    return; 
  }  
  free(m);
}

////////////////////////////////////////////////////////////////////////////

int mplot_do_contour(mplot *m, int num_levels, int sx, int sy) {
  static int i, j, l;
  static int graph2_init_offset;
  FIXED_NUMBER clevel_bot, clevel_top; //current level
  Uint8 current_color;
  
  //Update values on instance
  m->graph2_x = sx;
  m->graph2_y = sy;
  
  graph2_init_offset = sy*SCREEN_W +sx;
  /* Point to the screen buffer */
//  m->bufp  = (Uint8 *)(m->screen->pixels) + graph2_init_offset;
    //+ (SCREEN_H/2 +LINES_BETWEEN_GRAPHS)*(m->screen->pitch);  
  //  m->bufpf = (Uint8 *)(m->screen->pixels) + SCREEN_H*(m->screen->pitch);  

  //  printf("Drawing %d levels of contour!\n", num_levels);
  //For each level, find the intersections and so draw the curves.
  //  m->botz = 0;
  //  m->topz = 20;
  for (l=1; l<num_levels; l++) {
    mplot_init_curve(m);
    clevel_bot    = m->botz + (l-1)*(m->topz - m->botz)/(num_levels);
    clevel_top    = m->botz + (l)*(m->topz - m->botz)/(num_levels);
    current_color = (Uint8)(CONTOUR_BOT +(l*CONTOUR_RANGE/num_levels));
    //printf("Drawing contour for level %f-%f (b=%f t=%f)\n", clevel_bot, clevel_top, m->botz, m->topz);
    //    printf(" %d ", i);

    //First, find the values within range...
    for (i=0; i<(m->contour_w)*(m->contour_h); i++) { //For each point on the grid
      //Within range ?	
      m->contour_grid[i] = 
	((m->gridz[i]<clevel_top) && (m->gridz[i]>clevel_bot)) ? (Uint8) 1 : (Uint8)0;   
      //printf("%f %f %f %d\n", m->gridz[i], clevel_bot, clevel_top,  m->contour_grid[i]);
    }
    //...find the intersections...
    for (i=1; i<(m->contour_w)-2; i+=2)
      for (j=1; j<(m->contour_h)-2; j+=2) {
	//Eight points to be compared
	//1
	if (  m->contour_grid[i-1+((j-1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i-1, j-1, i, j, current_color);
	//2
	if (  m->contour_grid[i+((j-1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i, j-1, i, j, current_color);
	//3
	if (  m->contour_grid[i+1+((j-1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i+1, j-1, i, j, current_color);
	//4
	if (  m->contour_grid[i-1+((j)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i-1, j, i, j, current_color);
	//5
	if (  m->contour_grid[i+1+((j)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i+1, j, i, j, current_color);
	//6
	if (  m->contour_grid[i-1+((j+1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i-1, j+1, i, j, current_color);
	//7
	if (  m->contour_grid[i+((j+1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i, j+1, i, j, current_color);
	//8
	if (  m->contour_grid[i+1+((j+1)*(m->contour_w))] !=
	      m->contour_grid[i+(j*(m->contour_w))])
	  mplot_add_curve(m, i+1, j+1, i, j, current_color);
      }

    //...and now, draw the curve
    mplot_draw_curve(m, sx, sy);
  }

  mplot_draw_box(m, m->graph2_x, m->graph2_y, 
		 m->graph2_x+GRAPH_W/*-(GRAPH_W%(m->contour_w))*/,
		 m->graph2_y+GRAPH_H/*-(GRAPH_H%(m->contour_h))*/,
		 BOXCOLOR);
  
  return 1;
}

int mplot_draw_line(mplot *m, int x1, int y1, int x2, int y2, Uint8 color) {
  Uint8 *screen;
  int x_unit, y_unit;
  int offset;
  int ydiff, xdiff;
  int length, i, error_term;

  screen = (Uint8 *)(m->screen->pixels );
  offset=y1*SCREEN_W+x1;
  ydiff=y2-y1;
  if (ydiff<0) {
    ydiff=-ydiff;
    y_unit=-SCREEN_W;
  } else y_unit=SCREEN_W;
  xdiff=x2-x1;
  if (xdiff<0) {
    xdiff=-xdiff;
    x_unit=-1;
  } else x_unit=1;
  
  error_term=0;
  if (xdiff>ydiff) {
    length = xdiff+1;
    for (i=0; i<length; i++) {
      screen[offset]=color;
      offset+=x_unit;
      error_term+=ydiff;
      if (error_term>xdiff) {
	error_term-=xdiff;
	offset+=y_unit;
      }
    }
  } else {
    length = ydiff+1;
    for (i=0; i<length; i++) {
      screen[offset]=color;
      offset+=y_unit;
      error_term+=xdiff;
      if (error_term>0) {
	error_term-=ydiff;
	offset+=x_unit;
      }
    }
  }

  return 1;
}

int mplot_draw_curve(mplot *m, int sx, int sy) {
  //  static int x, y;
  static int x1, y1;
  static int x2, y2;
  static int i, temp;  

  //  printf("number of curves = %d\n", m->num_curves);
  for (i=0; i<m->num_curves; i++) {
    x1 = m->curve[i].x1;
    y1 = m->curve[i].y1;
    x2 = m->curve[i].x2;
    y2 = m->curve[i].y2;
    /* //////////////////////////////////////// */
    if((x1!=x2)&&(y1!=y2)){
      //continue;
      temp=x1;
      x1=x2;
      x2=temp;
      x1 *= GRAPH_W/(m->contour_w);
      y1 *= GRAPH_H/((m->contour_h));
      x2 *= GRAPH_W/(m->contour_w);
      y2 *= GRAPH_H/((m->contour_h));
    } else if((x1==x2)&&(y1!=y2)){
      continue;
      y1 = (y1+y2)/2;
      if (x1<x2) {
	x1-=1;
	x2+=1;
      } else {
	x1+=1;
	x2-=1;
      }
      x1 *= GRAPH_W/(m->contour_w);
      y1 *= GRAPH_H/((m->contour_h));
      x2 *= GRAPH_W/(m->contour_w);
      y2 *= GRAPH_H/((m->contour_h));
    } else if((x1!=x2)&&(y1==y2)){
      continue;
      x1 = (x1+x2)/2;
      if (y1<y2) {
	y1-=1;
	y2+=1;
      } else {
	y1+=1;
	y2-=1;
      }
      x1 *= GRAPH_W/(m->contour_w);
      y1 *= GRAPH_H/((m->contour_h));
      x2 *= GRAPH_W/(m->contour_w);
      y2 *= GRAPH_H/((m->contour_h));
    }
    ////////////////////////////////////////// 
    //x = (m->curve[i].x1 + m->curve[i].x2)/2;
    //y = (m->curve[i].y1 + m->curve[i].y2)/2;

    //    *(m->bufp + (x) + (y*SCREEN_W)) = m->curve[i].color;

    mplot_draw_line(m, x1+sx, y1+sy, 
		    x2+sx, y2+sy, m->curve[i].color);

    //    *(m->bufp+i+(i*SCREEN_W))=255;
    //    printf("x=%d y=%d\t", x, y);
  }
  return 1;
}

int mplot_add_curve(mplot *m, int x1, int y1, int x2, int y2, Uint8 color) {
  //  printf("x=%d y=%d\t", x1, y1);
  m->curve[m->num_curves].x1 = x1;
  m->curve[m->num_curves].y1 = y1;
  m->curve[m->num_curves].x2 = x2;
  m->curve[m->num_curves].y2 = y2;
  m->curve[m->num_curves].ignore = 0;
  m->curve[m->num_curves].color  = color;

  m->num_curves++;
  return 1;
}
int mplot_init_curve(mplot *m) {
  //Initialize contour curve
//  bzero(m->curve, sizeof(contour_curve)*10*(m->contour_w)*(m->contour_h));
  memset(m->curve, 0, sizeof(contour_curve)*10*(m->contour_w)*(m->contour_h));
//  bzero(m->contour_grid, sizeof(Uint8)*(m->contour_w)*(m->contour_h));
  memset(m->contour_grid, 0, sizeof(Uint8)*(m->contour_w)*(m->contour_h));
  m->num_curves=0;
  return 1;
}

/*
int mplot_gengrid_nearest(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *z, int num_items) {
  int i, j, l, k=0;
  FIXED_NUMBER dist, mindist=1000.0;
  FIXED_NUMBER xx, yy, aux, auy;
  for (j=0; j<(m->contour_h); j++)
    for (i=0; i<(m->contour_w); i++) {
      m->gridz[k]=0;
      mindist=1000.0;     
      aux = m->botx + (((double)i)*(m->topx - m->botx)/((double)(m->contour_w)));
      auy = m->boty + (((double)((m->contour_h)-j))*(m->topy - m->boty)/((double)(m->contour_h)));
      m->gridz[k] = 0;
      for (l=0; l<num_items; l++) {
	xx = (aux-x[l]);
	yy = (auy-y[l]);
	dist = hypot(xx, yy);
	if (dist<mindist) {
	  mindist     = dist;
	  m->gridz[k] = z[l];	  
	}                
      }   
      ((Uint8 *) (m->image->pixels))[k] = CONTOUR_BOT + (Uint8) (
	 (CONTOUR_RANGE)*((m->gridz[k]-m->botz)/(m->topz-m->botz))
	 );                 
      k++;
    }
  return 1; 
}
*/
int mplot_gengrid_barnes(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *z, int num_items) {
  int i, j, k; 
  FIXED_NUMBER vl;//, topz=0, botz=1000;
  FIXED_NUMBER xr, yr;
  //  printf("%f %f\n%f %f\n%f %f\n\n", m->botx, m->topx, m->boty, m->topy, m->botz, m->topz);  
  for (i=0; i<(m->contour_w); i++){
    m->xmatrix[i] = m->botx + (((double)i)*(m->topx - m->botx)/((double)(m->contour_w)));
    //    printf("xm[%d]=%f \t", i, m->xmatrix[i]);
  }
  //  printf("\n");
  for (j=0; j<(m->contour_h); j++){    
    m->ymatrix[j] = m->boty + (((double)((m->contour_h)-j))*(m->topy - m->boty)/((double)(m->contour_h)));
    //    printf("ym[%d]=%f \t", j, m->ymatrix[j]);
  }
  //  printf("\n");

  //Calculates xr and yr
  xr = 1.4 * ( (m->topx - m->botx)/sqrt(num_items)  );
  yr = 1.4 * ( (m->topy - m->boty)/sqrt(num_items)  );

  mplot_create_grid_barnes (m, x, y, z, m->w,
			    num_items, 
			    xr, yr, 0.5, 2);
	
  //Detect top and bottom z again
  for (k=0; k<(m->contour_w)*(m->contour_h); k++){
    if (m->botz>m->gridz[k]) m->botz=m->gridz[k];
    if (m->topz<m->gridz[k]) m->topz=m->gridz[k];
  }
  for (k=0; k<(m->contour_w)*(m->contour_h); k++) {
    //    if (m->gridz[k]!=0)
    //printf("%d %f\t", k, m->gridz[k]);
    vl = (CONTOUR_RANGE*(m->gridz[k]- m->botz)/(m->topz-m->botz));
    //    vl = (m->gridz2[k] != 0 ) ? 1 : 0;
    //    if (vl<0) vl=0;
    //    if (vl>CONTOUR_RANGE) vl=CONTOUR_RANGE;
    ((Uint8 *) (m->image->pixels))[k] = (Uint8)(CONTOUR_BOT + vl);
  }

/**/
  return 1;
}

int
mplot_create_grid_barnes (mplot *m, FIXED_NUMBER *x, 
			  FIXED_NUMBER *y, FIXED_NUMBER *z, 
			  FIXED_NUMBER *w, 
			  int num_items, 
			  double xr,    double yr,
			  double gamma, unsigned int iter
			  )
{
  static FIXED_NUMBER *z_last, *z_last2;
  static int ini=0;
  double xr2, yr2;
  unsigned int iteration;
  unsigned int i, j;
  unsigned int k, k2;
  int ix, iy;
  double fx, fy;
  int in_x, in_y;

  //Zera grade e grade auxiliar
  //bzero(m->gridz,  sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );
  memset(m->gridz,  0, sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );
  
  if (!ini) {
    //Zera z_last e z_last2
    z_last  = (FIXED_NUMBER *)calloc(num_items, sizeof(FIXED_NUMBER));
    z_last2 = (FIXED_NUMBER *)calloc(num_items, sizeof(FIXED_NUMBER));
    //bzero(m->gridz2, sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );
    memset(m->gridz2, 0, sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );
  } else {
    //bzero(z_last,  sizeof(FIXED_NUMBER) * num_items);
    //bzero(z_last2, sizeof(FIXED_NUMBER) * num_items);
    memset(z_last,  0, sizeof(FIXED_NUMBER) * num_items);
    memset(z_last2, 0, sizeof(FIXED_NUMBER) * num_items);
  }

  xr2 = xr, yr2 = yr;
  for (iteration = 0; iteration < iter; iteration++) {
    // Interpolate on grid
    k2=0;
    for (j = 0; j < (m->contour_h); j++) {
      for (i = 0; i < (m->contour_w); i++) {
	//if ((i+3<m->cut_contour[j]) || (i-3>m->cut_contour[j+(m->contour_h)]))
	//m->gridz[k2] = 0;	else
	m->gridz[k2] = mplot_interpolate_barnes(m->xmatrix[i],
						  m->ymatrix[j],
						  m->gridz[i+ (j*(m->contour_w))],
						  -1, // no skip
						  num_items,
						  x, y, z, w,
						  z_last, xr2, yr2);
	k2++;
      }
    }
    // Interpolate at data
    for (k = 0; k < num_items; k++) {
      in_x = mplot_nearest(m, x[k], (double *) m->xmatrix, (m->contour_w), &ix, &fx);
      in_y = mplot_nearest(m, y[k], (double *) m->ymatrix, (m->contour_h), &iy, &fy);
      if (in_x && in_y) {
	mplot_value_i_j(m, ix, iy, x[k], y[k], (double *) &z_last2[k]);
      } else {
	z_last2[k]
	  = mplot_interpolate_barnes(x[k],
			       y[k],
			       z_last[k],
			       -1, // no skip
			       num_items,
			       x, y, z, w,
			       z_last, xr2, yr2);
      }
    }
    // Update z_last
    for (k = 0; k < num_items; k++)
      z_last[k] = z_last2[k];
    if (!gamma)
      break;
    // Alter search range
    xr2 *= sqrt(gamma);
    yr2 *= sqrt(gamma);
  }				// iteration
  return 1;
}

double
mplot_interpolate_barnes (double xx, double yy, double zz, 
			  int skip,  unsigned int n_k, 
			  FIXED_NUMBER *x, FIXED_NUMBER *y,
			  FIXED_NUMBER *z, FIXED_NUMBER *weight,
			  FIXED_NUMBER *z_last, 
			  double xr,  double yr
			  )
{
  double sum = 0.0, sum_w = 0.0;
  double w; int k;
  double dx, dy, arg;
  for (k = 0; k < (int)n_k; k++) {
    if (k != skip) {
      dx = (xx - x[k]) / xr;
      dx *= dx;
      dy = (yy - y[k]) / yr;
      dy *= dy;
      arg = dx + dy;
      w = weight[k] * exp(-arg);
      sum += w * (z[k] - z_last[k]);
      sum_w += w;
    }
  }
  if (sum_w > 0.0)
    return (zz + sum / sum_w);
  else
    return 1.0e22; 
}

int
mplot_value_i_j(mplot *m, unsigned int ii, unsigned int jj, double xx, double yy, double *value)
{
  double          Dx, Dy;	// width/height of domain with point
  double          f0, f1, f2, f3;
  double dx;			// x - x_to_left
  double dy;			// y - y_below
  // Fiddle with dx,dy,Dx,Dy, to avoid looking past array
  dx = (ii == (m->contour_w) - 1 ? 0.0 : xx - m->xmatrix[ii]);
  dy = (jj == (m->contour_h) - 1 ? 0.0 : yy - m->ymatrix[jj]);
  if (m->gridz2[ii+ (jj*(m->contour_w))] == 0
      || (dx != 0.0 && m->gridz2[ii+1+ (jj*(m->contour_w))] == 0)
      || (dy != 0.0 && m->gridz2[ii+ ((jj+1)*(m->contour_w))] == 0)
      || (dx != 0.0 && dy != 0.0 && m->gridz2[ii+1+((jj+1)*(m->contour_w))] == 0)) {
    *value = 1.0e22; //gr_currentmissingvalue();
    return 0;
  }
  f0 = m->gridz[ii+ (jj*(m->contour_w))];
  f1 = dx != 0 ? m->gridz[ii+1+ (jj*(m->contour_w))] - f0 : 0.0;
  f2 = dy != 0 ? m->gridz[ii+ ((jj+1)*(m->contour_w))] - f0 : 0.0;
  f3 = (dx != 0 && dy != 0) ? m->gridz[ii+1+((jj+1)*(m->contour_w))] - f0 - f1 - f2 : 0;
  Dx = dx != 0 ? m->xmatrix[ii + 1] - m->xmatrix[ii] : 1;
  Dy = dy != 0 ? m->ymatrix[jj + 1] - m->ymatrix[jj] : 1;
  *value = f0 + f1 * dx / Dx + f2 * dy / Dy + f3 * dx / Dx * dy / Dy;
  return 1;
}

int
mplot_nearest(mplot *mp, double x, double g[], unsigned int ng, int *b, double *f)
{
  int l = 0;			// left index
  int r = ng - 1;		// right index
  int m;			// middle index
  if (g[0] < g[1]) {		// ascending sequence
    if (x <= g[l])	{ *b = 0; *f = g[l] - x; return 0; }
    if (g[r] <= x)	{ *b = r; *f = x - g[r]; return 0; }
    m = (l + r) / 2;
    while (r - l > 1) {
      if (x < g[m])
	r = m;
      else if (g[m] < x)
	l = m;
      else { 
	*b = m;
	*f = (x - g[*b]) / (g[*b+1] - g[*b]);
	return 1; 
      }
      m = (r + l) / 2;
    }
    *b = l;
    *f = (x - g[*b]) / (g[*b+1] - g[*b]);
    return 1;
  } else {			// descending sequence
    if (x >= g[l])	{ *b = 0; *f = g[l] - x; return 0; }
    if (g[r] >= x)	{ *b = r; *f = x - g[r]; return 0; }
    m = (l + r) / 2;
    while (r - l > 1) {
      if (x > g[m])
	r = m;
      else if (g[m] > x)
	l = m;
      else {
	*b = m;
	*f = (x - g[*b]) / (g[*b+1] - g[*b]);
	return 1;
      }
      m = (r + l) / 2;
    }
    *b =  l;
    *f = (x - g[*b]) / (g[*b+1] - g[*b]);
    return 1;
  }
}

int mplot_gengrid_box(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, FIXED_NUMBER *z, int num_items) {
  int i, j, k; 
  FIXED_NUMBER vl;//, topz=0, botz=1000;
  FIXED_NUMBER xr, yr;
  
  for (i=0; i<(m->contour_w); i++)
    m->xmatrix[i] = m->botx + (((double)i)*(m->topx - m->botx)/((double)(m->contour_w)));
  
  for (j=0; j<(m->contour_h); j++)
    m->ymatrix[j] = m->boty + (((double)((m->contour_h)-j))*(m->topy - m->boty)/((double)(m->contour_h)));

  //Calculates xr and yr
  xr = 1.4 * ( (m->topx - m->botx)/sqrt(num_items)  );
  yr = 1.4 * ( (m->topy - m->boty)/sqrt(num_items)  );

  mplot_create_grid_box (m, xr, yr, x, y, z, num_items);
					   
                     
  for (k=0; k<(m->contour_w)*(m->contour_h); k++) {
    //    if (m->gridz[k]!=0)
    //printf("%d %f\t", k, m->gridz[k]);
    vl = CONTOUR_RANGE*(m->gridz[k]- m->botz)/(m->topz-m->botz);
    //    vl = (m->gridz2[k] != 0 ) ? 1 : 0;
    //if (vl<0) vl=0;
    if (vl>CONTOUR_RANGE) vl=CONTOUR_RANGE;
    if (vl<0) vl=0;
    ((Uint8 *) (m->image->pixels))[k] = (Uint8) CONTOUR_BOT + (Uint8) (vl);
  }
                    /*   */
  return 1;
}


int mplot_create_grid_box(mplot *m, double xr, double yr,
			  FIXED_NUMBER *x, FIXED_NUMBER *y, 
			  FIXED_NUMBER *z, int num_items) {
  int number_to_find = 5, //Neighbors to find
    enlargements = 1;
  unsigned int i, j;
  unsigned int found = 0;
  double xx, yy;
  unsigned int number_found;
  double fpred;

  //bzero(m->gridz, sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );
  memset(m->gridz, 0, sizeof(FIXED_NUMBER) * (m->contour_w)*(m->contour_h) );

  // Do interpolation.
  for (i = 0; i < (m->contour_w); i++) {
    //    if (_chatty > 0) 
    //      printf("\n");
    xx = m->xmatrix[i];
    for (j = 0; j < (m->contour_h); j++) {
      //      if (_chatty > 0)
      //	printf(".");
      yy = m->ymatrix[j];      
      number_found = mplot_gr_grid_box(m, x, y, z, num_items,
			      xx, yy, xr, yr,
				       //0,// means boxcar
			      number_to_find, enlargements,
			      &fpred);
      m->gridz[j*(m->contour_w) + i] = fpred;
/*      if (number_found >= (unsigned int)number_to_find) {
	_legit_xy(i, j) = true;
	found++;
	} else {
	_legit_xy(i, j) = false;
	}*/
    }
  }
  return found;
}

int
mplot_gr_grid_box( mplot *m, FIXED_NUMBER *x,
		   FIXED_NUMBER *y,
		   FIXED_NUMBER *f,
		   int num_items, 
		   double x0, double y0,
		   double xr, double yr,
		   //int method,
		   unsigned int neighbors,
		   int enlargements,
		   double *fOut) {
  unsigned int n = num_items;
  double          dx, dy;
  int             enlarge = 0;
  unsigned int    i;
  unsigned int    num_in_rect;
  //  double          d2;	// squared distance 
  double          sumw;	// sum of weights 
  double          sumfw;	// sum of weighted values 
  //  double          w;		// weight of f[i] 
  // Search the rectangle, increasing its size if necessary.  
  do {
    num_in_rect = 0;
    sumw = sumfw = 0.0;
    for (i = 0; i < n; i++) {
      dx = x0 - x[i];
      if (-xr <= dx && dx <= xr) {
	dy = y0 - y[i];
	if (-yr <= dy && dy <= yr) {
	  sumw += 1.0;
	  sumfw += f[i];
	  num_in_rect++;
	}
      }
    }
    xr *= m->root_of_2;
    yr *= m->root_of_2;
  } while ((++enlarge <= enlargements || enlargements < 0)
	   && num_in_rect < neighbors);
  if (num_in_rect > 0) {
    *fOut = sumfw / sumw;
    return num_in_rect;
  } else {
    *fOut = 0;
    return 0;
  }
}


int mplot_set_values(mplot *m, 
		     FIXED_NUMBER botx, FIXED_NUMBER topx, 
		     FIXED_NUMBER boty, FIXED_NUMBER topy, 
		     FIXED_NUMBER botz, FIXED_NUMBER topz) {
  m->topx = topx;  
  m->botx = botx;  
  m->topy = topy;
  m->boty = boty;  
  m->topz = topz;  
  m->botz = botz;  
  return 1;
}

int mplot_draw_colors(mplot *m, int sx, int sy) {
  static int i, offset;
  static char value[CONTOUR_RANGE];
  static int coffset;
  offset=0;
  coffset=sx + (sy*SCREEN_W);
  for (i=(GRAPH_HEIGHT); i>-1; i--){
    memset((m->screen->pixels)+coffset+offset, 
	   CONTOUR_BOT+i*CONTOUR_RANGE/GRAPH_HEIGHT, COLORS_WIDTH);
    if ( (i%(GRAPH_HEIGHT/((m->numlevels)))) ==0 ) {
      memset((m->screen->pixels)+coffset+COLORS_WIDTH+offset, 
	     CONTOUR_BOT+(i*CONTOUR_RANGE/GRAPH_HEIGHT), COLORS_WIDTH);
      //      printf("%.2f %.2f\t ", m->topz, m->botz);
      sprintf(value, "%.2f", m->topz - 
      	      ( ((m->topz-m->botz)/(m->numlevels)) * 
		(GRAPH_HEIGHT-(i))*(m->numlevels)/(GRAPH_HEIGHT) )
				 );
      mplot_draw_text(m, value, 
		      sx+2*COLORS_WIDTH, 
		      sy
		      +(GRAPH_HEIGHT) -i 
		      //-((GRAPH_HEIGHT/2)/m->numlevels)
		      -(FONT_SIZE/2)
		      , 
		      LABELCOLOR);  
    }

    offset+=SCREEN_W;
  }
  
  return 1;
}


int mplot_update (mplot *m, FIXED_NUMBER *cx, FIXED_NUMBER *cy, FIXED_NUMBER *cz, int num_items, FIXED_NUMBER *cfex, FIXED_NUMBER *cfey, int fenum_items, int method, int numlevels, int autodetect, int graph_type){
  static int ii=0;
  static FIXED_NUMBER x[MAX_ITEMS], y[MAX_ITEMS], z[MAX_ITEMS];
  static FIXED_NUMBER fex[MAX_ITEMS], fey[MAX_ITEMS];
//  FILE *f1;
  mplot_clear_screen(m);
#ifdef DEBUG
  printf("mplot: numero de items: %d\n", num_items);
  for (ii=0; ii<num_items; ii++) {
    printf("x=%f y=%f z=%f\n", cx[ii], cy[ii], cz[ii]);
  }
#endif

  if (autodetect)
    mplot_autodetect_values(m, cx, cy, cz, num_items, cfex, cfey, fenum_items);

// f1 = fopen("saida.txt", "a");
// fprintf(f1, "%d\n", num_items);
// fclose(f1);
 for (ii=0; ii<num_items; ii++) {
   x[ii]=m->botx + (m->topx- cx[ii]);
   y[ii]=m->botx + (m->topx- cy[ii]);
   z[ii]=cz[ii];
 }
 for (ii=0; ii<fenum_items; ii++) {
   fex[ii]=m->botx + (m->topx- cfex[ii]);
   fey[ii]=m->botx + (m->topx- cfey[ii]);
 }

 //#define DEBUG
#ifdef DEBUG
  printf("numitems: %d\n", num_items);
  printf("botx1=%f topx1=%f\n",  m->botx, m->topx);
  printf("boty1=%f topy1=%f\n",  m->boty, m->topy);
  printf("botz1=%f topz1=%f\n\n", m->botz, m->topz);
#endif
    
  m->graph_width = GRAPH_WIDTH;
  m->graph_height = GRAPH_HEIGHT;
  
  if ( ((m->topx-m->botx)/((m->topy-m->boty))) > ((FIXED_NUMBER)GRAPH_WIDTH/((FIXED_NUMBER)GRAPH_HEIGHT))) {
    m->graph_height = GRAPH_WIDTH*(m->topy-m->boty)/(m->topx-m->botx);
#ifdef DEBUG
    printf("1 ");
#endif
  } else {
    m->graph_width  = GRAPH_HEIGHT*(m->topx-m->botx)/((m->topy-m->boty));
#ifdef DEBUG
    printf("2 ");
#endif
  }
#ifdef DEBUG
  printf("%d %d\n", GRAPH_W, GRAPH_H);
#endif
  /*
  printf("\nup1 %p\n", m->cut_contour);
  fgetc(stdin);
  mplot_detect_finit(m, fex, fey, fenum_items);
  printf("\nup2 %p %p\n", m->cut_contour, m);
  fgetc(stdin);
  fflush(stdout);
  printf("\na\n");
  mplot_detect_cut_graph(m);
  printf("\nup3 %p\n", m->cut_contour);
  fgetc(stdin);
  printf("b\n");
  */
   // Generate grid data from colums 
  switch (method) {
  case METHOD_BARNES:
    mplot_gengrid_barnes(m, x, y, z, num_items);
    printf("a\n");
    break;
  case METHOD_BOXCAR:
    mplot_gengrid_box(m, x, y, z, num_items);
    break;
  default:
    fprintf(stderr, "Unknow method!\n");
    break;
  }  
  /*
  for (ii=0; ii<(m->contour_w); ii++)    {
      for (jj=0; jj<(m->contour_h); jj++) {
          printf("%f ", (float)(m->gridz[jj*(m->contour_w) +ii]));
      }
      printf("\n");
  }
    */
/*  for (ii=0; ii<num_items; ii++)    {
      printf("%f ", (float)(cz[ii]));
  }*/
  //////////////////////////////////////////////////////////////////// 
  mplot_show_shade(m, GRAPH1_X, GRAPH1_Y);
  printf("b\n");
  //  mplot_draw_cut_graph(m, GRAPH1_X, GRAPH1_Y);
  //  mplot_draw_finit(m, GRAPH1_X, GRAPH1_Y, COLOR_FINIT);
  ////////////////////////////////////////////////////////////////////
  m->numlevels = numlevels;
  //  mplot_do_contour(m, m->numlevels, GRAPH2_X, GRAPH2_Y); //Draw ten levels of contour  
  //  mplot_draw_cut_graph(m, GRAPH2_X, GRAPH2_Y);
  //  mplot_draw_finit(m, GRAPH2_X, GRAPH2_Y, COLOR_FINIT);
  ////////////////////////////////////////////////////////////////////
  mplot_draw_titles (m);
  printf("c\n");
  mplot_draw_colors(m, 2, LINES_TO_SKIP_ON_START);
  printf("d\n");
  //  mplot_draw_axes(m);
  printf("e\n");
  ////////////////////////////////////////////////////////////////////
  mplot_refresh_screen(m);
  ////////////////////////////////////////////////////////////////////
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
  
  return 1;
}

int mplot_show_shade(mplot *m, int sx, int sy) {
  //Show the grid bigger
  static int k, k2, i, j, l;   
  static int graph_init_offset;

  //Update values on instance
  m->graph1_x = sx;
  m->graph1_y = sy;

  graph_init_offset = sx + sy*SCREEN_W;
  k=k2=0;
  for (j=0; j<(m->contour_h); j++)
    for (l=0; l<GRAPH_H/((m->contour_h)); l++) {
      for (i=0; i<(m->contour_w); i++) {
	//k= ((j+l)*SCREEN_W) + (i*SCREEN_W/(m->contour_w));
	memset( (char *)(m->screen->pixels + graph_init_offset) + k, 
		(int)(((char *)m->image->pixels)[j*(m->contour_w)+i]),
		GRAPH_W/(m->contour_w));
	k += GRAPH_W/(m->contour_w);
      }
      //k+= GRAPH_OFFSET;
      k=k2+SCREEN_W;
      k2=k;
    }
  mplot_draw_box(m, m->graph1_x, m->graph1_y, 
		 m->graph1_x+GRAPH_W,//-(GRAPH_W%(m->contour_w)),
		 m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h)),
		 BOXCOLOR);
  return 1;
}


int mplot_set_axe_div(mplot *m, int num_div_x1, int num_div_y1, 
		      int num_div_x2, int num_div_y2){
  m->num_div_x1=num_div_x1;
  m->num_div_y1=num_div_y1;
  m->num_div_x2=num_div_x2;
  m->num_div_y2=num_div_y2;
  return 1;
}

int mplot_set_titles(mplot *m, char *title, 
		     char *httitle1, char *httitle2,
		     char *hbtitle1, char *hbtitle2,
		     char *vltitle, char *vmtitle, char *vrtitle) {
  strcpy(m->title, title);
  strcpy(m->httitle1, httitle1);
  strcpy(m->httitle2, httitle2);
  strcpy(m->hbtitle1, hbtitle1);
  strcpy(m->hbtitle2, hbtitle2);
  strcpy(m->vltitle, vltitle);
  strcpy(m->vmtitle, vmtitle);
  strcpy(m->vrtitle, vrtitle);
  return 1;
}

int mplot_clear_screen(mplot *m) {
  memset(m->screen->pixels, BACKCOLOR, SCREEN_H*(m->screen->pitch));
  return 1;
}

int mplot_refresh_screen(mplot *m) {
  if ( SDL_MUSTLOCK(m->screen) ) {
    SDL_UnlockSurface(m->screen);
  }
  
  SDL_UpdateRect(m->screen, 0, 0, SCREEN_W, SCREEN_H);
  return 1;
}

int mplot_set_autoclear(mplot *m, int autoclear) {
  m->autoclear=autoclear;
  return 1;
}
int mplot_set_autorefresh(mplot *m, int autorefresh){
  m->autorefresh=autorefresh;
  return 1;
}

int mplot_draw_box (mplot *m, int x1, int y1, int x2, int y2, Uint8 color){
  //Horizontal top
  mplot_draw_line(m, x1, y1, x2, y1, color);
  //Horizontal bottom
  mplot_draw_line(m, x1, y2, x2, y2, color);
  //Vertical left
  mplot_draw_line(m, x1, y1, x1, y2, color);
  //Vertical right
  mplot_draw_line(m, x2, y1, x2, y2, color);
  return 1;
}

int mplot_draw_titles (mplot *m){
  static int i, offset, len;
  static char letter[2];
  mplot_draw_text(m, m->title, (SCREEN_W-(strlen(m->title)*FONT_SIZE/2))/2, 
		  LINES_TO_SKIP_ON_START/4, TITLECOLOR);
  mplot_draw_text(m, m->httitle1, 
		  m->graph1_x + (GRAPH_W-(strlen(m->httitle1)*FONT_SIZE/2))/2, 
		  m->graph1_y-FONT_SIZE-3, TITLECOLOR);
  mplot_draw_text(m, m->httitle2, 
		  m->graph2_x + (GRAPH_W-(strlen(m->httitle2)*FONT_SIZE/2))/2,
		  m->graph2_y-FONT_SIZE-3, TITLECOLOR);
  mplot_draw_text(m, m->hbtitle1, 
		  m->graph1_x + (GRAPH_W-(strlen(m->hbtitle1)*FONT_SIZE/2))/2, 
		  m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE, TITLECOLOR);
  mplot_draw_text(m, m->hbtitle2, 
		  m->graph2_x + (GRAPH_W-(strlen(m->hbtitle2)*FONT_SIZE/2))/2,
		  m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE, TITLECOLOR);
  
  letter[1]='\0';
  offset=0;
  len=strlen(m->vltitle);
  for (i=0; i<len; i++) {
    letter[0] = m->vltitle[i];
    mplot_draw_text(m, letter, 
		    m->graph1_x- 2*FONT_SIZE,
		    m->graph1_y+((GRAPH_H-len*FONT_SIZE)/2)+offset, TITLECOLOR);
    offset+=FONT_SIZE;
  }
  offset=0;
  len=strlen(m->vmtitle);
  for (i=0; i<len; i++) {
    letter[0] = m->vrtitle[i];
    mplot_draw_text(m, letter, 
		    m->graph2_x  /*- (GRAPH_W%(m->contour_w))*/+ GRAPH_W + FONT_SIZE/2 +3,
		    m->graph2_y+((GRAPH_H-len*FONT_SIZE)/2)+offset, TITLECOLOR);
    offset+=FONT_SIZE;
  }
  offset=0;
  len=strlen(m->vmtitle);
  for (i=0; i<len; i++) {
    letter[0] = m->vmtitle[i];
    mplot_draw_text(m, letter, 
		    m->graph1_x  /*- (GRAPH_W%(m->contour_w))*/+GRAPH_W+3,
		    m->graph2_y+((GRAPH_H-len*FONT_SIZE)/2)+offset, TITLECOLOR);
    offset+=FONT_SIZE;
  }
  return 1;
}

int mplot_autodetect_values(mplot *m, 
			    FIXED_NUMBER *x, FIXED_NUMBER *y, 
			    FIXED_NUMBER *z, int num_items,
			    FIXED_NUMBER *fex, FIXED_NUMBER *fey, 
			    int num_items_2) {
  static int i;

  // Find the top and the bottom values
  m->topx = m->topy = m->topz = -10.0;
  m->botx = m->boty = m->botz = 1000.0;
  for (i=0; i<num_items; i++) {
    if (x[i] < (m->botx)) m->botx = x[i];
    if (y[i] < (m->boty)) m->boty = y[i];
    if (z[i] < (m->botz)) m->botz = z[i];
    if (x[i] > (m->topx)) m->topx = x[i];
    if (y[i] > (m->topy)) m->topy = y[i];
    if (z[i] > (m->topz)) m->topz = z[i];
#ifdef DEBUG2
    printf(" %6.3f %d\n", z[i], i);    
    printf("%f %f %f\n", x[i], y[i], z[i]);
#endif
  }
  for (i=0; i<num_items_2; i++) {
    if (fex[i] < (m->botx)) m->botx = fex[i];
    if (fey[i] < (m->boty)) m->boty = fey[i];
    if (fex[i] > (m->topx)) m->topx = fex[i];
    if (fey[i] > (m->topy)) m->topy = fey[i];
#ifdef DEBUG2
    printf(" %6.3f %d\n", z[i], i);
    printf("%f %f\n", fex[i], fey[i]);
#endif
  }
  return 1;
}

int mplot_draw_axes(mplot *m) {
  static int i;
  static char value[100];
  //Vertical
  for (i=0; i<m->num_div_y1+1; i++) {
    mplot_draw_line(m, 
		    m->graph1_x-AXE_SIZE_BIG, 
		    m->graph1_y +(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y1)),
		    m->graph1_x, 
		    m->graph1_y +(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y1)),
		    AXE1COLOR);
    mplot_draw_line(m, m->graph2_x-AXE_SIZE_BIG, 
		    m->graph2_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y1)),
		    m->graph2_x, 
		    m->graph2_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y1)),
		    AXE1COLOR);
  }
  for (i=0; i<m->num_div_y2+1; i++) {
    mplot_draw_line(m,
            m->graph1_x-AXE_SIZE_SMALL,
		    m->graph1_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y2)),
		    m->graph1_x, 
		    m->graph1_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y2)),
		    AXE1COLOR);
    mplot_draw_line(m, m->graph2_x-AXE_SIZE_SMALL, 
		    m->graph2_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y2)),
		    m->graph2_x, 
		    m->graph2_y+(i*(GRAPH_H-(GRAPH_H%(m->contour_h)))/(m->num_div_y2)),
		    AXE1COLOR);
  }  

  //Horizontal
  for (i=0; i<m->num_div_x1+1; i++) {
    mplot_draw_line(m,
		    m->graph1_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x1)),
		    m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h)),
		    m->graph1_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x1)),
		    m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h))+AXE_SIZE_BIG,
		    AXE2COLOR);
    mplot_draw_line(m,
		    m->graph2_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x1)),
		    m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h)),
		    m->graph2_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x1)),
		    m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h))+AXE_SIZE_BIG,
		    AXE2COLOR);
  }
  for (i=0; i<m->num_div_x2+1; i++) {
    mplot_draw_line(m,
		    m->graph1_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x2)),
		    m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h)),
		    m->graph1_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x2)),
		    m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h))+AXE_SIZE_SMALL,
		    AXE2COLOR);
    mplot_draw_line(m,
		    m->graph2_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x2)),
		    m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h)),
		    m->graph2_x+(i*(GRAPH_W/*-(GRAPH_W%(m->contour_w))*/)/(m->num_div_x2)),
		    m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h))+AXE_SIZE_SMALL,
		    AXE2COLOR);
  }
  sprintf(value, AXE_MASK, m->topy);
  mplot_draw_text(m,value, m->graph1_x- strlen(value)*FONT_SIZE/2, 
		  m->graph1_y-FONT_SIZE, AXE1COLOR);
  mplot_draw_text(m,value, m->graph2_x- strlen(value)*FONT_SIZE/2, 
		  m->graph2_y-FONT_SIZE, AXE1COLOR);
  sprintf(value, AXE_MASK, m->boty);
  mplot_draw_text(m,value, m->graph1_x- strlen(value)*FONT_SIZE/2, 
		  m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h)), AXE1COLOR);
  mplot_draw_text(m,value, m->graph2_x- strlen(value)*FONT_SIZE/2, 
		  m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h)), AXE1COLOR);
  sprintf(value, AXE_MASK, m->topx);
  mplot_draw_text(m,value, 
		  m->graph1_x+GRAPH_W/*-(GRAPH_W%(m->contour_w))*/-strlen(value)*FONT_SIZE/2,
		  m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE/2, AXE1COLOR);
  mplot_draw_text(m,value, 
		  m->graph2_x+GRAPH_W/*-(GRAPH_W%(m->contour_w))*/-strlen(value)*FONT_SIZE/2,
		  m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE/2, AXE1COLOR);
  sprintf(value, AXE_MASK, m->botx);
  mplot_draw_text(m,value, m->graph1_x, 
		  m->graph1_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE/2, AXE1COLOR);
  mplot_draw_text(m,value, m->graph2_x, 
		  m->graph2_y+GRAPH_H-(GRAPH_H%(m->contour_h))+FONT_SIZE/2, AXE1COLOR);


  return 1;
}

int mplot_detect_finit(mplot *m, FIXED_NUMBER *x, FIXED_NUMBER *y, int num_items) {
  static int i, j, index;
  static int left_i, right_i;
  static int num_detected;
  static int first, current, next;
  static FIXED_NUMBER tang, distance, temp;
  static FIXED_NUMBER xm, ym; //Center point
  static FIXED_NUMBER yc; //Current point
  
  index=-1;
  num_detected=0;
  first=0;

  if(num_items<NUM_ANGLES) {
    printf("Warning: number of items passed to detect finite element is less than %d, not detecting!\n", NUM_ANGLES);
    return 0;
  }
  //Initialize structures and find median points
  for (i=0; i<NUM_ANGLES; i++) {
    m->fex[i]=0.0;
    m->fey[i]=0.0;
    m->fedetected[i]=0;
  }
  //Calculates center point
  xm = m->botx + (m->topx - m->botx)/2;
  ym = m->boty + (m->topy - m->boty)/2;
  //  printf("xm = %f, ym=%f\n", xm, ym);  
  for (i=0; i<num_items; i++) {
    //First, detect index in vector corresponding to this tangent
    if (x[i]-xm==0.0) { //Special case: tangent equal infinite
      if ((y[i]-ym)>0.0)   index=(NUM_ANGLES/2);
      else                 index=0;
    } else {
      tang = (y[i]-ym) / (x[i]-xm);      
      //printf("%f \t", tang);
      //Special case 2, last and first tangents
      if ((tang <m->tangent[0]) && (tang>m->tangent[NUM_TANGENTS-1])){
	if (x[i]-xm<0.0) index=(3*NUM_ANGLES/4) ;
	else             index=(NUM_ANGLES/4)   ;
      }
      //Now, generic case
      left_i  =(3*NUM_ANGLES/4)+1;
      right_i =(NUM_ANGLES/4)+1;
      for (j=0; j<NUM_TANGENTS-1; j++) {
	if (left_i!=0) { 
	  if ((tang > m->tangent[j]) && (tang < m->tangent[j+1])){
	    if (y[i]-ym<0.0) index= left_i;
	    else             index= right_i;
	    //	    if (x[i]-xm<0.0) index=(left_i>(3*NUM_ANGLES/4)) ? left_i : right_i;
	    //	    else             index=(left_i>(3*NUM_ANGLES/4)) ? right_i : left_i;
	  } 
	} else { //Verify OR instead of AND
	  if ((tang > m->tangent[j]) || (tang < m->tangent[j+1])){
	    if (y[i]-ym<0.0) index= left_i;
	    else             index= right_i;
	  }
	}
	// 	printf("left=%d, right=%d\n", left_i, right_i);
	left_i++;
	left_i= left_i % NUM_ANGLES;
	right_i++;        
      }

    }
    //Alredy have an index, set if greater
    if (!(m->fedetected[index])) {
      m->fex[index]=x[i];
      m->fey[index]=y[i];
      m->fedistance[index]=hypot((double)(x[i]-xm), (double)(y[i]-ym));
      m->fedetected[index]=1;
      num_detected ++;
    } else {
      distance = hypot((double)x[i], (double)y[i]);
      if (m->fedistance[index]<distance) {
	m->fex[index]=x[i];
	m->fey[index]=y[i];
	m->fedistance[index]=distance;
      }
    }
  }
  m->xm = xm; 
  m->ym = ym;
#ifdef DEBUG3
  printf("numbers detected = %d\n", num_detected);
#endif

  ///////////////////////////////////////////////
  if (num_detected<2) return 0;
  if (num_detected!=NUM_ANGLES) {
    //Fill undetected by linear interpolation
    //Find the first detected
    for (i=0; i<NUM_ANGLES; i++) {
      if (m->fedetected[i]) {
	first = i;
	break;
      }
    }
    //Fill starting from first
    next = first;
    while (num_detected!=NUM_ANGLES){
      current=next;
      //Find the current detected...
      while(m->fedetected[(current+1)%NUM_ANGLES]) {
	//      printf("%d %d\n", current, next);
	current++;
	current = current % NUM_ANGLES;
	if (current==next){
	  //	num_detected = NUM_ANGLES;
	  break;
	}
      }
      //...and now find the next detected 
      next = current;
      do {
	next = (next+1) % NUM_ANGLES;
	if (next==current){ //All detected
	  break; 
	}          
      }while(!(m->fedetected[next]));
      
      j= (next<current) ? next + NUM_ANGLES : next;
      for (i=current+1; i<j; i++) {
	m->fex[i%NUM_ANGLES] = m->fex[current]+ (i-current) *(m->fex[next]-m->fex[current]) /(j-current);
	m->fey[i%NUM_ANGLES] = m->fey[current]+ (i-current) *(m->fey[next]-m->fey[current]) /(j-current);
	//m->fedetected[i%NUM_ANGLES] = 1;
	num_detected ++;;
      }
    }
  }
  
  ////////////////////////////////////////////
  //Now detect values within poligon and linear interpolate it
  for (i=0; i<NUM_ANGLES; i++) {
    current=i;
    next = (i+2);
#ifdef DEBUG4
    printf("\nPoint to be analised: line(%d and %d), point: %d \t",
	   current%NUM_ANGLES, next%NUM_ANGLES, (next-1)%NUM_ANGLES);
    printf("line: ([%.2f,%.2f] and [%.2f,%.2f]) , point: [%.2f,%.2f]\n",
	   m->fex[current%NUM_ANGLES], m->fey[current%NUM_ANGLES],
	   m->fex[next%NUM_ANGLES], m->fey[next%NUM_ANGLES],
	   m->fex[(next-1)%NUM_ANGLES], m->fey[(next-1)%NUM_ANGLES]
	   );
#endif
    
    while(next<current+NUM_ANGLES) {
#ifdef DEBUG4
      printf("%d ", next);
#endif
      
      //Verify if next-1 is within polygon
      // Same y
      if ((m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES])==0) { 
	if (fabs(m->fey[(next-1)%NUM_ANGLES]-ym)
	    < fabs(m->fey[(next)%NUM_ANGLES]-ym)) {
	  //Point is in
	  if (m->fedetected[(next-1)%NUM_ANGLES]) {
	    m->fex[(next-1)%NUM_ANGLES] = m->fex[current%NUM_ANGLES] 
	      + (next-1-current)		
	      *(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES])
	      /(next-current);	
	    m->fey[(next-1)%NUM_ANGLES] = m->fey[current%NUM_ANGLES] 
	      + (next-1-current)		
	      *(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES])
	      /(next-current);	
	  }
	  next ++;
	} else break;
      }	
      // Same x  
      else if ((m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES])==0) { 
	if (fabs(m->fex[(next-1)%NUM_ANGLES]-xm)
	    < fabs(m->fex[(next)%NUM_ANGLES]-xm)) {
	  //Point is in
	  if (m->fedetected[(next-1)%NUM_ANGLES]) {
	    m->fex[(next-1)%NUM_ANGLES] = m->fex[current%NUM_ANGLES] 
	      + (next-1-current)		
	      *(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES])
	      /(next-current);	
	    m->fey[(next-1)%NUM_ANGLES] = m->fey[current%NUM_ANGLES] 
	      + (next-1-current)		
	      *(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES])
	      /(next-current);	
	  }
	  next ++;
	} else break;
      }	
      // Generic case
      else { 
	//Find y on xm. If positive, point will be in if be lesser.
	//If negative, point will be in if be greater
	temp = m->fey[current%NUM_ANGLES] + (xm-m->fex[current%NUM_ANGLES])
	  *(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES]) 
	  /(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES]);	
	yc = m->fey[current%NUM_ANGLES] + 
	  (m->fex[(next-1)%NUM_ANGLES]-m->fex[current%NUM_ANGLES])
	  *(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES]) 
	  /(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES]);	
	if (temp>ym) { //Need be lesser to be in
	  if ((m->fey[(next-1)%NUM_ANGLES]) < yc) {
	    //Point is in
	    if (m->fedetected[(next-1)%NUM_ANGLES]) {
	      m->fex[(next-1)%NUM_ANGLES] = m->fex[current%NUM_ANGLES] 
		+ (next-1-current)		
		*(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES])
		/(next-current);	
	      m->fey[(next-1)%NUM_ANGLES] = m->fey[current%NUM_ANGLES] 
		+ (next-1-current)		
		*(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES])
		/(next-current);	
	    }
	    next ++;
	  } else break;
	} else if (temp<ym) { //Need be greater to be in
	  if ((m->fey[(next-1)%NUM_ANGLES])>yc) {
	    //Point is in
	    if (m->fedetected[(next-1)%NUM_ANGLES]) {	
	      m->fex[(next-1)%NUM_ANGLES] = m->fex[current%NUM_ANGLES] 
		+ (next-1-current)		
		*(m->fex[next%NUM_ANGLES] - m->fex[current%NUM_ANGLES])
		/(next-current);	
	      m->fey[(next-1)%NUM_ANGLES] = m->fey[current%NUM_ANGLES] 
		+ (next-1-current)		
		*(m->fey[next%NUM_ANGLES] - m->fey[current%NUM_ANGLES])
		/(next-current);	
	    }
	    next ++;
	  } else break;
	} else {
	  //Point is out, line goes by the center
	  break;
	}
      }	
    }        
  }
  return 1;
}
int mplot_draw_finit(mplot *m, int sx, int sy, int color) {
  static int i;

#ifdef DEBUG3
  printf("botx=%f topx=%f boty=%f topy=%f\n", m->botx, m->topx, m->boty, m->topy);
  for (i=0; i<NUM_ANGLES; i++) {
    printf("angle %d: detected=%d \t", 
	   i, m->fedetected[i]);
    printf("x=%f, y=%f, dist=%f tan=%f\n", m->fex[i] - m->xm, m->fey[i] - m->ym, m->fedistance[i]
	   , (m->fey[i]-m->ym)/(m->fex[i]-m->xm));
  }
  printf("\n");
#endif

  mplot_draw_line(m, 
		  sx+ (m->fex[0]-m->botx)*GRAPH_W/(m->topx-m->botx)/*-(GRAPH_W%(m->contour_w))*/,
		  sy+ GRAPH_H - (m->fey[0]-m->boty)*GRAPH_H/(m->topy-m->boty)-(GRAPH_H%(m->contour_h)),
		  sx+ (m->fex[NUM_ANGLES-1]-m->botx)*GRAPH_W/(m->topx-m->botx)/*-(GRAPH_W%(m->contour_w))*/,
		  sy+ GRAPH_H - (m->fey[NUM_ANGLES-1]-m->boty)*GRAPH_H/(m->topy-m->boty)-(GRAPH_H%(m->contour_h)),
		  color);

  for (i=0; i<NUM_ANGLES-1; i++) 
    mplot_draw_line(m, 
		    sx+ (m->fex[i]-m->botx)*GRAPH_W/(m->topx-m->botx)/*-(GRAPH_W%(m->contour_w))*/,
		    sy+ GRAPH_H - (m->fey[i]-m->boty)*GRAPH_H/(m->topy-m->boty) -(GRAPH_H%(m->contour_h)),
		    sx+ (m->fex[i+1]-m->botx)*GRAPH_W/(m->topx-m->botx)/*-(GRAPH_W%(m->contour_w))*/,
		    sy+ GRAPH_H - (m->fey[i+1]-m->boty)*GRAPH_H/(m->topy-m->boty)-(GRAPH_H%(m->contour_h)),
		    color); 

  return 1;
}

int mplot_detect_cut_graph(mplot *m) {
  static int left_i, right_i, i;
  static int lx, ly, rx, ry;
  static int lxa, lya, rxa, rya, temp;
  static FIXED_NUMBER maxy, miny;
  static int index_min, index_max, stop_left, stop_right;
  //bzero(m->cut_graph, sizeof(int)*2*GRAPH_HEIGHT);
  printf("%p %p\n", m->cut_contour, m);
  memset(m->cut_graph, 0, sizeof(int)*2*GRAPH_HEIGHT);

  //First, detect the top most index
  maxy=0;
  miny=1000000000;
  for (i=0; i<NUM_ANGLES; i++) {
    if (m->fey[i]>maxy) {
      maxy=m->fey[i];
      index_max=i;
    }
    if (m->fey[i]<miny) {
      miny=m->fey[i];
      index_min=i;
    }
  }
  left_i = right_i = index_max;
  lx = (m->fex[left_i]-m->botx)*GRAPH_W/(m->topx-m->botx);
  ly = (m->fey[left_i]-m->boty)*GRAPH_H/(m->topy-m->boty);
  rx = (m->fex[right_i]-m->botx)*GRAPH_W/(m->topx-m->botx);
  ry = (m->fey[right_i]-m->boty)*GRAPH_H/(m->topy-m->boty);
  left_i++;
  right_i--;
  lxa=lx; lya=ly; rxa=rx; rya=ry;
  stop_left=(index_min+1)%NUM_ANGLES;
  stop_right=(index_min-1+NUM_ANGLES)%NUM_ANGLES;
  do {
    //lxa=lx; lya=ly; rxa=rx; rya=ry;
    if(((left_i%NUM_ANGLES)!=stop_left)) {
      lx = (m->fex[left_i]-m->botx)*GRAPH_W/(m->topx-m->botx);
      ly = (m->fey[left_i]-m->boty)*GRAPH_H/(m->topy-m->boty);
      if(lya>ly) {
	temp=ly; ly=lya; lya=temp;
	temp=lx; lx=lxa; lxa=temp;
      }
      for (i=lya; i<ly; i++) {
	m->cut_graph[i] = lxa + (i-lya)*(lx-lxa)/(ly-lya);
      }
      left_i++;      
      left_i=left_i%NUM_ANGLES;
    }
    //    printf("\n");
    //          printf("| left=%d right=%d (%d,%d)\n", left_i, right_i, index_max, index_min);
    if (((right_i%NUM_ANGLES)!=stop_right)) {
      rx = (m->fex[right_i]-m->botx)*GRAPH_W/(m->topx-m->botx);
      ry = (m->fey[right_i]-m->boty)*GRAPH_H/(m->topy-m->boty);
      if(rya>ry) {
	temp=ry; ry=rya; rya=temp;
	temp=rx; rx=rxa; rxa=temp;
      }
      for (i=rya; i<ry; i++) {
	m->cut_graph[(i+GRAPH_HEIGHT)] = rxa + (i-rya)*(rx-rxa)/(ry-rya);
      }
      right_i--;
      if (right_i<0) right_i += NUM_ANGLES;
    }
  } while (((left_i%NUM_ANGLES)!=stop_left)||((right_i%NUM_ANGLES)!=stop_right));

  /////////////////////////////////////
  //Do it on contour cut now
  printf("rr\n");
  for (i=0; i<2*(m->contour_h); i++) {
    printf("i=%d %p\n", i, m->cut_contour);
    m->cut_contour[i]=0;
  }
  printf("ss\n");
  
  for (i=0; i<(m->contour_h); i++) {
    printf("%d %d %d\n", i, (m->contour_h)-i-1, -1-i+2*(m->contour_h));
    fflush(stdout);
    m->cut_contour[(m->contour_h)-i-1] = 0;//m->cut_graph[(int)(i*(GRAPH_HEIGHT)/(m->contour_h))]*(m->contour_w)/(GRAPH_WIDTH);
    m->cut_contour[-1-i+2*(m->contour_h)] = 0;//m->cut_graph[(int)((i+(m->contour_h))*(GRAPH_HEIGHT)/(m->contour_h))]*(m->contour_w)/(GRAPH_WIDTH);
    //printf("%d %d %d\n", i, m->cut_contour[i], m->cut_contour[i+(m->(m->contour_h))]);
  }

  return 1;
}
int mplot_draw_cut_graph(mplot *m, int sx, int sy) {
  static int i, max, diff1, diff2, s1, s2;

  max = GRAPH_H-(GRAPH_H%(m->contour_h));
  for (i=1; i<max; i++) {

    if ((m->cut_graph[i])!=(m->cut_graph[i+GRAPH_HEIGHT])) {
      s1=(sy+max-i)*SCREEN_W +sx+1;
      diff1 = m->cut_graph[i]-2;
      s2=(sy+max-i)*SCREEN_W +sx+1+m->cut_graph[i+GRAPH_HEIGHT];
      diff2 = GRAPH_W-2-m->cut_graph[i+GRAPH_HEIGHT];
      if (diff1>0) 
	memset(m->screen->pixels + s1, BACKCOLOR, diff1);
      if (diff2>0) 
	memset(m->screen->pixels + s2, BACKCOLOR, diff2);
    }

/*    mplot_draw_line(m, sx+1, 
		    sy+max-i,
		    sx+m->cut_graph[i]-1, 
		    sy+max-i, 
		    BACKCOLOR);
    mplot_draw_line(m, sx+1+ m->cut_graph[i+GRAPH_HEIGHT], 
		    sy+max-i,
		    sx+GRAPH_W-1, 
		    sy+max-i, 
		    BACKCOLOR);

  */
  }    

  return 1;
}

int mplot_detect_graph_size(mplot *m) {
  m->graph_width = GRAPH_WIDTH;
  m->graph_height = GRAPH_HEIGHT;	  
  if ( ((m->topx-m->botx)/((m->topy-m->boty))) > ((FIXED_NUMBER)GRAPH_WIDTH/((FIXED_NUMBER)GRAPH_HEIGHT))) {
    m->graph_height = GRAPH_WIDTH*(m->topy-m->boty)/(m->topx-m->botx);
  } else {
    m->graph_width  = GRAPH_HEIGHT*(m->topx-m->botx)/((m->topy-m->boty));
  }	  
  return 1;
}


int mplot_show_dots(mplot *m, int sx, int sy, FIXED_NUMBER *cx, FIXED_NUMBER *cy, FIXED_NUMBER *cz, int num_items) {
  //Show the grid bigger
  static int graph_init_offset;
  static int i, offsetx, offsety;

  if ((m->topx-m->botx==0) || (m->topy-m->boty==0) || (m->topz-m->botz==0) )  {
    fprintf(stderr, "Invalid range of values!\n");
    return 0;
  }

  //Update values on instance
  m->graph1_x = sx;
  m->graph1_y = sy;

  graph_init_offset = sx + sy*SCREEN_W; 
  for (i=0; i<num_items; i++) {
    offsetx = m->graph_width  *(cx[i]-m->botx)/(m->topx-m->botx);
    offsety = m->graph_height *(cy[i]-m->boty)/(m->topy-m->boty);
    *((Uint8 *)(m->screen->pixels+graph_init_offset+offsetx+(m->screen->w * offsety))) = 
      CONTOUR_BOT + (Uint8) (CONTOUR_RANGE*(cz[i]-m->botz)/(m->topz-m->botz));
  }

  mplot_draw_box(m, m->graph1_x, m->graph1_y, 
		 m->graph1_x+GRAPH_W,
		 m->graph1_y+GRAPH_H,
		 BOXCOLOR);
  return 1;
}
