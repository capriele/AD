#ifndef POSE_RENDERER_HPP_
#define POSE_RENDERER_HPP_

#ifdef __cplusplus
extern "C" {
#endif

void add_pose_renderer_to_viewer (BotViewer *viewer, int render_priority, lcm_t *lcm);

#ifdef __cplusplus
}
#endif

#endif

