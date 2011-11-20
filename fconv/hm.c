#include <fconv.h>
#include <stdlib.h> 
#include <string.h>

int fconv_write_hm(FILE *f, int diam, double current, int comment) {
  int i;
  //Print header
  fprintf(f, "%s", "modelo: PATRAN\n");
  if (diam) 
    fprintf(f, "$Diametral\n");
  else
    fprintf(f, "$adjacentes\n");
  fprintf(f, "nome PATRAN Neutral, Hype\n");
  fprintf(f, "nelems   %d $ number of elements\n", num_elems);
  fprintf(f, "nnos     %d $ number of nodes\n", num_nodes);
  fprintf(f, "nmat     %d $ number of materials\n", num_materials);
  fprintf(f, "ndim   2 $ number of dimensions\nsymm  0  $ symmetry\n$ condicoes de contorno\n");
  fprintf(f, "Caso Irms\nnlc  32\nnter  32\nncor  32\nnmed  32\nnpre    0\nnequ   0\n");

  //Print nodes
  fprintf(f, "$ coordenadas dos nos\n");
  for (i=0; i<num_nodes; i++) {
    if ((comment) && (i==num_nodes-32))
      fprintf(f, "$ Nós adicionais:\n");
    fprintf(f, "no   %3d     %3.7f     %3.7f     %3.7f\n", nos[i].num, nos[i].x, nos[i].y, 0.0);
  }
  //Print elements
  fprintf(f, "$ Propr. dos elementos : numero, tipo, material,prop. real\n");
  for (i=0; i<num_elems; i++) {
    if ((comment) && (i==num_elems-32))
      fprintf(f, "$ Elementos adicionais:\n");
    fprintf(f, "elem   %3d  %d     %d %.5f     %d     %d     %d     %d\n", elems[i].num, 
	    elems[i].type, elems[i].material,
	    elems[i].prop, elems[i].p[0], elems[i].p[1], 
	    elems[i].p[2], elems[i].p[3]);
  }
  //Print Materials
  fprintf(f, "$ materiais, numero tipo e props\n");
  for (i=0; i<num_materials; i++) 
    fprintf(f, "diel,  %d  %d  %f  %f  %.1f  %.1f  %.1f  %.1f\n", materials[i].num, 
	    materials[i].type, 
	    materials[i].cond1, materials[i].cond2, 
	    materials[i].prop[0], materials[i].prop[1],
	    materials[i].prop[2], materials[i].prop[3] );
  
  //Print load cases (voltage and current)
  fprintf(f, "$%d eletrodos:\n$ Deslocamentos Prescritos\n$ no  dir  valor\n$ terra\n", num_eletr);
  for (i=0; i<num_eletr; i++) {
    fprintf(f, "terra     %d       0.       %d\n", nos[eletr[i]].num, i+1);
  }
  for (i=0; i<num_eletr; i++) {
    fprintf(f, "$Load case %d\n", i+1);
    if (diam)
      fprintf(f, "crnte   %d    %f    %d\n", nos[eletr[(i+(num_eletr/2))%num_eletr]].num, current, i+1);
    else
      fprintf(f, "crnte   %d    %f    %d\n", nos[eletr[(i+1)%num_eletr]].num, current, i+1);
  }
  //Print measures
  fprintf(f, "$Nós de medição\n");
  for (i=0; i<num_eletr; i++) {
    fprintf(f, "medid %d\n", nos[eletr[i]].num);
  }


  //Print expr data
  if (num_expr_data>0)
    fprintf(f, "exper\n");

  for (i=0; i<num_expr_data; i++) {
    if ((i%32)==0) {
      fprintf(f, "$ Set %d\n", (int)(i/32)+1);
    }
    fprintf(f, "    %d     %f\n", nos[eletr[i%32]].num, expr_data[i]);
  }

  fprintf(f, "FIM $Final do arquivo\n");

  return 1;
}

//This function reads the data (nodes, materials, elems) from an hm file
int  fconv_read_hm  (FILE *f) {
  char buffer2[1000], *buffer;
  double temp;
  int num_read, i;
  num_elems=num_nodes=num_materials=0;

  while (!(feof(f))) {
    fgets(buffer2, sizeof(buffer2), f);
    i=0;
    while (buffer2[i]<33) i++;//Find the first char "not space"
    buffer= buffer2+i;
    //printf("%d %d <=>'%s'", buffer[0], strncmp(buffer, "no ", 3), buffer);
    if (!strncmp(buffer, "elem", 4)) { //Read and add an element
      num_read=sscanf(buffer, "elem %d %d %d %lf %d %d %d %d ", &(elems[num_elems].num), &(elems[num_elems].type),
	     &(elems[num_elems].material), &(elems[num_elems].prop),
	     &(elems[num_elems].p[0]), &(elems[num_elems].p[1]),
	     &(elems[num_elems].p[2]), &(elems[num_elems].p[3])
	     );      
      elems[num_elems].has_cond = 0;
      elems[num_elems].cond = 0;
      //      printf("Reading element number %d (num_read=%d)\n'%s'", elems[num_elems].num, num_read, buffer);
      num_elems++;
    } else if (!strncmp(buffer, "no", 2)) { //Read and add a node
      sscanf(buffer, "no %d %lf %lf %lf  ", (&nos[num_nodes].num), 
	     (&nos[num_nodes].x), (&nos[num_nodes].y), &temp
	     );               
      //      printf("(%d %f %f)\n", nos[num_nodes].num, nos[num_nodes].x, nos[num_nodes].y);
      num_nodes++;
    } else if (!strncmp(buffer, "diel,", 5)) { //Read and add an material
      sscanf(buffer, "diel, %d %d %lf %lf %lf %lf %lf %lf ", (&materials[num_materials].num), 
	     (&materials[num_materials].type), 
	     (&materials[num_materials].cond1), (&materials[num_materials].cond2), 
	     (&materials[num_materials].prop[0]), (&materials[num_materials].prop[1]), 
	     (&materials[num_materials].prop[2]), (&materials[num_materials].prop[3])
	     );            
      num_materials++;
    }
  }
  printf("num_elems=%d, num_nodes=%d, num_materials=%d\n", num_elems, num_nodes, num_materials);
  return 1;  
}
