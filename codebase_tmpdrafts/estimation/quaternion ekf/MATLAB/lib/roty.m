function R = roty(beta)
%
% Create a 3x3 rotation matrix, describing a rotation of 'beta' around the y 
% axis. The angle beta is expressed in radiants.
% 
% R = [ cos(beta)     0           sin(beta) 
%       0             1           0
%       -sin(beta)    0           cos(beta)]
%
% Author: Luca Carlone

R = eye(3);
R(1,1) = cos(beta);
R(3,3) = R(1,1);
R(1,3) = sin (beta);
R(3,1) = -R(1,3);

