%% Calibrator


%% Record data: turn IMU all directions, no acceleration please!

%fake testdata
bias_sim = [-.1; .2; .15];
gainbias_sim = [1.1;0.95;1.05];

X_grid = (2*rand(1,1000)-1);
Y_grid = (2*rand(1,1000)-1);
%remove those not in circle, take sperical coordinates for nicer fake data setup
ok = sqrt(X_grid.^2 + Y_grid.^2)<=1;
X_grid = X_grid(ok);
Y_grid = Y_grid(ok);
Z_grid = sign(rand(1,sum(ok))-0.5).*sqrt(1-X_grid.^2 -Y_grid.^2);



data_orig = diag(gainbias_sim)*9.81*[X_grid;Y_grid;Z_grid]+repmat(bias_sim,1,sum(ok));
data = data_orig + 0.5*(rand(size(data_orig))-0.5);

hold off;
plot3(data_orig(1,:),data_orig(2,:),data_orig(3,:),'x');
axis equal
hold all;
plot3(data(1,:),data(2,:),data(3,:),'x');


%% 1 load data

%% 2 fit 3D ball to data

sphereFun = @(b) (9.81^2 - 1/(b(4)^2)*(data(1,:)-b(1)).^2 - 1/(b(5)^2)*(data(2,:)-b(2)).^2 - 1/(b(6)^2)*(data(3,:)-b(3)).^2) * (9.81^2 - 1/(b(4)^2)*(data(1,:)-b(1)).^2 - 1/(b(5)^2)*(data(2,:)-b(2)).^2 - 1/(b(6)^2)*(data(3,:)-b(3)).^2)';

[b_opt] = fminsearch(sphereFun,[0 0 0 1 1 1],optimset('TolX',1e-8,'MaxFunEvals',10000,'MaxIter',10000))

%% 3 Find bias and gain errors

% Find r = [rx,ry,rz] offset of ball from origin

% Apply SVD, find rotaion matrix R to rotate major axis to align with x-y-z
% Find length of major axis

% bias      = R*r
% gainerror = majoraxis_x/(2*g) majoraxis_y/(2*g) majoraxis_z/(2*g)