function R = rotz(gamma)
%
% Create a 3x3 rotation matrix, describing a rotation of 'gamma' around the z 
% axis. The angle beta is expressed in radiants.
% 
% R = [ cos(gamma)    -sin(gamma)    0 
%       sin(gamma)    cos(gamma)     0
%       0             0              1]
%
% Author: Luca Carlone

R = eye(3);
R(2,2) = cos(gamma);
R(1,1) = R(2,2);
R(2,1) = sin(gamma);
R(1,2) = -R(2,1);
