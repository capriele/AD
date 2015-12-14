#include <iostream>
#include <ctime>

#include <bot_core/bot_core.h>
#include <bot_vis/bot_vis.h>

#include <gtk/gtk.h>


#include <lcm/lcm.h>
#include <lcmtypes/lcmtypes.h>

//#include <GLUT/GLUT.h>
#include <GL/gl.h>


#include <cmath>
#include <glib.h>


#include "renderer_pose.h"



#define RENDERER_NAME "Pose"

using namespace std;

class RendererPose {

public:
  //Standard renderer variables
  BotRenderer renderer;
  BotGtkParamWidget *pw;
  BotViewer *viewer;

  // LCM messages
  lcm_t *lcm;
  agile_pose_t *pose_last;

  //Wavefront model
  BotWavefrontModel *wavefront_model;
  int display_lists_ready;
  GLuint wavefront_dl;

};

static void
pose_message_handler (const lcm_recv_buf_t *rbuf, const char *channel, const agile_pose_t *msg, void *user){

	RendererPose *self = (RendererPose *) user;

	if (self->pose_last != NULL) {
		agile_pose_t_destroy (self->pose_last);
	}	
	
	self->pose_last = agile_pose_t_copy (msg);
	
	cout << "Pose message received: " << msg->timestamp << endl; 

	bot_viewer_request_redraw (self->viewer);
}





static void
draw_wavefront_model (RendererPose * self) {
    
    glPushMatrix ();
    glEnable (GL_BLEND);
    glEnable (GL_RESCALE_NORMAL);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel (GL_SMOOTH);
    glEnable (GL_LIGHTING);
    glCallList (self->wavefront_dl);
    glPopMatrix ();
}

static GLuint
compile_display_list (RendererPose * self, BotWavefrontModel * model) {
    
    GLuint dl = glGenLists (1);
    glNewList (dl, GL_COMPILE);
    
    glEnable(GL_LIGHTING);
    bot_wavefront_model_gl_draw(model);
    glDisable(GL_LIGHTING);
    
    glEndList ();
    return dl;
}

static void 
renderer_pose_control_draw (BotViewer *viewer, BotRenderer *renderer) {

  RendererPose *self = (RendererPose*) renderer;
  
  glEnable (GL_DEPTH_TEST);
  
  if (!self->display_lists_ready) {
    if (self->wavefront_model)
      self->wavefront_dl = compile_display_list (self, self->wavefront_model);
    self->display_lists_ready = 1;
  }

  glPushMatrix();
  
  double rot_x = 0;
  double rot_y = 0;
  double rot_z = 0;
  double dx = 0.0;
  double dy = 0.0;
  double dz = 0.7;
  
  if (self->pose_last)
    {
      dx = self->pose_last->position[0];
      dy = self->pose_last->position[1];
      dz = self->pose_last->position[2];
      
      rot_x = 0;
      rot_y = 0;
      rot_z = 0;
    }
  
  
  glTranslated(dx, dy, dz);
  glRotatef(rot_z, 0, 0, 1);
  glRotatef(rot_y, 0, 1, 0);
  glRotatef(rot_x, 1, 0, 0);
  
  
  const double kModelRotX = 90.0;
  const double kModelRotY = 0.0;
  const double kModelRotZ = 0.0;
  const double kModelDX = -2.6;
  const double kModelDY = -0.9;
  const double kModelDZ = 0.0;
  const double kModelScale = 4.5 / 13.5;
  
  glTranslated(kModelDX, kModelDY, kModelDZ);
  glRotatef(kModelRotZ, 0, 0, 1);
  glRotatef(kModelRotY, 0, 1, 0);
  glRotatef(kModelRotX, 1, 0, 0);
  glScalef(kModelScale, kModelScale, kModelScale);
  
  
  if (self->display_lists_ready && self->wavefront_dl)
    draw_wavefront_model (self);
  
  glPopMatrix();
  
  
  return;
}






static void
renderer_pose_control_free (BotRenderer *renderer) {
	
	RendererPose *self = (RendererPose*) renderer;

	free(self);	

	return;
}





static void
on_param_widget_changed (BotGtkParamWidget *pw, const char *name, void *user) {
	
	RendererPose *self = (RendererPose*) self;
	
	return;
}





void add_pose_renderer_to_viewer (BotViewer *viewer, int render_priority, lcm_t *lcm) {
	
	// Setup renderer
	RendererPose *self = new RendererPose;
	BotRenderer *renderer = &self->renderer; 
	self->viewer = viewer;
	renderer->draw = renderer_pose_control_draw;
	renderer->destroy = renderer_pose_control_free;
	renderer->widget = bot_gtk_param_widget_new ();
	renderer->name = (char *) RENDERER_NAME;
	renderer->user = self;
	renderer->enabled = 1;
	self->pw = BOT_GTK_PARAM_WIDGET (renderer->widget);
	self->pose_last = NULL;	


	// Subscribe to lcm messages
	self->lcm = lcm;
	self->pose_last = NULL;
	agile_pose_t_subscribe (lcm, "POSE", &pose_message_handler, self);

	// Arrange the GTK interface
	gtk_widget_show_all (renderer->widget);
	g_signal_connect (G_OBJECT(self->pw), "changed", G_CALLBACK(on_param_widget_changed), self);
	on_param_widget_changed (self->pw, "", self);

	// Setup the wavefront model
	self->display_lists_ready = 0;
	self->wavefront_model = bot_wavefront_model_create("../../media/wavefront/lamborgini/car.obj");

	// Add the renderer to viewer
	bot_viewer_add_renderer (viewer, &self->renderer, render_priority);


}


