#include <fconv.h>
#include <stdlib.h>

//This program reads a input file exported on ansys by the cdwrite command and writes a file containing a number of node by line. Each one of this nodes is a node on the border of the mesh
int main (int argc, char *argv[]) {
	FILE *f, *fout, *fout2;
	double corrente, espessura, tensao_terra;
	int diam;
	fconv_init();
	if (argc<8) {
	  fprintf(stderr, "Usage: %s CDWRITE_FILE FILE_OUTPUT  FILE_OUTPUT2 corrente espessura tensao_terra diam(0 or 1) \n", argv[0]);
	  return 1;
	}
	corrente=atof(argv[4]);
	espessura=atof(argv[5]);
	tensao_terra=atof(argv[6]);	
	diam=atoi(argv[7]);
	f = fopen(argv[1], "r");
	if (!f) {
	  fprintf(stderr, "Couldn't read file '%s'!\n", argv[1]);
	  return 2;
	}
	fout = fopen(argv[2], "w");
	if (!fout) {
	  fprintf(stderr, "Couldn't write on file '%s'!\n", argv[2]);
	  fclose(f);
	  return 3;
	}
	fout2 = fopen(argv[3], "w");
	if (!fout2) {
	  fprintf(stderr, "Couldn't write on file '%s'!\n", argv[3]);
	  fclose(f);
	  fclose(fout);
	  return 3;
	}
	if (espessura==0.0) {
	  fprintf(stderr, "Invalid value to width!!!\n");
	  return 4;
	}
	if (corrente==0.0) {
	  fprintf(stderr, "Invalid value to current!!!\n");
	  return 5;
	}
	printf("Reading cdwrite file...\n");
	fconv_read_cdwrite(f);
	printf("Detecting border...\n");
	fconv_detect_cut();
	printf("Detecting center of material 2 elem (copo)...\n");
	fconv_detect_center_elem(2);

	printf("Writing output...");
	fconv_write_cdread (fout, corrente/espessura, tensao_terra, diam, 0);
	fconv_write_cdread_path (fout2);
	printf(" Done.\n");

	fclose(f);
	fclose(fout);
	fclose(fout2);
	return 0;
}









