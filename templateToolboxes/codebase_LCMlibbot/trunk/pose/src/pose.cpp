#include <iostream>
#include <cmath>

#include <glib.h>


#include <lcm/lcm-cpp.hpp>

#include <lcmtypes/lcmtypes.hpp>

using namespace std;



class Pose {
  
public:
  int value;
  
  lcm::LCM lcm;
  
  Pose () : value(0) {}
  
};


gboolean gtimerfunc (gpointer data) {

  Pose* pose = (Pose*) data;

  cout << "Ready: " << pose->value << endl;

  // Publish the pose message
  agile::pose_t pose_data;

  pose_data.timestamp = 0;
  pose_data.position[0] = 0;
  pose_data.position[1] = 0;
  pose_data.position[2] = sin((double)(pose->value++)/10);

  pose_data.orientation[0] = 1;
  pose_data.orientation[1] = 0;
  pose_data.orientation[2] = 0;
  pose_data.orientation[3] = 0;

  pose->lcm.publish ("POSE", &pose_data);

  return TRUE;
}


int main (int argc, char** argv) {


  // Create the app
  Pose pose; 

  // Create LCM
  if (!pose.lcm.good()) 
    return 1;

  
  // Create a function call 
  guint timer = g_timeout_add (100, gtimerfunc, (gpointer)&pose);
  
  // Create the mainloop
  GMainLoop* mainloop = g_main_loop_new (NULL, TRUE);
  g_main_loop_run (mainloop);

  return 0;
}







