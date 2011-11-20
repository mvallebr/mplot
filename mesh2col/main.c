#include <fconv.h>
#include <mplot.h>

//This program reads a input file exported on ansys by the cdwrite command and writes a file containing a number of node by line. Each one of this nodes is a node on the border of the mesh
int main (int argc, char *argv[]) {
	FILE *f, *fhm, *fout;
	int shade;
	int i;

	//Init fconv library
	fconv_init();

	//open the input files
	if (argc<5) {
	  fprintf(stderr, "Usage: %s DENS_FILE HM_FILE OUTPUT_FILE plot(bool)\n", argv[0]);
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
	fout = fopen(argv[3], "w");
	if (!fout) {
	  fprintf(stderr, "Couldn't read file '%s'!\n", argv[3]);
	  fclose(fhm);
	  fclose(f);
	  return 4;
	}

	shade = atoi(argv[4]);
	//Read the files
	printf("Reading HM file...\n");
	fconv_read_hm(fhm);

	printf("Reading dens file...\n");
	fconv_read_dens(f);
	
	printf("Generating columns...\n");
	fconv_fill_columns_by_elements();

	for (i=0; i<fconv_num_columns; i++) {
	  fprintf(fout, "  %f  %f  %f\n", fconv_cx[i], fconv_cy[i], fconv_cz[i]);
	}

	//Bye bye
	fclose(f);
	fclose(fhm);
	fclose(fout);

	//Exit the main program
	return 0;
}









