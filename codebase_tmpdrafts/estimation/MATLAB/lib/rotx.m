function R = rotx(alpha)
%
% Create a 3x3 rotation matrix, describing a rotation of 'alpha' around the x 
% axis. The angle alpha is expressed in radiants
% 
% R = [ 1   0           0 
%       0   cos(alpha) -sin(alpha)
%       0   sin(alpha)  cos(alpha)]
%
% Author: Luca Carlone

R = eye(3);
R(2,2) = cos(alpha);
R(3,3) = R(2,2);
R(3,2) = sin(alpha);
R(2,3) = -R(3,2);

