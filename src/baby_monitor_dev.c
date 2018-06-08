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
#include <signal.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <vlc/vlc.h>

#define URI_VIDEO "file:///home/morgan/test.mp4"

libvlc_media_player_t *MediaPlayer;
libvlc_instance_t *VlcInst;

void onDestroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

void playerOnRealize(GtkWidget *widget, gpointer data)
{
    libvlc_media_player_set_xwindow(MediaPlayer, GDK_WINDOW_XID(gtk_widget_get_window(widget)));
}

int main(int argc, char **argv)
{
    /* Variables */

	gchar *VideoURI = URI_VIDEO;

    GtkWidget *MainWindow = NULL,
    			*MainTable = NULL,
    			*ButtonSound[3],
    			*ButtonPower = NULL,
				*VideoWindow = NULL;

    sigset_t set;
    signal(SIGCHLD, SIG_DFL);
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    libvlc_media_t *VideoMedia;

    /* Initialisation de GTK+ */
    gtk_init(&argc, &argv);

    /* Création de la fenêtre */
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), 480, 320);

    /* Création des tables */
    MainTable = gtk_table_new(8, 10, TRUE);
    gtk_container_add(GTK_CONTAINER(MainWindow), GTK_WIDGET(MainTable));

    /* Création des boutons */
    ButtonSound[0] = gtk_button_new_with_label("+");
    ButtonSound[1] = gtk_button_new_with_label("Mute");
    ButtonSound[2] = gtk_button_new_with_label("-");

    ButtonPower = gtk_button_new_with_label("OFF");

    /* Insertion VLC */
    VideoWindow = gtk_drawing_area_new();
    gtk_table_attach_defaults(GTK_TABLE(MainTable), VideoWindow, 1, 6, 1, 6);
    //gtk_container_add(GTK_CONTAINER(MainWindow), VideoWindow);

    VlcInst = libvlc_new(0, NULL);
    MediaPlayer = libvlc_media_player_new(VlcInst);
    g_signal_connect(G_OBJECT(VideoWindow), "realize", G_CALLBACK(playerOnRealize), NULL);

    VideoMedia = libvlc_media_new_location(VlcInst, VideoURI);
	libvlc_media_player_set_media(MediaPlayer, VideoMedia);
	libvlc_media_player_play(MediaPlayer);
	//g_free(VideoURI);

    /* Insertion des boutons */
    gtk_table_attach(GTK_TABLE(MainTable), ButtonPower, 8, 9, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(MainTable), ButtonSound[0], 8, 9, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(MainTable), ButtonSound[1], 8, 9, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);
    gtk_table_attach(GTK_TABLE(MainTable), ButtonSound[2], 8, 9, 5, 6, GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0, 0);


    /* Création des signaux */
    g_signal_connect(G_OBJECT(MainWindow), "destroy", G_CALLBACK(onDestroy), NULL);

    /* Intégration VLC */

    /* Affichage et boucle événementielle */
    gtk_widget_show_all(MainWindow);
    gtk_main();

    /* Fermeture de GTK+ */
    g_free(VideoURI);
    libvlc_media_release(VideoMedia);
    libvlc_media_player_release(MediaPlayer);
    libvlc_release(VlcInst);
    gtk_exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}
