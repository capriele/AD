function [pose_hat_kp1_kp1,P_kp1_kp1,prev_T] = pose_ekf(pose, cov_matrixes, w_stats, a_stats, reference_versors, z_kp1, IMUdata, prev_T)
%#codegen

%% Getting data
%B: all measurements
%pose: current pose estimate
q_hat_k_k = pose(1:4,1);    % estimated quaternion (q) at k given k measurements 
t_hat_k_k = pose(5:7,1);    % estimated position (t) at k given k measurements
v_hat_k_k = pose(8:10,1);   % estimated velocity (v) at k given k measurements

g = [0; 0; 9.81];

Q = cov_matrixes.Q;     % process noise covariance ( Qa & Qw )
R_kp1 = cov_matrixes.R; % measurement noise covariance
P_k_k = cov_matrixes.P; % pose noise covariance

r1 = reference_versors.r1; % reference vectros 
r2 = reference_versors.r2; 
r3 = reference_versors.r3;
r4 = reference_versors.r4;
r5 = reference_versors.r5;
r6 = reference_versors.r6;
r7 = reference_versors.r7; 

%prev_T: timestamp of the previous measurement(

kk = 1000000; %multiplier factor used in arduino measurements 

% correct biases
wx = IMUdata(2)/kk - w_stats.bias.x; % angular velocity mesauremnts
wy = IMUdata(3)/kk - w_stats.bias.y; 
wz = IMUdata(4)/kk - w_stats.bias.z; 

ax = IMUdata(5)/kk - a_stats.bias.x; % x linear aceleration measurements
ay = IMUdata(6)/kk - a_stats.bias.y;
az = IMUdata(7)/kk - a_stats.bias.z;

a_k = [ax;ay;az];

% compute time interval between current and previous measurement (integration period)
T = (IMUdata(1)/kk-prev_T);  % time between mesaurements (s)

% store in better format the current direction measurements
B2 = [IMUdata(8)/kk; IMUdata(9)/kk; IMUdata(10)/kk];   % magnetic versor measurement  (currently not used)

%% 
% rotation rate matrix
% Continuous-time quaternion process model: qdot = (1/2) Omega q
% Discrete-time quaternion process model: q_{k+1} = e^{1/2 Omega T} q_k (Wertz78book, pag 564)

Omega_k =       [ 0  -wx   -wy  -wz;
                  wx    0   wz  -wy;
                  wy  -wz    0   wx;
                  wz   wy  -wx    0];

w_k = norm([wx; wy; wz]);

% See Wertz78book, eq (17.18)
Exp_Omega_k_T = cos(w_k*(T/2))*eye(4)+(1/w_k)*sin(w_k*(T/2))*Omega_k;

%% 1a - Estimated state propagation
Fq_k = [Exp_Omega_k_T zeros(4,3) zeros(4,3)];

q_hat_kp1_k = Fq_k(1:4,1:4)*q_hat_k_k;

q1 = q_hat_k_k(1);
q2 = q_hat_k_k(2);
q3 = q_hat_k_k(3);
q4 = q_hat_k_k(4);

dDdq1_inv = [ -2*q1  2*q4 -2*q3;
              -2*q4 -2*q1  2*q2;
               2*q3 -2*q2 -2*q1];
    
dDdq2_inv = [ 2*q2  2*q3  2*q4;
              2*q3 -2*q2 -2*q1;
              2*q4  2*q1 -2*q2];

dDdq3_inv = [-2*q3  2*q2  2*q1;
              2*q2  2*q3  2*q4;
             -2*q1  2*q4 -2*q3];

dDdq4_inv = [-2*q4 -2*q1  2*q2;
              2*q1 -2*q4  2*q3;
              2*q2  2*q3  2*q4];

dDdq_inv_ak   = [dDdq1_inv*a_k dDdq2_inv*a_k dDdq3_inv*a_k dDdq4_inv*a_k];

D_q_k_inv=[ q1^2+q2^2-q3^2-q4^2    2*(q2*q3-q1*q4)       2*(q2*q4+q1*q3);
              2*(q2*q3+q1*q4)    q1^2-q2^2+q3^2-q4^2     2*(q4*q3-q1*q2);
              2*(q2*q4-q1*q3)      2*(q4*q3+q1*q2)     q1^2-q2^2-q3^2+q4^2];

t_hat_kp1_k = t_hat_k_k + T*eye(3)*v_hat_k_k + 0.5*(T^2)*eye(3)*(D_q_k_inv*a_k-g);

Ft_k = [0.5*(T^2)*eye(3)*dDdq_inv_ak eye(3) T*eye(3)];

%----

v_hat_kp1_k = v_hat_k_k + T*eye(3)*(D_q_k_inv*a_k-g);

Fv_k = [T*eye(3)*dDdq_inv_ak zeros(3) eye(3)];

%----

F_k = [Fq_k; Ft_k; Fv_k];

pose_hat_kp1_k = [q_hat_kp1_k; t_hat_kp1_k; v_hat_kp1_k];

%----

Csi_k=[-q2 -q3 -q4;
        q1 -q4  q3; 
        q4  q1 -q2;
       -q3  q2  q1];
 
Gama_k=(T/2)*Exp_Omega_k_T*Csi_k;

Q_q_k = Gama_k*Q(1:3,1:3)*Gama_k';

Wq_k = [Q_q_k zeros(4,3) zeros(4,3)];  

%----
Ak = 0.5*(T^2)*eye(3)*D_q_k_inv;

Q_t_k = Ak*Q(4:6,4:6)*Ak';

Wt_k = [zeros(3,4) Q_t_k zeros(3,3)];

%---

Q_v_k = T*D_q_k_inv*Q(4:6,4:6)*(T*D_q_k_inv)';

Wv_k = [zeros(3,4) zeros(3,3) Q_v_k];

%----

Q_k = [Wq_k; Wt_k; Wv_k];

P_kp1_k = F_k*P_k_k*F_k'+Q_k;

%% 1b - Measuremnts Prediction
doEKFupdate = 1;
if doEKFupdate == 1
    
    q1=pose_hat_kp1_k(1);
    q2=pose_hat_kp1_k(2);
    q3=pose_hat_kp1_k(3);
    q4=pose_hat_kp1_k(4);
    
    D_q_hat_kp1_k = [ q1^2+q2^2-q3^2-q4^2    2*(q2*q3+q1*q4)       2*(q2*q4-q1*q3);
                       2*(q2*q3-q1*q4)    q1^2-q2^2+q3^2-q4^2     2*(q4*q3+q1*q2);
                       2*(q2*q4+q1*q3)      2*(q4*q3-q1*q2)     q1^2-q2^2-q3^2+q4^2];
    
    r1_t = r1-t_hat_kp1_k;
    r2_t = r2-t_hat_kp1_k;
    r3_t = r3-t_hat_kp1_k;
    r4_t = r4-t_hat_kp1_k;
    r5_t = r5-t_hat_kp1_k;
    r6_t = r6-t_hat_kp1_k;
    r7_t = r7-t_hat_kp1_k;
    
                   
    b1_hat_kp1_k = D_q_hat_kp1_k * r1_t;
    b2_hat_kp1_k = D_q_hat_kp1_k * r2_t;
    b3_hat_kp1_k = D_q_hat_kp1_k * r3_t;
    b4_hat_kp1_k = D_q_hat_kp1_k * r4_t;
    b5_hat_kp1_k = D_q_hat_kp1_k * r5_t;
    b6_hat_kp1_k = D_q_hat_kp1_k * r6_t;
    b7_hat_kp1_k = D_q_hat_kp1_k * r7_t;
    
    
    z_hat_kp1_k = [b1_hat_kp1_k' b2_hat_kp1_k' b3_hat_kp1_k' b4_hat_kp1_k' b5_hat_kp1_k' b6_hat_kp1_k' b7_hat_kp1_k']';
    
    
    dDdq1_kp1_k = [ 2*q1  2*q4 -2*q3;
                   -2*q4  2*q1  2*q2;
                    2*q3 -2*q2  2*q1];
    
    dDdq2_kp1_k = [ 2*q2  2*q3  2*q4;
                    2*q3 -2*q2  2*q1;
                    2*q4 -2*q1 -2*q2];
    
    dDdq3_kp1_k = [-2*q3  2*q2 -2*q1;
                    2*q2  2*q3  2*q4;
                    2*q1  2*q4 -2*q3];
    
    dDdq4_kp1_k = [-2*q4  2*q1  2*q2;
                   -2*q1 -2*q4  2*q3;
                    2*q2  2*q3  2*q4];
    
   
    H_1_kp1 = [dDdq1_kp1_k*r1_t dDdq2_kp1_k*r1_t dDdq3_kp1_k*r1_t dDdq4_kp1_k*r1_t D_q_hat_kp1_k zeros(3)]; % 3X10
    H_2_kp1 = [dDdq1_kp1_k*r2_t dDdq2_kp1_k*r2_t dDdq3_kp1_k*r2_t dDdq4_kp1_k*r2_t D_q_hat_kp1_k zeros(3)];
    H_3_kp1 = [dDdq1_kp1_k*r3_t dDdq2_kp1_k*r3_t dDdq3_kp1_k*r3_t dDdq4_kp1_k*r3_t D_q_hat_kp1_k zeros(3)];
    H_4_kp1 = [dDdq1_kp1_k*r4_t dDdq2_kp1_k*r4_t dDdq3_kp1_k*r4_t dDdq4_kp1_k*r4_t D_q_hat_kp1_k zeros(3)];
    H_5_kp1 = [dDdq1_kp1_k*r5_t dDdq2_kp1_k*r5_t dDdq3_kp1_k*r5_t dDdq4_kp1_k*r5_t D_q_hat_kp1_k zeros(3)];
    H_6_kp1 = [dDdq1_kp1_k*r6_t dDdq2_kp1_k*r6_t dDdq3_kp1_k*r6_t dDdq4_kp1_k*r6_t D_q_hat_kp1_k zeros(3)];
    H_7_kp1 = [dDdq1_kp1_k*r7_t dDdq2_kp1_k*r7_t dDdq3_kp1_k*r7_t dDdq4_kp1_k*r7_t D_q_hat_kp1_k zeros(3)];
    
    
    H_kp1 = [H_1_kp1; H_2_kp1; H_3_kp1; H_4_kp1; H_5_kp1; H_6_kp1; H_7_kp1]; % 21X10
    
    
    P_z_kp1_k = H_kp1 * P_kp1_k * H_kp1' + R_kp1;
    
    %% 1c - Cross-covariance
    
    P_pose_z_kp1_k = P_kp1_k * H_kp1';
    
    %% 2a - Gain
    
    K_kp1 = P_pose_z_kp1_k/(P_z_kp1_k);
    
    %% 2b - Estimation update
    
    pose_hat_kp1_kp1 = pose_hat_kp1_k + K_kp1*(z_kp1 - z_hat_kp1_k);
    
    P_kp1_kp1 = P_kp1_k - P_pose_z_kp1_k * K_kp1';
else
    pose_hat_kp1_kp1 = pose_hat_kp1_k; % use the estimate from the prediction
end
%% 3 - Normalization

q = pose_hat_kp1_kp1(1:4);
if norm(q) ~= 0 
    q = (1/norm(q))*q;
    pose_hat_kp1_kp1(1:4) = q;
else
   error('Quaternion normalization failed') 
end
    
prev_T = IMUdata(1);
end
