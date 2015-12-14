function S = skew(x)
%
% Creates a 3x3 skew symmetrix matrix from a vectr x in R3
% 
% Author: Luca Carlone

S = [0 -x(3) x(2)  
     x(3) 0 -x(1)  
     -x(2) x(1) 0 ];
