function [success] = test_quat2rot()
% test_quat2rot: unit test for quat2rot.m
% success = 1 if all tests pass, otherwise it is zero
close all
clc
success = 1;

%% Test 1
theta_z = 0.1; % rotation around z axis of 0.1 rad 
q = [cos(0.1/2); 0; 0; sin(0.1/2)]; % corresponding quaternion

expected_R = rotz(theta_z); % manually build a rotation around z of theta_z radians
actual_R = rot_matrix(q);

if ~equalWithTol(expected_R,actual_R,1e-4)
    warning('TEST1: expected_q ~= actual_q')
    success = 0;
end



 
