function [success] = test_qekf()
% test_qekf: unit test for qekf.m
% success = 1 if all tests pass, otherwise it is zero
close all
clc
success = 1;

%% Test 1
cov_matrixes.Q = 0.01 * eye(3);  % process noise covariance
cov_matrixes.R = 0.01*eye(6); % measurement noise covariance
cov_matrixes.P = 0.01*eye(4); % quaternion noise covariance

reference_versors.r1 = [0 0 -1];
reference_versors.r2 = [0 1  0];
q = [1; 0; 0; 0];   %initialization
prev_T = 0;
w_stats.bias.x = 0;
w_stats.bias.y = 0;
w_stats.bias.z = 0;
% B: Vector to store values read from IMU (1 timestamp, 3 gyro, 3 acc, 3 mag) 
B = [1 ,  0  0  0.1 ,  0 0 -1,  0 1  0];

% Expected values: this is what we expect as result of the prediction of the EKF:
% we measure a rotation rate of 0.1rad/s along z, for 1 second, hence 
% we expect that the estimated rotation is:
expected_q = [cos(0.1/2); 0; 0; sin(0.1/2)];
expected_prev_T = 1;

% Now we get the *actual values* by running the function we want to test
[actual_q,actual_P,actual_prev_T] = qekf(q, cov_matrixes, w_stats, reference_versors, B, prev_T);

if ~equalWithTol(expected_q,actual_q,1e-4)
    warning('TEST1: expected_q ~= actual_q')
    success = 0;
end
if ~equalWithTol(expected_prev_T,actual_prev_T,1e-4)
    warning('TEST1: expected_prev_T ~= actual_prev_T')
    success = 0;
end


 
