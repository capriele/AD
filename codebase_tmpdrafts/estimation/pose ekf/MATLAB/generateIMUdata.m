function [B] = generateIMUdata(sensordata,tout)
% creates IMUdata-like from the sim_quadrotor simulink data

N = length(sensordata.ddx.Data);
B = zeros(N,10);
initial_time = round(1e6*rand());  %arduino time

for n=1:N,
    B(n,1) = round(1e6*tout(n)) + initial_time;         %timestamp
    B(n,2) = round(1e6*sensordata.p.Data(n));           %omega x
    B(n,3) = round(1e6*sensordata.q.Data(n));           %omega y
    B(n,4) = round(1e6*sensordata.r.Data(n));           %omega z
    B(n,5) = round(1e6*sensordata.ddx.Data(n));
    B(n,6) = round(1e6*sensordata.ddy.Data(n));
    B(n,7) = round(1e6*sensordata.ddz.Data(n));
    B(n,8) = 0;  % no magnetometer data from sim_quadrotor
    B(n,9) = 0;
    B(n,10) = 0; 
    
end

    