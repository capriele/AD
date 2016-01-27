%% Linearization of drone dynamics about hover & full-state feedback design
% ===============================
% AUTHOR Fabian Riether
% CREATE DATE 2015/08/25
% PURPOSE This code assists in linearizing drone dynamics and designing
% orientation LQR control
% SPECIAL NOTES
% ===============================
%  2015/08/25 created
% ==================================

%Load drone parameters from RoboticsToolbox
mdl_quadrotor


%% 1.1) Simplified Dynamics

%symbolic variables
syms yaw pitch roll p q r taup taur tauy;
symsvector  = [yaw ;pitch ;roll ; p ;q ;r ;taup ;taur ;tauy];

%Inertia
J            = quad.J;

%Define Rotation matrices
Ryaw = [
	[ cos(yaw), -sin(yaw), 0],
	[ sin(yaw),  cos(yaw), 0],
	[        0,         0, 1]
];

Rpitch = [
	[  cos(pitch), 0, sin(pitch)],
	[           0, 1,          0],
	[ -sin(pitch), 0, cos(pitch)]
];

Rroll = [
	[ 1,         0,          0],
	[ 0, cos(roll), -sin(roll)],
	[ 0, sin(roll),  cos(roll)]
];

Body2Global = Ryaw*Rpitch*Rroll;
Global2Body = simplify(Body2Global^-1);

%Transformation from body rates p-q-r to euler rates yaw pitch roll
iW = ...
    [0        sin(roll)          cos(roll);             
     0        cos(roll)*cos(pitch) -sin(roll)*cos(pitch);
     cos(pitch) sin(roll)*sin(pitch) cos(roll)*sin(pitch)] / cos(pitch);

%%Linearization Point = Hover
%-----------
state_equil = [0 ;0 ;0 ;0 ;0 ;0 ]; %x_eq
input_equil = [0 ;0 ;0];           %u_eq
equil       = [state_equil; input_equil];

%%Dynamics
%----------      
%O dot      
O_dot           = iW*[p;q;r];
O_dot_jacobian  = jacobian(O_dot,symsvector);
O_dot_jacobian_eql = subs(O_dot_jacobian,symsvector,equil);

%o ddot      
o_ddot          = inv(J)*([taur; taup; tauy] - cross([p;q;r],J*[p;q;r]));
o_ddot_jacobian = jacobian(o_ddot,symsvector);
o_ddot_jacobian_eql = subs(o_ddot_jacobian,symsvector,equil);

%Dynamics matrix
%---------- 

matrixAB = [O_dot_jacobian_eql;o_ddot_jacobian_eql];

A = double(matrixAB(:,1:6));
B = double(matrixAB(:,7:9));

%LQR
Q = diag([1/(pi/2) 1/(pi/3) 1/(pi/3) 1/5 1/5 1/5].^2);
rho = 1/0.164^2;
R = rho*diag([1 1 1]);

K_lqr = lqr(A,B,Q,R)


