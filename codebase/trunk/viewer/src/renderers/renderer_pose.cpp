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



//#define RENDERER_NAME "Pose - simulated"

using namespace std;

class RendererPose
{

public:
    //Standard renderer variables
    BotRenderer renderer;
    BotGtkParamWidget *pw;
    BotViewer *viewer;

    // LCM messages
    lcm_t *lcm;
    agile_pose_t *pose_last;
    agile_stateVariances_t *stateVariances_last;

    //Wavefront model
    BotWavefrontModel *wavefront_model;
    int display_lists_ready;
    GLuint wavefront_dl;

    //name-flexible widgets
    const char* widget_posenable_name = new char[80];
    const char* widget_xoff_name = new char[80];
    const char* widget_yoff_name = new char[80];
    const char* widget_zoff_name = new char[80];

};

void get_widgetspec_name(const char* widgetname, const char* uielement, const char* renderer_name)
{
    char* tmp = (char*)widgetname;
    sprintf(tmp, "%s: %s", uielement, renderer_name);
}

void quat2Euler(double* q, double *yaw, double *pitch, double *roll)
{
    *roll = atan2(2 * (q[0] * q[1] + q[2] * q[3]), (1 - 2 * (pow(q[1], 2) + pow(q[2], 2))));
    *pitch = asin(2 * (q[0] * q[2] - q[3] * q[1]));
    *yaw = atan2(2 * (q[0] * q[3] + q[1] * q[2]), (1 - 2 * (pow(q[2], 2) + pow(q[3], 2))));
}

static void Euler2quat(double* q, double *yaw, double *pitch, double *roll)
{
    // Assuming the angles are in radians.
    double cy = cos(*yaw / 2);
    double sy = sin(*yaw / 2);
    double cp = cos(*pitch / 2);
    double sp = sin(*pitch / 2);
    double cr = cos(*roll / 2);
    double sr = sin(*roll / 2);

    q[0] = cy * cp * cr + sy * sp * sr;
    q[1] = sr * cp * cy - cr * sp * sy;
    q[2] = cr * sp * cy + sr * cp * sy;
    q[3] = cr * cp * sy - sr * sp * cy;
}

static void
pose_message_handler(const lcm_recv_buf_t *rbuf, const char *channel, const agile_pose_t *msg, void *user)
{

    RendererPose *self = (RendererPose *) user;

    if(self->pose_last != NULL)
    {
        agile_pose_t_destroy(self->pose_last);
    }

    self->pose_last = agile_pose_t_copy(msg);

    //cout << "Pose message received: " << msg->timestampJetson << endl;

    bot_viewer_request_redraw(self->viewer);
}

static void
stateVariances_message_handler(const lcm_recv_buf_t *rbuf, const char *channel, const agile_stateVariances_t *msg, void *user)
{

    RendererPose *self = (RendererPose *) user;

    if(self->stateVariances_last != NULL)
    {
        agile_stateVariances_t_destroy(self->stateVariances_last);
    }

    self->stateVariances_last = agile_stateVariances_t_copy(msg);

    //cout << "stateVariances message received: " << msg->timestampJetson << endl;

    bot_viewer_request_redraw(self->viewer);
}





static void
draw_wavefront_model(RendererPose * self)
{

    glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_RESCALE_NORMAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glCallList(self->wavefront_dl);
    glPopMatrix();
}

static GLuint
compile_display_list(RendererPose * self, BotWavefrontModel * model)
{

    GLuint dl = glGenLists(1);
    glNewList(dl, GL_COMPILE);

    glEnable(GL_LIGHTING);
    bot_wavefront_model_gl_draw(model);
    glDisable(GL_LIGHTING);

    glEndList();
    return dl;
}

static void
renderer_pose_control_draw(BotViewer *viewer, BotRenderer *renderer)
{

    RendererPose *self = (RendererPose*) renderer;

    glEnable(GL_DEPTH_TEST);

    if(!self->display_lists_ready)
    {
        if(self->wavefront_model)
            self->wavefront_dl = compile_display_list(self, self->wavefront_model);
        self->display_lists_ready = 1;
    }

    glPushMatrix();

    double rot_x = 0;
    double rot_y = 0;
    double rot_z = 0;

    double yaw = 0;
    double pitch = 0;
    double roll = 0;

    double rotmatrix[16];

    double q[4];

    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;

    char widgetname[80];

    /*if (self->pose_last)
      {
        dx = self->pose_last->position[0];
        dy = self->pose_last->position[1];
        dz = self->pose_last->position[2];

        a = self->pose_last->orient[0];
        b = self->pose_last->orient[1];
        c = self->pose_last->orient[2];
        d = self->pose_last->orient[3];
      }*/

// use display position offset (note that our global coordinate frame is rotated by 180deg about X!)
    dx = bot_gtk_param_widget_get_double(self->pw, self->widget_xoff_name);
    dy = - bot_gtk_param_widget_get_double(self->pw, self->widget_yoff_name);
    dz = -  bot_gtk_param_widget_get_double(self->pw, self->widget_zoff_name);


    if(self->stateVariances_last)
    {
        //animate position
        if(!(bot_gtk_param_widget_get_bool(self->pw, (self->widget_posenable_name))))
        {
            //(note that our global coordinate frame is rotated by 180deg about X!)
            dx +=  self->stateVariances_last->position[0] ;
            dy += -self->stateVariances_last->position[1];
            dz += -self->stateVariances_last->position[2];
        };


        q[0] = self->stateVariances_last->orient[0];
        q[1] = self->stateVariances_last->orient[1];
        q[2] = self->stateVariances_last->orient[2];
        q[3] = self->stateVariances_last->orient[3];

        //Create rotation matrix https://www.opengl.org/sdk/docs/man2/xhtml/glMultMatrix.xml http://stackoverflow.com/questions/7938373/from-quaternions-to-opengl-rotations

        //Transform angles and axes from our CS (z-down by rot 180deg about x ) to GUI angles
        quat2Euler(q, &(yaw), &(pitch), &(roll));

        printf("euler X in iner.main frame: %f %f %f\n",yaw/3.14*180, pitch/3.14*180, roll/3.14*180); //@TODO remove

        yaw = -yaw;
        pitch = -pitch;

        Euler2quat(q, &(yaw), &(pitch), &(roll));
        //---

        rotmatrix[0] = pow(q[0], 2) +  pow(q[1], 2) - pow(q[2], 2) - pow(q[3], 2);
        rotmatrix[1] = 2 * q[1] * q[2] + 2 * q[0] * q[3];
        rotmatrix[2] = 2 * q[1] * q[3] - 2 * q[0] * q[2];
        rotmatrix[3] = 0;

        rotmatrix[4] = 2 * q[1] * q[2] - 2 * q[0] * q[3];
        rotmatrix[5] = pow(q[0], 2) - pow(q[1], 2) + pow(q[2], 2) - pow(q[3], 2);
        rotmatrix[6] = 2 * q[2] * q[3] + 2 * q[0] * q[1];
        rotmatrix[7] = 0;

        rotmatrix[8] = 2 * q[1] * q[3] + 2 * q[0] * q[2];
        rotmatrix[9] = 2 * q[2] * q[3] - 2 * q[0] * q[1];
        rotmatrix[10] = pow(q[0], 2) - pow(q[1], 2) - pow(q[2], 2) - pow(q[3], 2);
        rotmatrix[11] = 0;

        rotmatrix[12] = 0;
        rotmatrix[13] = 0;
        rotmatrix[14] = 0;
        rotmatrix[15] = 1;



       
	glTranslated(dx, dy, dz);
	//glMultMatrixd(rotmatrix);
        


        glRotated(yaw/3.14*180, 0, 0, 1);
        glRotated(pitch/3.14*180, 0, 1, 0);        
        glRotated(roll/3.14*180, 1, 0, 0);

        const double kModelRotX = 90.0;
        const double kModelRotY = 0.0;
        const double kModelRotZ = 225.0;
        const double kModelDX = 0.0;//-2.6;
        const double kModelDY = 0.0;//-0.9;
        const double kModelDZ = 0.0;
        const double kModelScale = 1; //4.5 / 13.5; lamborgini;

        glTranslated(kModelDX, kModelDY, kModelDZ);
        glRotated(kModelRotZ, 0, 0, 1);
        glRotated(kModelRotY, 0, 1, 0);
        glRotated(kModelRotX, 1, 0, 0);
        //glScaled(kModelScale, kModelScale, kModelScale);




    }


    if(self->display_lists_ready && self->wavefront_dl)
        draw_wavefront_model(self);

    glPopMatrix();
    return;
}






static void
renderer_pose_control_free(BotRenderer *renderer)
{

    RendererPose *self = (RendererPose*) renderer;

    free(self);

    return;
}



static void
on_param_widget_changed(BotGtkParamWidget *pw, const char *name, void *user)
{

    RendererPose *self = (RendererPose*) self;


    return;
}





void add_pose_renderer_to_viewer(BotViewer *viewer, int render_priority, lcm_t *lcm, const char* renderer_name, const char* pose_lcm_channel)
{

    // Setup renderer
    RendererPose *self = new RendererPose;
    BotRenderer *renderer = &self->renderer;
    self->viewer = viewer;
    renderer->draw = renderer_pose_control_draw;
    renderer->destroy = renderer_pose_control_free;
    renderer->widget = bot_gtk_param_widget_new();
    renderer->name = (char*)renderer_name;
    renderer->user = self;
    renderer->enabled = 1;
    self->pw = BOT_GTK_PARAM_WIDGET(renderer->widget);
    self->pose_last = NULL;
    self->stateVariances_last = NULL;

    // Subscribe to lcm messages
    self->lcm = lcm;

    /*
    self->pose_last = NULL;
    agile_pose_t_subscribe (lcm, "testdata", &pose_message_handler, self);
    */

    self->stateVariances_last = NULL;
    agile_stateVariances_t_subscribe(lcm, pose_lcm_channel, &stateVariances_message_handler, self);

    //add UI elements to interface

    get_widgetspec_name(self->widget_posenable_name, "fix position", renderer_name);
    bot_gtk_param_widget_add_booleans(self->pw, BOT_GTK_PARAM_WIDGET_CHECKBOX,
                                      self->widget_posenable_name, 0, NULL);

    get_widgetspec_name(self->widget_xoff_name, "X offset", renderer_name);
    bot_gtk_param_widget_add_double(self->pw, self->widget_xoff_name, BOT_GTK_PARAM_WIDGET_SPINBOX,
                                    INT_MIN, INT_MAX, 0.1, 0.0);

    get_widgetspec_name(self->widget_yoff_name, "Y offset", renderer_name);
    bot_gtk_param_widget_add_double(self->pw, self->widget_yoff_name, BOT_GTK_PARAM_WIDGET_SPINBOX,
                                    INT_MIN, INT_MAX, 0.1, 0.0);

    get_widgetspec_name(self->widget_zoff_name, "Z offset", renderer_name);
    bot_gtk_param_widget_add_double(self->pw, self->widget_zoff_name, BOT_GTK_PARAM_WIDGET_SPINBOX,
                                    INT_MIN, INT_MAX, 0.1, 0.0);

    // Arrange the GTK interface
    gtk_widget_show_all(renderer->widget);
    g_signal_connect(G_OBJECT(self->pw), "changed", G_CALLBACK(on_param_widget_changed), self);
    on_param_widget_changed(self->pw, "", self);

    // Setup the wavefront model
    self->display_lists_ready = 0;
    self->wavefront_model = bot_wavefront_model_create("../../media/wavefront/quadrotor/quadrotor_model.obj");
    //self->wavefront_model = bot_wavefront_model_create("../../media/wavefront/lamborgini/car.obj");

    // Add the renderer to viewer
    bot_viewer_add_renderer(viewer, &self->renderer, render_priority);


}


