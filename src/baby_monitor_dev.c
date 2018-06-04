/*
 ============================================================================
 Name        : baby_monitor_dev.c
 Author      : morgan venandy
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdlib.h>
#include <gtk-2.0/gtk/gtk.h>

int main(int argc, char **argv) {

	GtkWidget* MainWindow = NULL;

	gtk_init(&argc, &argv);

	MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

	/* Affichage et boucle évènementielle */
	gtk_widget_show(MainWindow);
	gtk_main();

	return 1;
}
