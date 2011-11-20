#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "main.h"
#include <fconv.h>
#include "tipo.h"

void
on_gerar1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  on_button3_clicked(NULL, NULL);
}


void
on_sair1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_main_quit();
}


gboolean
on_wndMain_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_main_quit();
  return FALSE;
}


//Botão aplicar
void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  GnomeFileEntry *file_entry1, *file_entry2, *file_entry3;  
  GnomeNumberEntry *n1, *n2, *n3, *n4, *n5, *n6, *n7;
  gchar *f1, *f2, *f3, f4[1024];
  gchar filename[1024];
  gdouble num1, num2;
  FILE *file1, *file2, *file3, *file4;
  int diam=0, sint=0;
  int i, j;
  double current=0, espessura=0, cond1=0, cond2=0;
  GtkToggleButton *btnDiam, *btndata, *btnextranode, *btnApplyScale;
  int with_data=1, with_extra_nodes=1, apply_scale=1;

  fconv_init();

#ifdef USO_INTERNO
  printf("versão uso interno\n");
#else
  printf("versão oficial\n");
#endif

  //       Esta função gera o arquivo .hm
  //Primeiro, leia os nomes dos arquivo e verifique que eles existem
  file_entry1 = (GnomeFileEntry *) lookup_widget(wndMain, "fileentry1");
  file_entry2 = (GnomeFileEntry *) lookup_widget(wndMain, "fileentry2");
  file_entry3 = (GnomeFileEntry *) lookup_widget(wndMain, "fileentry3");
  btnDiam     = (GtkToggleButton *) lookup_widget(wndMain, "btnDiam");
  btndata     = (GtkToggleButton *) lookup_widget(wndMain, "btnExpr");
  btnextranode     = (GtkToggleButton *) lookup_widget(wndMain, "btnExtraNode");
  btnApplyScale    = (GtkToggleButton *) lookup_widget(wndMain, "btnApplyScale");
  n1 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry1");//Escalas
  n2 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry2");
  n3 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry3");
  n4 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry4");
  n5 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry5");
  n6 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry6");
  n7 = (GnomeNumberEntry *) lookup_widget(wndMain, "numberentry7");

  f1 = gnome_file_entry_get_full_path(file_entry1, 1);  
  f2 = gnome_file_entry_get_full_path(file_entry2, 1);  
  f3 = gnome_file_entry_get_full_path(file_entry3, 0);
  if ((f1==NULL) || (f2==NULL) || (f3==NULL)) {
    printf("\a");
    gtk_widget_show(wndError);
    return;
  }
  strcpy(f4, f3);
  f4[strlen(f4)-2]='\0';
  strcat(f4, "cfg");
  diam               = btnDiam->active;
  with_data          = btndata->active;
  with_extra_nodes   = btnextranode->active;
  apply_scale        = btnApplyScale->active;
  num1 = gnome_number_entry_get_number (n1);
  num2 = gnome_number_entry_get_number (n2);
  current = -gnome_number_entry_get_number (n3);
  espessura = gnome_number_entry_get_number (n4);
  cond1 = gnome_number_entry_get_number (n5);
  cond2 = gnome_number_entry_get_number (n6);
  sint = gnome_number_entry_get_number (n7);

  //Open the files
  file1 = fopen(f1, "r");
  file2 = fopen(f2, "r");
  file3 = fopen(f3, "wb");
  file4 = fopen(f4, "wb");
  if ((!file1) || (!file2) || (!file3) || (!file4)) {
    printf("\a");
    gtk_widget_show(wndError);
    return;
  }
  /**********************************/
  //Read the file 1 - geometry
  fconv_read_cdwrite(file1);

  fconv_detect_center();
  if (apply_scale) {
    if ((num1==0) || (num2==0)) {
      printf("\a");
      return;
    }
    num1 = num1/(xmax-xmin);
    num2 = num2/(ymax-ymin);
  } else {
    num1 = num2 = 1;
  }
  printf("Escala: %f x %f\n", num1, num2);
  printf("Tamanho original (m): %f x %f\n", (xmax-xmin), (ymax-ymin));

  //Scale nodes
  fconv_scale_nos(num1, num2);
  
  if (!with_data) {
    for (i=0; i<num_elems; i++) {
      elems[i].prop = 1.0;
      elems[i].type = 5;
    }
    num_materials=2;
    materials[0].num=1;
    materials[1].num=2;
    materials[0].type=2;
    materials[1].type=2;
    materials[0].prop[0]=materials[0].prop[1]=materials[0].prop[2]=materials[0].prop[3]=0.0;
    materials[1].prop[0]=materials[1].prop[1]=materials[1].prop[2]=materials[1].prop[3]=0.0;
    materials[0].cond1=cond1;
    materials[0].cond2=cond1;
    materials[1].cond1=cond2;
    materials[1].cond2=0.0;
  } else {
    for (i=0; i<num_elems; i++) {
      elems[i].prop = 0.5;
      elems[i].type = 5;
    }
    num_materials=1;
    materials[0].num=1;
    materials[0].type=2;
    materials[0].prop[0]=materials[0].prop[1]=materials[0].prop[2]=materials[0].prop[3]=0.0;
    materials[0].cond1=cond1;
    materials[0].cond2=cond2;
    fconv_read_exprfile(file2);
  }

  //Write hm file
  fconv_detect_cut();

  if (with_extra_nodes)
    fconv_add_extra_node();

  fconv_write_hm(file3, diam, current/espessura, with_extra_nodes);

  
  //Write cfg file
  i=j=0;
  while (i<strlen(f3)) {
    if (f3[i]=='/') j=i+1;
    i++;
  }
  strcpy(filename, f3+j);
  filename[strlen(filename)-3]='\0';
  printf("'%s'\n", filename);
  if (with_data) {
    fprintf(file4, "$    *** Arquivo de Configuracao para analise piezoeletrica ***\ntipan 23\n");
    fprintf(file4, "file %s\n", filename);
    fprintf(file4, "opt tmin=1.0d-4\nopt tmax=1.0d0\nopt slp\nopt maxm1=0.4\nopt maxm2=0.2\nopt maxm3=0.05\npena 2.\n");
    fprintf(file4, "opt vcon=%d.\n", sint);
    fprintf(file4, "$read\ntext\n$patran\n");
  } else {
    fprintf(file4, "$    *** Arquivo de Configuracao para analise piezoeletrica ***\n");
    fprintf(file4, "tipan 2\nfile %s\n$read\ntext\n$patran\n", filename);
  }


  //Close the files
  fclose(file1);
  fclose(file2);
  fclose(file3);
  fclose(file4);
}

void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(wndError);
}


void
on_btnDiam_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkToggleButton *btnDiam;  
  btnDiam     = (GtkToggleButton *) lookup_widget(wndMain, "btnDiam");
  if (btnDiam->active) {
    gtk_label_set_text(GTK_LABEL((GTK_BIN(btnDiam))->child), "Diametral");
  } else {
    gtk_label_set_text(GTK_LABEL((GTK_BIN(btnDiam))->child), "Adjacente");
  }
}

