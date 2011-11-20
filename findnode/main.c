#include <fconv.h>

//This program reads a input file exported on ansys by the cdwrite command and writes a file containing a number of node by line. Each one of this nodes is a node on the border of the mesh
int main (int argc, char *argv[]) {
	FILE *f, *fout;
	fconv_init();
	if (argc<3) {
	  fprintf(stderr, "Usage: %s CDWRITE_FILE FILE_OUTPUT\n", argv[0]);
	  return 1;
	}
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
	printf("Reading cdwrite file...\n");
	fconv_read_cdwrite(f);
	printf("Detecting border...\n");
	fconv_detect_cut();
	printf("Writing output...");
	fconv_write_cut_file (fout);
	printf(" Done.\n");

	fclose(f);
	fclose(fout);
	return 0;
}









