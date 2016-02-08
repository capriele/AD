esc_control_by_serial_or_receiver_using_rcarduinofastlib
-----------
Contains the code currently used for controlling ESCs

Doesn't seem to work beyond 100hz - oscilloscope shows jitter of 5us

also good, but uses interrupt pins int0 and int1:
esc_control_by_serial_or_receiver_using_servo_lib)100hz

interrupt on receiver-channel8-flag disturbs pulseln-reading, but that might be ok as cleaness and accuray of jetson-pwm is more important!



esc_control_by_serial_or_receiver_using_servo_library_100Hz:
----------------
requires changes in Servo.h!




