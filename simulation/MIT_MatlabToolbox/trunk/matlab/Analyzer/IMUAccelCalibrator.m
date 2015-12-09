%% Calibrator

%% Record data: turn IMU all directions, no acceleration please!

%% 1 load data

%% 2 fit 3D ball to data

%% 3 Find bias and gain errors

% Find r = [rx,ry,rz] offset of ball from origin

% Apply SVD, find rotaion matrix R to rotate major axis to align with x-y-z
% Find length of major axis

% bias      = R*r
% gainerror = majoraxis_x/(2*g) majoraxis_y/(2*g) majoraxis_z/(2*g)