% test02: script to acquire data from IMU and perform attitude estimation using 
% gyroscope (for prediction) and gravity and magnetic north directions for the update
%
% Date: 2015/8/20
% Authors: Roberto Brusniki, Luca Carlone

% set port to communicate with IMU via arduino
s = serial('/dev/tty.usbserial-A98NBXP9');   % arduino address
set(s,'BaudRate', 115200);

% conversion factor, to avoid round-off errors of data from Arduino
k = 1e6;

fopen(s);
flushinput(s);
fscanf(s);

%% Initial calibration: 
% When the IMU is not moving we estimate
% gravity direction, magnetic north direction, gyro biases, and gyro std 
n=100;   %amount of data colected to use in calibration
C = zeros(n,10);
for i=1:n
    not_done = true;
    while not_done
        A = fscanf(s);
        D = str2double(strsplit(A,' '))/k;
        C(i,:) = check_vectors_and_normalize(D);
        if C(i,1)~=0
            not_done = false;
        end
    end
end
[w_stats, B] = calibration(C);   %struct w_stats {bias {x,y,z}, sigma {x,y,z} }
% B: Vector to store values read from IMU (1 timestamp, 3 gyro, 3 acc, 3 mag)  

%% Set parameters: covariances and initial state
% measurements std for b1 and b2 (gravity and magnetic north direction) 
sigma_b1 = 0.007;            
sigma_b2 = 0.005;

% Measurement covariance
cov_matrixes.R = [sigma_b1^2 0 0 0 0 0;
                  0 sigma_b1^2 0 0 0 0;
                  0 0 sigma_b1^2 0 0 0;
                  0 0 0 sigma_b2^2 0 0;
                  0 0 0 0 sigma_b2^2 0;
                  0 0 0 0 0 sigma_b2^2];

% Process covariance
cov_matrixes.Q = [w_stats.sigma.x^2 0 0;
                  0 w_stats.sigma.y^2 0;
                  0 0 w_stats.sigma.z^2];

% Initial covariance
cov_matrixes.P = 0.01*eye(4);

% Initial state
q = [1; 0; 0; 0];   %initialization
reference_versors.r1 = [B(5); B(6); B(7)];    % averaged gravity versor at start
reference_versors.r2 = [B(8); B(9); B(10)];   % averaged magnetic versor at start

% Clean buffer and start acquisition
flushinput(s);  % clean the buffer
pause(0.001);
fscanf(s);  % the first one gets partial data, drop it
flushinput(s);  % clean the buffer
A = fscanf(s);  % the first ones gets partial data, drop it

%% Plotting
figure(1)
plot_frame(q);

%% Estimation loop
TIME = zeros(1000,1);
for i=1:1000            % read 100 measurements of imu data
    flushinput(s);
    
    % check data consistency, as before
    not_done = true;
    while not_done
      A = fscanf(s);                            %check data and normalize vectors
      B = check_vectors_and_normalize(str2double(strsplit(A,' '))/k);
      
      if (i== 1 || i==2 ) && B(1)~=0 
          prev_T = B(1);
      end
      
      % last condition: remember to lower this value in c version of code
      % 0.25 is aproximately 3.5 times 'normal' dt in matlab-version
      not_done = (B(1)==0 || abs(B(1)-prev_T) > 0.25 );
      
    end
    
    TIME(i,1) = prev_T;
    if i~=1    
       %call kalman filter loop
       [q, cov_matrixes.P, prev_T] = qekf(q, cov_matrixes, w_stats, reference_versors, B, prev_T); 
    end
    
    % NOTE(Luca): this is not super-clear
    TIME(i,1) = prev_T - TIME(i,1);
    
    % Plotting results
    plot_frame(q);
end
delete(findall(0,'Type','figure'))
fclose(s);
