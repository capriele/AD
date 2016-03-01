%% Initial data consistency check (right length of data string, no [0 0 0] in acc and mag)
% and normalize the data. We store the average in the vector B, such that
% the directions of the initial acceleration and magnetometer fix our reference frame
function [w_stats, B] = calibration(C)

B = mean(C);      
STD = std(C);        

% estimate gyro biases
w_stats.bias.x = B(2);       %stores stats in the struct
w_stats.bias.y = B(3);
w_stats.bias.z = B(4);

% estimate gyro standard deviation
w_stats.sigma.x = STD(2);           
w_stats.sigma.y = STD(3);          
w_stats.sigma.z = STD(4);

% After averaging gravity and magnetic north direction, we normalize them to unit vectors
B(5:7) = B(5:7)/norm(B(5:7));
B(8:10) = B(8:10)/norm(B(8:10));