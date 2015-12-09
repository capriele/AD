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
	bot_glib_mainloop_attach_lcm (lcm);
	
		
	
	// Setup renderers 
	bot_viewer_add_stock_renderer (viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
	add_pose_renderer_to_viewer (viewer, 1, lcm);
	
	// Run the main loop
	gtk_main ();


	// Cleanup
	// TOOD

	return 0;
}

