% pose_sim: script to simulate full pose estimation using 
% gyroscope and acelerometer (for prediction) and mark vectors directions for the update
%
% Date: 2016/02/29
% Author: Roberto Brusnicki

load('tout.mat');      
load('states.mat');
load('sensordata.mat');
load('quadEDT.mat');

%% Getting data from simulink

IMUdata = generateIMUdata(sensordata,tout);

%% Initial calibration values: 
  
% estimated accel biases
a_stats.bias.x = quadEDT.sensordataRSbias(1);   %stores stats in the struct    
a_stats.bias.y = quadEDT.sensordataRSbias(2);
a_stats.bias.z = quadEDT.sensordataRSbias(3);

% estimated gyro biases
w_stats.bias.x = quadEDT.sensordataRSbias(4);          
w_stats.bias.y = quadEDT.sensordataRSbias(5);
w_stats.bias.z = quadEDT.sensordataRSbias(6);

% estimated accel standard deviation
a_stats.sigma.x = 0.1470;           
a_stats.sigma.y = 0.1459;           %only true for the current data used
a_stats.sigma.z = 0.1900;           %TO DO: write script to extract this information from any data used

% estimated gyro standard deviation
w_stats.sigma.x = 9.9459e-04;           
w_stats.sigma.y = 8.4800e-04;          
w_stats.sigma.z = 7.4911e-04;

% struct w_stats {bias {x,y,z}, sigma {x,y,z} }
% struct a_stats {bias {x,y,z}, sigma {x,y,z} }
% B: Vector to store values read from IMU (1 timestamp, 3 gyro, 3 acc, 3 mag == 0)  

%% Set parameters: covariances and initial state
% measurements std for versors 

                            %simplified model for the uncertainty in vector lengths.
                            
sigma_b1 = pi/1024;         %field of view of 180º and 1024 pixels for it           
sigma_b2 = pi/1024;         %+/- 1 pixel gives +-pi/1024 in the length of vectors
sigma_b3 = pi/1024;         %(strictly, this is true only for camera's x and y directions.
sigma_b4 = pi/1024;         % Chech with Luca: should we use lower value for z, or even zero? )
sigma_b5 = pi/1024;            
sigma_b6 = pi/1024;
sigma_b7 = pi/1024;            


% Process covariance
Qw = [w_stats.sigma.x^2 0 0;
      0 w_stats.sigma.y^2 0;
      0 0 w_stats.sigma.z^2];

Qa = [a_stats.sigma.x^2 0 0;
      0 a_stats.sigma.y^2 0;
      0 0 a_stats.sigma.z^2];

cov_matrixes.Q = [Qw zeros(3);
                  zeros(3) Qa];

% Measurement covariance
Rb1 = [sigma_b1^2 0 0;
       0 sigma_b1^2 0;
       0 0 sigma_b1^2];
Rb2 =  [sigma_b2^2 0 0;
       0 sigma_b2^2 0;
       0 0 sigma_b2^2];
Rb3 =  [sigma_b3^2 0 0;
       0 sigma_b3^2 0;
       0 0 sigma_b3^2];
Rb4 =  [sigma_b4^2 0 0;
       0 sigma_b4^2 0;
       0 0 sigma_b4^2];
Rb5 =  [sigma_b5^2 0 0;
       0 sigma_b5^2 0;
       0 0 sigma_b5^2];
Rb6 =  [sigma_b6^2 0 0;
       0 sigma_b6^2 0;
       0 0 sigma_b6^2];
Rb7 =  [sigma_b7^2 0 0;
       0 sigma_b7^2 0;
       0 0 sigma_b7^2];
O3 = zeros(3);                  %simplified model. In final version of code this 
                                %information will come from camera's code
   
cov_matrixes.R = [Rb1 O3  O3  O3  O3  O3  O3;
                  O3  Rb2 O3  O3  O3  O3  O3;
                  O3  O3  Rb3 O3  O3  O3  O3;
                  O3  O3  O3  Rb4 O3  O3  O3;
                  O3  O3  O3  O3  Rb5 O3  O3;
                  O3  O3  O3  O3  O3  Rb6 O3;
                  O3  O3  O3  O3  O3  O3  Rb7];

% Initial covariance
cov_matrixes.P = 0.01*eye(10);

% Initial state
q = [1; 0; 0; 0];       
t = [0; 0; 0];          %initialy at the origin of World Coordinate System, with zero velocity
v = [0; 0; 0];

pose = [q; t; v];  %initialization

features_initial = [2.00   2.00   2.00   2.00   2.00   2.00   2.00;     %simulates the corners of the square and triangule
                    0.00   0.52   0.52   0.00  -0.10  -0.24  -0.10;     %in the lab's board that Guilherme used 
                   -1.51  -1.51  -1.04  -1.04  -1.04  -1.04  -1.54];

[features, N] = generateFeaturesData(states,features_initial);
               
reference_versors.r1 = features_initial(:,1);    
reference_versors.r2 = features_initial(:,2); 
reference_versors.r3 = features_initial(:,3);    
reference_versors.r4 = features_initial(:,4);
reference_versors.r5 = features_initial(:,5);    
reference_versors.r6 = features_initial(:,6);
reference_versors.r7 = features_initial(:,7);


%% Plotting
figure(1)
plot_frame(pose);

%% Estimation loop

prev_T = IMUdata(1:1)/1000000;
for i=1:N           
    
    if i~=1
       z = [features(i,:,1); features(i,:,2); features(i,:,3)]; 
       z_kp1 = [z(:,1); z(:,2); z(:,3); z(:,4); z(:,5); z(:,6); z(:,7)];
       %call kalman filter loop
       [pose, cov_matrixes.P, prev_T] = pose_ekf(pose, cov_matrixes, w_stats, a_stats, reference_versors, z_kp1, IMUdata(i,:), prev_T); 
    end
    
    % Plotting results
    plot_frame(pose);
end

delete(findall(0,'Type','figure'))