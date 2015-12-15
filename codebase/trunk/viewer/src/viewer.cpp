#include <iostream>
#include <ctime>

#include <bot_core/bot_core.h>
#include <bot_vis/bot_vis.h>

#include <GL/gl.h>

#include <renderers/renderer_pose.h>


using namespace std; 



int main (int argc, char* argv[]) {

	// Initialize Gtk and GL
	gtk_init (&argc, &argv);
	glutInit (&argc, argv);

	//setlinebuff (stdout);

	// Create LCM
	lcm_t* lcm = lcm_create (NULL);

	BotViewer* viewer = bot_viewer_new ("Viewer");
        bot_gtk_quit_on_interrupt();
	bot_glib_mainloop_attach_lcm (lcm);
	

	
	// Setup renderers 
	bot_viewer_add_stock_renderer (viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
	add_pose_renderer_to_viewer (viewer, 1, lcm,"Pose - simulated","stateVariancesSim");
	add_pose_renderer_to_viewer (viewer, 1, lcm,"Pose - estimatedV1","stateVariancesOrientV1");
	add_pose_renderer_to_viewer (viewer, 1, lcm,"Pose - estimatedCF","stateVariancesOrientCF");

	  GtkWidget * window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	    g_signal_connect( G_OBJECT(window), "delete_event", gtk_main_quit, NULL );
	    g_signal_connect( G_OBJECT(window), "destroy", gtk_main_quit, NULL );


	//save params
	 char *fname = g_build_filename(g_get_user_config_dir(), ".wavefront-viewerrc", NULL);
    	 bot_viewer_load_preferences(viewer, fname);	//@TODO does not work

	
	// Run the main loop
	gtk_main ();


	// Cleanup
	printf("cleanup viewer, pref-path: %s!\n",fname);
	bot_viewer_save_preferences(viewer, fname);

    	bot_viewer_unref(viewer);

	return 0;
}

