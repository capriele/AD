function [] = plot_frame(pose)

% Vectors in local frame
o  = pose(5:7,1);
vx = [10  0  0]';
vy = [ 0 10  0]';
vz = [ 0  0 10]';

% Convert to rotation matrix - Rot converts global to local frame
Rot = rot_matrix(pose(1:4,1));

% Plotting
Vx = Rot'*vx;        % rotates coordinate system
Vy = Rot'*vy;
Vz = Rot'*vz;

hold off;               % plot 'animation'
quiver3(o(1),o(2),o(3),o(1)+Vx(1),o(2)+Vx(2),o(3)+Vx(3));
hold on
axis([-21 21 -21 21 -21 21]);
quiver3(o(1),o(2),o(3),o(1)+Vy(1),o(2)+Vy(2),o(3)+Vy(3));
quiver3(o(1),o(2),o(3),o(1)+Vz(1),o(2)+Vz(2),o(3)+Vz(3));
title(pose');
drawnow