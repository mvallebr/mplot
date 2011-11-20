#include <fconv.h>
#include <mplot.h>

//This program reads a input file exported on ansys by the cdwrite command and writes a file containing a number of node by line. Each one of this nodes is a node on the border of the mesh
int main (int argc, char *argv[]) {
	FILE *f, *fhm;
	mplot *m;
	int shade;
	FIXED_NUMBER gridz[4*DEFAULT_CONTOUR_W*DEFAULT_CONTOUR_H];

	//Init mplot library
	m = mplot_open();
	if (!m) return -1;
	mplot_initgraph(m);
	//Init fconv library
	fconv_init();

	//open the input files
	if (argc<4) {
	  fprintf(stderr, "Usage: %s DENS_FILE HM_FILE shade(bool)\n", argv[0]);
	  return 1;
	}
	f = fopen(argv[1], "r");
	if (!f) {
	  fprintf(stderr, "Couldn't read file '%s'!\n", argv[1]);
	  return 2;
	}
	fhm = fopen(argv[2], "r");
	if (!fhm) {
	  fprintf(stderr, "Couldn't read file '%s'!\n", argv[2]);
	  fclose(f);
	  return 3;
	}
	shade = atoi(argv[3]);
	//Read the files
	printf("Reading HM file...\n");
	fconv_read_hm(fhm);

	printf("Reading dens file...\n");
	fconv_read_dens(f);
	
	printf("Generating columns...\n");
	fconv_fill_columns_by_elements();

	printf("\nDetecting ranges...\n");
	mplot_autodetect_values(m, fconv_cx, fconv_cy, fconv_cz, fconv_num_columns, 
				fconv_cx, fconv_cy, fconv_num_columns);

	printf("Ranges: \nminx=%f max=%f \nminy=%f maxy=%f \nminz=%f maxz=%f \n",
	       m->botx, m->topx, m->boty, m->topy, m->botz, m->topz);

	printf("Detecting graph size...\n");
	mplot_detect_graph_size(m);
	//	m->graph_width  *= 2;
	//	m->graph_height *= 2;
	printf("Graph size - x:%d y:%d\n", m->graph_width, m->graph_height);

/*	m->colors[20].r = 100;
	m->colors[20].g = 100;
	m->colors[20].b = 100;

	m->colors[19].r = 80;
	m->colors[19].g = 120;
	m->colors[19].b = 100;

	m->colors[18].r = 120;
	m->colors[18].g = 80;
	m->colors[18].b = 100;

	m->colors[17].r = 80;
	m->colors[17].g = 80;
	m->colors[17].b = 100;

	m->colors[16].r = 80;
	m->colors[16].g = 80;
	m->colors[16].b = 80;

	m->colors[15].r = 60;
	m->colors[15].g = 60;
	m->colors[15].b = 60;

	m->colors[14].r = 30;
	m->colors[14].g = 70;
	m->colors[14].b = 50;

	m->colors[13].r = 0;
	m->colors[13].g = 0;
	m->colors[13].b = 200;

	m->colors[12].r = 0;
	m->colors[12].g = 0;
	m->colors[12].b = 100;

	m->colors[11].r = 50;
	m->colors[11].g = 50;
	m->colors[11].b = 50;

	m->colors[10].r = 30;
	m->colors[10].g = 30;
	m->colors[10].b = 10;

	m->colors[BACKCOLOR].r = 255;
	m->colors[BACKCOLOR].g = 255;
	m->colors[BACKCOLOR].b = 255;
	m->colors[BOXCOLOR].r = 10;
	m->colors[BOXCOLOR].g = 10;
	m->colors[BOXCOLOR].b = 10;
*/
	SDL_SetPalette(m->screen,  SDL_LOGPAL|SDL_PHYSPAL, m->colors, 0, 256);

	printf("Drawing...\n");
	mplot_clear_screen( m);
	mplot_draw_colors(m, 10, GRAPH1_Y);


	if (!shade) 
	  mplot_show_dots(m, 100, 100, fconv_cx, fconv_cy, fconv_cz, fconv_num_columns);
	else {
	  mplot_gengrid(m, 
			fconv_cx, fconv_cy, fconv_cz, fconv_num_columns, 
			fconv_cx, fconv_cy, fconv_num_columns, 	  
			METHOD_BARNES,
			DEFAULT_CONTOUR_W, DEFAULT_CONTOUR_H, gridz, MPLOT_AUTODETECT,
			0, 0, 0, 0, 0, 0);
	  mplot_show_shade(m, GRAPH1_X, GRAPH1_Y);
	  //mplot_update(m, 
	  //   fconv_cx, fconv_cy, fconv_cz, fconv_num_columns, 
	  //   fconv_cx, fconv_cy, fconv_num_columns, 	  		       
	  //   METHOD_BARNES, 10, MPLOT_AUTODETECT, MPLOT_GRAPHTYPE_SHADE);	  
	}
	printf("Done.\n");
	
	//Plot and receive the events
	while(1) {

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
	}
	
	//Bye bye
	fclose(f);
	fclose(fhm);

	//Finish mplot library
	mplot_close(m);
	
	//Exit the main program
	return 0;
}









