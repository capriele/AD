function [features, N] = generateFeaturesData(states,features_initial)
%
% Create a simulated matrix of vector measurments from drone's pose
% - states.Data is the drone's pose over time
% - features matrix containing the simulated vector measurements over time
%       dimensions: [sample, markers, coordinates]  ( [N,7,3] )
%
% Author: Roberto Brusnicki

N = length(states.Data);
A = states.Data; % [X, Y, Z, yaw, pitch, roll]

%markes used, cornes of a square and a triangle drawn in the drone's front wall
%features_initial = [2.00   2.00   2.00   2.00   2.00   2.00   2.00;     
%                    0.00   0.52   0.52   0.00  -0.10  -0.24  -0.10;
%                   -1.51  -1.51  -1.04  -1.04  -1.04  -1.04  -1.54];
               
features = zeros(N,7,3);
v = zeros(3,1);

%for each sample
for n=1:N,      
    %rotations matrix in: yaw, pitch and roll
    Ry=uth2rot([0,0,1],-A(n,4));    
    Rp=uth2rot([0,1,0],-A(n,5));
    Rr=uth2rot([1,0,0],-A(n,6));
    
    %for each markers
    for k=1:7,
        
        %compute vector from drone to marker
        v(1) = features_initial(1,k)'-A(n,1); 
        v(2) = features_initial(2,k)'-A(n,2);
        v(3) = features_initial(3,k)'-A(n,3);
        
        %normalize vector
       % v = v/norm(v);     %Normalize if decide to decrease dimension 
                            %to get a faster performance (~2.36x faster)
                            
        %rotate vector using drone's yaw, pitch and roll angles
        v_rot = Rr*Rp*Ry*v;
        
        features(n,k,1) = v_rot(1);
        features(n,k,2) = v_rot(2);
        features(n,k,3) = v_rot(3);
        
    end
        
end
