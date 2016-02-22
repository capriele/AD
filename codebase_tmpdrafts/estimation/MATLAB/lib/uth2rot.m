function R = uth2rot(u,theta)
%
% Create a 3x3 rotation matrix from an axis-angle representation
% - u is the unit vector describing the rotation axis
% - theta is the rotation angle, in radians
% 
% Author: Luca Carlone

tol = 1e-4;
if abs(norm(u)-1) > tol
   error('rotation axis does not have unit norm') 
end
S=skew(u); 

% this is a version of the rodrigues formula
R = eye(3) + sin(theta) * S + (1-cos(theta)) * S^2;

