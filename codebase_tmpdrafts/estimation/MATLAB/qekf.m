function [q,P,prev_T] = qekf(q, cov_matrixes, w_stats, reference_versors, B, prev_T)
%#codegen
addpath('./lib')

%B: all measurements
%q: current quaternion estimate: q_k
Q = cov_matrixes.Q; % process noise covariance
R = cov_matrixes.R; % measurement noise covariance
P = cov_matrixes.P; % quaternion noise covariance

r1 = reference_versors.r1; % reference gravity direction (from accelerometer)
r2 = reference_versors.r2; % reference magnetometer direction
%prev_T: timestamp of the previous measurement

% correct biases
wx = B(2) - w_stats.bias.x; % x angular velocity mesauremnt
wy = B(3) - w_stats.bias.y; % y angular velocity mesauremnt
wz = B(4) - w_stats.bias.z; % z angular velocity mesauremnt

% compute time interval between current and previous measurement (integration period)
dt = (B(1)-prev_T);  % time between mesaurements (s)

% store in better format the current direction measurements
B1 = [B(5); B(6); B(7)];    % gravity versor measurement
B2 = [B(8); B(9); B(10)];   % magnetic versor measurement

% rotation rate matrix
% Continuous-time process model: qdot = (1/2) Omega q
% Discrete-time process model: q_{k+1} = e^{1/2 Omega dt} q_k (Wertz78book, pag 564)

% NOTE(Luca): this does not match with Wertz78book, eq (17.3): different quaternion notation?
Omega_k =       [ 0  -wx   -wy  -wz;
                  wx    0   wz  -wy;
                  wy  -wz    0   wx;
                  wz   wy  -wx    0];

w_k = norm([wx; wy; wz]);

% See Wertz78book, eq (17.18)
Exp_Omega_k_T = cos(w_k*(dt/2))*eye(4)+(1/w_k)*sin(w_k*(dt/2))*Omega_k;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1a - Estimated state propagation
qp = Exp_Omega_k_T*q;

Csi_k=[-q(2) -q(3) -q(4);
        q(1) -q(4)  q(3); 
        q(4)  q(1) -q(2);
       -q(3)  q(2)  q(1)];
 
Gama_k=(dt/2)*Exp_Omega_k_T*Csi_k;


Q_q_k=Gama_k*Q*Gama_k';

P=Exp_Omega_k_T*P*Exp_Omega_k_T'+Q_q_k;

%----------------------------------------------------------------------
%1b - Measuremnts Prediction
doEKFupdate = 0;
if doEKFupdate == 1
    q1=qp(1);
    q2=qp(2);
    q3=qp(3);
    q4=qp(4);
    
    D_q_k=[ q1^2+q2^2-q3^2-q4^2    2*(q2*q3+q1*q4)       2*(q2*q4-q1*q3);
        2*(q2*q3-q1*q4)    q1^2-q2^2+q3^2-q4^2     2*(q4*q3+q1*q2);
        2*(q2*q4+q1*q3)      2*(q4*q3-q1*q2)     q1^2-q2^2-q3^2+q4^2];
    
    b_estimated=[(D_q_k*r1)' (D_q_k*r2)']';
    
    dDdq1 = [ 2*q1  2*q4 -2*q3;
        -2*q4  2*q1  2*q2;
        2*q3 -2*q2  2*q1];
    
    dDdq2 = [ 2*q2  2*q3  2*q4;
        2*q3 -2*q2  2*q1;
        2*q4 -2*q1 -2*q2];
    
    dDdq3 = [-2*q3  2*q2 -2*q1;
        2*q2  2*q3  2*q4;
        2*q1  2*q4 -2*q3];
    
    dDdq4 = [-2*q4  2*q1  2*q2;
        -2*q1 -2*q4  2*q3;
        2*q2  2*q3  2*q4];
    
    H_1_q = [dDdq1*r1 dDdq2*r1 dDdq3*r1 dDdq4*r1];
    H_2_q = [dDdq1*r2 dDdq2*r2 dDdq3*r2 dDdq4*r2];
    
    H_q = [H_1_q; H_2_q];
    
    P_b = H_q*P*H_q' + R;
    
    %----------------------------------------------------------------------
    %1c - Cross-covariance
    
    P_b_q = P*H_q';
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %2a - Gain
    
    K = P_b_q/(P_b);
    
    %----------------------------------------------------------------------
    %2b - Estimation update
    
    q = qp + K*([B1' B2']' - b_estimated);
    
    P = P - K*P_b*K';
else
    q = qp; % use the estimate from the prediction
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%3 - Normalization
if norm(q) ~= 0 
q = (1/norm(q))*q;
else
   error('Quaternion normalization failed') 
end

%[q' wx wy wz]
    
prev_T = B(1);
end
