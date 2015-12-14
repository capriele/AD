function [] = plot_frame(q)

% Vectors in local frame
o  = [ 0  0  0]';
vx = [10  0  0]';
vy = [ 0 10  0]';
vz = [ 0  0 10]';

% Convert to rotation matrix - Rot converts global to local frame
Rot = rot_matrix(q);

% Plotting
Vx = Rot'*vx;        % rotates coordinate system
Vy = Rot'*vy;
Vz = Rot'*vz;

hold off;               % plot 'animation'
quiver3(o(1),o(2),o(3),Vx(1),Vx(2),Vx(3));
hold on
axis([-11 11 -11 11 -11 11]);
quiver3(o(1),o(2),o(3),Vy(1),Vy(2),Vy(3));
quiver3(o(1),o(2),o(3),Vz(1),Vz(2),Vz(3));
drawnow