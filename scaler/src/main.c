 /*
 * Arquivo main.c inicial gerado pelo Glade. Editar como requisitado.
 * Glade não irá sobregravar este arquivo.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#include "interface.h"
#include "support.h"
#include "main.h"
#include <fconv.h>
#include "tipo.h"

int
main (int argc, char *argv[])
{
#ifndef USO_INTERNO
  GtkWidget *v1, *v2, *v3;
#endif
#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);
#endif

  gnome_init ("ansys2hm", VERSION, argc, argv);

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  fconv_init();

  wndMain = create_wndMain ();
  gtk_widget_show (wndMain);
  wndError = create_wndError ();

#ifndef USO_INTERNO
  v1 = lookup_widget(wndMain, "vbox_geom");
  v2 = lookup_widget(wndMain, "vbox_options");
  v3 = lookup_widget(wndMain, "vbox_output");
  gtk_widget_hide(v1);
  gtk_widget_hide(v2);
  gtk_widget_hide(v3);
#endif

  gtk_main ();
  return 0;
}




