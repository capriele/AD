function isEqual = equalWithTol(x,y,tol)
%
% Tests if the vectors x == y up to the specified tolerance
%
% Author: Luca Carlone
%
isEqual = 0;
if norm(x-y) < tol
   isEqual = 1; 
end