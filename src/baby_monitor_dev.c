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

void onClickMute(GtkWidget *widget, gpointer data)
{
	gboolean EtatButton;
	EtatButton = gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(widget));
	gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(widget), (EtatButton ^ TRUE));
	if(!EtatButton) {
		gtk_button_set_label(GTK_BUTTON(widget), "Unmute");
	}
	else {
		gtk_button_set_label(GTK_BUTTON(widget), "Mute");
	}
}
void onClickUp(GtkWidget *widget, gpointer data)
{
	//gtk_image_set_from_file();
}

void onClickDown(GtkWidget *widget, gpointer data)
{

}

int main(int argc, char **argv)
{
    /* Variables */

	gchar *VideoURI = URI_VIDEO;

    GtkWidget *MainWindow = NULL,
    			*MainTable = NULL,
    			*ButtonSound[3],
    			*ButtonPower = NULL,
				*VideoWindow = NULL,
				*VUImage[10],
				*VolumeImage[10];

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
    MainTable = gtk_table_new(20, 20, TRUE);
    gtk_container_add(GTK_CONTAINER(MainWindow), GTK_WIDGET(MainTable));

    /* Création des boutons */
    ButtonSound[0] = gtk_button_new_with_label("+");
    ButtonSound[1] = gtk_toggle_button_new_with_label("Mute");
    ButtonSound[2] = gtk_button_new_with_label("-");

    ButtonPower = gtk_button_new_with_label("OFF");

    int LeftAttach = 0;
    int RightAttach = 0;
    /* Création des Images */
    for(int i=0; i<10; i++) {
    	VUImage[i] = gtk_image_new_from_file("/home/morgan/testImage.png");
    	LeftAttach = 2+i;
    	RightAttach = 3+i;
    	gtk_table_attach_defaults(GTK_TABLE(MainTable), VUImage[i], LeftAttach, RightAttach, 17, 19);
    }

    int TopAttach = 0;
    int BottomAttach = 0;
    for(int i=0; i<10; i++){
    	VolumeImage[i] = gtk_image_new_from_file("/home/morgan/testImageVolume.png");
    	TopAttach = 3+i;
		BottomAttach = 4+i;
		gtk_table_attach_defaults(GTK_TABLE(MainTable), VolumeImage[i], 18, 19, TopAttach, BottomAttach);
    }

    /* Insertion VLC */
    VideoWindow = gtk_drawing_area_new();
    gtk_table_attach_defaults(GTK_TABLE(MainTable), VideoWindow, 1, 17, 1, 13);

    VlcInst = libvlc_new(0, NULL);
    MediaPlayer = libvlc_media_player_new(VlcInst);
    g_signal_connect(G_OBJECT(VideoWindow), "realize", G_CALLBACK(playerOnRealize), NULL);

    VideoMedia = libvlc_media_new_location(VlcInst, VideoURI);
	libvlc_media_player_set_media(MediaPlayer, VideoMedia);
	libvlc_media_player_play(MediaPlayer);
	//g_free(VideoURI);

    /* Insertion des boutons */
    gtk_table_attach_defaults(GTK_TABLE(MainTable), ButtonPower, 18, 19, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(MainTable), ButtonSound[0], 18, 19, 13, 15);
    gtk_table_attach_defaults(GTK_TABLE(MainTable), ButtonSound[1], 18, 19, 15, 17);
    gtk_table_attach_defaults(GTK_TABLE(MainTable), ButtonSound[2], 18, 19, 17, 19);

    /* Création des signaux */
    g_signal_connect(G_OBJECT(MainWindow), "destroy", G_CALLBACK(onDestroy), NULL);
    g_signal_connect(G_OBJECT(ButtonSound[1]), "clicked", G_CALLBACK(onClickMute), ButtonSound[1]);
    g_signal_connect(G_OBJECT(ButtonSound[0]), "clicked", G_CALLBACK(onClickUp), VolumeImage);
    g_signal_connect(G_OBJECT(ButtonSound[2]), "clicked", G_CALLBACK(onClickDown), VolumeImage);


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
