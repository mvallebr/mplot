#include <fconv.h>
#include <math.h>
#include <stdlib.h> 
#include <string.h>

int fconv_scale_nos(double xscale, double yscale) {
  int i;

  fconv_detect_center();
  //Do by center
  //for (i=0; i<num_nodes; i++) {
  //nos[i].x = xcenter + ( (nos[i].x - xcenter)*xscale );
  // nos[i].y = ycenter + ( (nos[i].y - ycenter)*yscale );
  //}
  //Do by (0,0)
  for (i=0; i<num_nodes; i++) {
    nos[i].x = ( (nos[i].x )*xscale );
    nos[i].y = ( (nos[i].y )*yscale );
  }

  return 1;
}

int fconv_detect_center(void) {
  int actual;
  xmin=ymin=100000.0;
  xmax=ymax=-100000.0;
  actual=0;
  while(actual<num_nodes) {
    if ((nos[actual].x)<xmin) xmin= (nos[actual].x);
    if ((nos[actual].x)>xmax) xmax= (nos[actual].x);
    if ((nos[actual].y)<ymin) ymin= (nos[actual].x);
    if ((nos[actual].y)>ymax) ymax= (nos[actual].x);
    actual++;
  }
  xcenter = (xmin+xmax)/2;
  ycenter = (ymin+ymax)/2;
  
  //  printf("xcenter=%f ycenter=%f\n", xcenter, ycenter);
  return 1;
}

 
void fconv_init(void) {
  num_nodes=num_elems=num_materials=fconv_num_columns=num_expr_data=0;
}

int  fconv_write_cut_file  (FILE *f) {
  int i;
  fprintf(f, "#%d eletrodos:\n", num_eletr);
  for (i=0; i<num_eletr; i++) {
    fprintf(f, "%d \n", nos[eletr[i]].num);
  }
  //  fprintf("\n");     
  return 1;
}

int  fconv_detect_cut  () {
  int i, j, k, detected;
  int node;
  int  id_cut_nodes[10000];
  int num_cnodes=0;
  int detected1, detected2;
  int first, prior;
  double dist=0, dist2=0, olddist=0;

  fconv_detect_center();
  
  for (i=0; i<num_nodes; i++) {
    detected = 0;
    for (j=0; j<num_elems; j++) {
      if  ( (nos[i].num==elems[j].p[0]) || (nos[i].num==elems[j].p[1]) ||
	    (nos[i].num==elems[j].p[2]) || (nos[i].num==elems[j].p[3]) ) {
	detected++;
      }
    }
    if (detected==2) { //Is a border node
      //printf("Detected node: %d", nos[i].num);
      id_cut_nodes[num_cnodes] = i;
      num_cnodes++;
    }    
  }

  //Now, detect sequence of nodes
  num_cut_nodes=0;
  //First detect nodes more to right and with y>=0
  node=id_cut_nodes[0];
  for (i=0; i<num_cnodes; i++) 
    if (nos[id_cut_nodes[i]].y>=0) 
      if  ( (nos[id_cut_nodes[i]].x)>nos[node].x) {
	node = id_cut_nodes[i];
      }
  first = node;
  cut_nodes[num_cut_nodes] = node;
  num_cut_nodes++;
  //  printf("nos[node].num=%d node=%d id_cut_nodes[0]=%d\n", nos[node].num, node, id_cut_nodes[0]);
  //Find the elements that contains this node
  detected1=detected2=-1;
  for (j=0; j<num_elems; j++) {
    if  ( (nos[node].num==elems[j].p[0]) || (nos[node].num==elems[j].p[1]) ||
	  (nos[node].num==elems[j].p[2]) || (nos[node].num==elems[j].p[3]) ) {
      //      printf("Pertence ao elemento %d (nós %d %d %d %d)\n", elems[j].num, elems[j].p[0], elems[j].p[1], 
      //	     elems[j].p[2], elems[j].p[3]);
      for (i=0; i<4; i++) {
	if (nos[node].num!=elems[j].p[i]){
	  for (k=0; k<num_cnodes; k++) {
	    if (nos[id_cut_nodes[k]].num==elems[j].p[i]) {
	      //printf("Nó %d ligado pelo elemento %d ao nó %d\n", nos[node].num, elems[j].num, elems[j].p[i]);
	      if (detected1<0) detected1=id_cut_nodes[k];
	      else detected2=id_cut_nodes[k];
	    }
	  }
	}
      }
    }
  }
  if ( (detected1<0) || (detected2<0) ) {
    fprintf(stderr, "Error on detecting border!\n");
    return 0;
  }
  prior=nos[node].num;
  if (nos[detected1].y>nos[detected2].y) node = detected1;
  else node = detected2;
  cut_nodes[num_cut_nodes] = node;
  num_cut_nodes++;
  do {
    detected1=detected2=-1;
    for (j=0; j<num_elems; j++) {
      if  ( (nos[node].num==elems[j].p[0]) || (nos[node].num==elems[j].p[1]) ||
	    (nos[node].num==elems[j].p[2]) || (nos[node].num==elems[j].p[3]) ) {
	for (i=0; i<4; i++) {
	  if (nos[node].num!=elems[j].p[i]){
	    for (k=0; k<num_cnodes; k++) {
	      if (nos[id_cut_nodes[k]].num==elems[j].p[i]) {
		//printf("Nó %d ligado pelo elemento %d ao nó %d\n", nos[node].num, elems[j].num, elems[j].p[i]);
		if (detected1<0) detected1=id_cut_nodes[k];
		else detected2=id_cut_nodes[k];
	      }
	    }
	  }
	}
      }
    }    
    if ( (detected1<0) || (detected2<0) ) {
      fprintf(stderr, "Error on detecting border!\n");
      return 0;
    }
    k=node;
    if (nos[detected1].num!=prior) node = detected1;
    else node = detected2;
    prior=nos[k].num;
    if (node!=first) {
      cut_nodes[num_cut_nodes] = node;
      num_cut_nodes++;
    }
  } while(node!=first);


  printf("%d nodes\n", num_cut_nodes);
  for (i=0; i<num_cut_nodes; i++) {
    printf("%d \t", nos[cut_nodes[i]].num);
  }
  printf("\n");   
  
  dist_beetwen_eletr=0;
  num_eletr=0;
  //Detect distance beetwen the eletr
  for (i=0; i<num_cut_nodes-1; i++) {
    dist_beetwen_eletr+=hypot(nos[cut_nodes[i+1]].x - nos[cut_nodes[i]].x, 
			      nos[cut_nodes[i+1]].y - nos[cut_nodes[i]].y);		   
    //    printf("nos[cut_nodes[%d]].num=%d até %d dist=%f (%f %f) até (%f %f)\n", i, nos[cut_nodes[i]].num, 
    //	   nos[cut_nodes[i+1]].num, 
    //	   dist_beetwen_eletr, nos[cut_nodes[i]].x   , nos[cut_nodes[i]].y,
    //			      nos[cut_nodes[i+1]].x , nos[cut_nodes[i+1]].y);
  }
  //  dist_beetwen_eletr /=  NUM_ELETR;
  printf("distance beetwen eletrs: %f\n", dist_beetwen_eletr);
  j=0;
  eletr[num_eletr]=cut_nodes[j]; //Add the first eletr
  num_eletr++;
  dist2=0;
  for (i=1; i<NUM_ELETR; i++) {
    dist=i*dist_beetwen_eletr/NUM_ELETR;
    while (dist2 < dist) {
      olddist=dist2;
      dist2+=hypot(nos[cut_nodes[j+1]].x-nos[cut_nodes[j]].x, 
		  nos[cut_nodes[j+1]].y-nos[cut_nodes[j]].y
		  );
      //            printf("Beetwen %d and %d: %f \n", nos[cut_nodes[j]].num, nos[cut_nodes[j+1]].num, dist2);
      if (j+1>num_cut_nodes) {
	//eletr[num_eletr]=cut_nodes[j];
	//num_eletr++;
	break;
      }
      j++;
    }
    eletr[num_eletr]= (dist2-dist<dist-olddist) 
      ? cut_nodes[j] 
      : cut_nodes[j-1];
    //printf("distance: %f %f detected: %d\n", dist, dist2, nos[eletr[num_eletr]].num);
    //    dist=(dist-dist_beetwen_eletr>dist_beetwen_eletr-olddist) 
    //      ? dist-dist_beetwen_eletr 
    //      : dist_beetwen_eletr-olddist;    
    num_eletr++;    
  }

  return 1;
}

int  fconv_detect_center_elem   (int material) {
  int i, j, k;
  double lx, ly;
  int num_nodes_mat=0;
  celemx=celemy=0.0;
  for (i=0; i<num_elems; i++) {
    if (elems[i].material==material) { //The material matchs      
      lx=0.0; ly=0.0;
      for (j=0; j<num_nodes; j++) 
	for (k=0; k<4; k++)
	  if (nos[j].num==elems[i].p[k]){
	    lx+=nos[j].x;
	    ly+=nos[j].y;
	    //	    printf("%f %f %f %f\n", nos[j].x, nos[j].y, lx, ly);
	  }     
      celemx+=(lx/4);
      celemy+=(ly/4); 
      num_nodes_mat++;
    }
  }
  celemx/=num_nodes_mat;
  celemy/=num_nodes_mat; 
  num_nodes_mat++;
  printf("center of element for material %d: %f %f\n", material, celemx, celemy);
  return 1;
}

int  fconv_write_cdread  (FILE *f, double current, double voltage, int diam, int skip) {
  //Writes cdread style ansys input file
  int i, j;
  for (i=0; i<num_eletr; i+=skip+1) {
    if (diam) j = (i+(num_eletr/2)) % (num_eletr);
    else j=i+1;
    fprintf(f, "F,%d,AMPS,%f,\n", nos[eletr[i]].num, current);
    fprintf(f, "D,%d,VOLT,%f,\n", nos[eletr[j]].num, voltage);
    fprintf(f, "LSWRITE\n");
    fprintf(f, "LSCLEAR,FE\n");
  }
  return 1;
}
int  fconv_write_cdread_path  (FILE *f) {
  int i, j;
  for (i=0; i<num_eletr; i+=1) {
    fprintf(f, "SET, %d\n", i+1);
    fprintf(f, "PATH, CAMINHO%d, 32, 1, 1\n", i+1);
    for (j=0; j<num_eletr; j+=1) {
      fprintf(f, "PPATH, %d, %d\n", j+1, nos[eletr[j]].num);
    }
    fprintf(f, "PDEF, DEF%d, VOLT\n", i+1);
    fprintf(f, "PRPATH, DEF%d\n", i+1);
  }
  return 1;
}

//Read the cond. from dens file:
// cons = c1 + dens*dens*(c2-c1)
// where c1 and c2 and material cond.
int  fconv_read_dens  (FILE *f) {
  char buffer[1000];
  double d;
  int num_elem;
  int num_read=1;
  elem *e;
  material *m;
  num_elem=1;
  //Skip first line
  fgets(buffer, sizeof(buffer), f);
  //Read dens
  while ( (!(feof(f))) && (num_read>0) ){
    num_read=fscanf(f, "%lf ", &d);
    //    printf("%d %d %f\n", num_read, num_elem, d);
    if (num_read==1) {
      e = find_elem(num_elem);
      if (e==NULL) {
	printf("Error reading dens: element %d not found!!\n", num_elem);
	return 0;
      }
      m = find_material(e->material);
      if (m==NULL) {
	printf("Error reading dens: material %d of element %d not found!!\n", e->material, e->num);
	return 0;
      }
      e->has_cond = 1;
      e->cond = m->cond1 + d*d*(m->cond2 - m->cond1);
      num_elem++;
    }
  }  
  return 1;
}
  
no  *find_node(int number) {
  int i;
  for (i=0; i<num_nodes; i++) {
    if (nos[i].num==number)
      return &(nos[i]);
  }
  return NULL;
}

elem *find_elem(int number) {
  int i;
  for (i=0; i<num_elems; i++) {
    if (elems[i].num==number)
      return &(elems[i]);
  }
  return NULL;
}

material *find_material(int number) {
  int i;
  for (i=0; i<num_materials; i++) {
    if (materials[i].num==number)
      return &(materials[i]);
  }
  return NULL;
}

int  fconv_fill_columns_by_elements() {
  int i;
  double x, y;
  printf("num_e=%d num_n=%d\n", num_elems, num_nodes);
  for (i=0; i<num_elems; i++) {
    //    printf("e%d - has_cond=%d\n", i, elems[i].has_cond);
    if (elems[i].has_cond) {
      //Add a columns
      if (!fconv_elem_center(elems[i], &x, &y)) return 0;
      fconv_cx[fconv_num_columns] = (float) x;
      fconv_cy[fconv_num_columns] = (float) y;
      fconv_cz[fconv_num_columns] = (float) (elems[i].cond);   
      //      printf("%f\t",  (elems[i].cond));
      fconv_num_columns++;
    }
  }

  return 1;
}

int fconv_elem_center(elem e, double *x, double *y) {
  no *n[4];
  int i;
  for (i=0; i<4; i++) {
    n[i] = find_node(e.p[i]);
    if (n[i]==NULL) {
      fprintf(stderr, "Node not found: %d\n", e.p[i]);
      return 0;
    }
  }
  *x = (n[0]->x + n[1]->x + n[2]->x + n[3]->x);
  *y = (n[0]->y + n[1]->y + n[2]->y + n[3]->y);

  return 1;
}

//Add an extra node above the eletrode
int fconv_add_extra_node() {
  int i, j, cutnode1=0, cutnode2=0;  
  double x, y;
  fconv_detect_center();
  for (i=0; i<num_eletr; i++) {
    //Find index on list of border nodes
    for (j=0; j<num_cut_nodes; j++){
      if (nos[cut_nodes[j]].num==nos[eletr[i]].num){
	cutnode1=(j+1) % num_cut_nodes;
	cutnode2=(j-1);
	if (cutnode2<0) cutnode2+=num_cut_nodes;
	break;
      }
    }
    //Add node
    nos[num_nodes].num=fconv_last_node_num() +1;
    //    printf("%d \t", nos[num_nodes].num);
    x = nos[eletr[i]].x;
    y = nos[eletr[i]].y;
    x += (x-xcenter>0) ? 0.01 : -0.01;
    y += (y-ycenter>0) ? 0.01 : -0.01;
    nos[num_nodes].x=x;
    nos[num_nodes].y=y;
    //Add element
    elems[num_elems].num = fconv_last_elem_num() +1;
    elems[num_elems].material = 1;
    elems[num_elems].type = 3;
    elems[num_elems].has_cond = 0;
    elems[num_elems].cond = 0;
    elems[num_elems].prop = 0.5;
    elems[num_elems].p[0]=nos[cut_nodes[cutnode1]].num;
    elems[num_elems].p[1]=nos[eletr[i]].num;
    elems[num_elems].p[2]=nos[cut_nodes[cutnode2]].num;
    elems[num_elems].p[3]=nos[num_nodes].num;
    //Update size of vectors
    num_nodes++;
    num_elems++;
  }
  for (j=0; j<num_eletr; j++){
    eletr[j] = num_nodes - 32 + j;
  }
  return 1;
}

//Read file witch contains the experimental data
int fconv_read_exprfile(FILE *f) {
  char buffer2[10240];
  char *buffer;
  int i=0, num_read=0;
  num_expr_data = 0;
  while (!(feof(f))) {
    fgets(buffer2, sizeof(buffer2), f);
    i=0;
    while ((buffer2[i]<33) && (buffer2[i]!='\0') ) i++;//Find the first char "not space"
    buffer= buffer2+i;
    //   printf("'%s'",buffer);
    fflush(stdout);
    if ( (!(buffer[0]=='%')) && (strncmp(buffer, "padrao", 6)) ) {//Not a comment
      num_read=sscanf(buffer, " %d %lf ", &i, &(expr_data[num_expr_data]));
      if (num_read==2) num_expr_data++;
      //  printf ("%d %f\n", i, expr_data[num_expr_data-1]);
    }
  }
  return 1;
}

int fconv_last_node_num() {
  int i, j=-1;
  for (i=0; i<num_nodes; i++) 
    if (nos[i].num>j) j= nos[i].num;
  return j;
}

int fconv_last_elem_num() {
  int i, j=-1;
  for (i=0; i<num_elems; i++) 
    if (elems[i].num>j) j= elems[i].num;
  return j;
}
