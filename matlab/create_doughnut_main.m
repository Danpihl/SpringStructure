clc, clear

r = 1;
R = 4;
Nn = 20;
N = 10;

phi = linspace(0, 2*pi, Nn);
theta = linspace(0, 2*pi - 2*pi/N, N);
P = [];

p = [R + r*cos(phi); r*sin(phi); zeros(1, Nn)];
% Rotate around y axis
Ry = @(p)[cos(p), 0, sin(p);
          0,      1, 0;
          -sin(p), 0, cos(p)];
% pr = R*p;
for k = 1:N
    pr = Ry(theta(k))*p;
    P = [P, pr];
    
end

a = 5;

figure(1); clf; hold on; grid on;
% scatter3(p(1, :), p(2, :), p(3, :), 'r');
scatter3(P(1, :), P(2, :), P(3, :), 'r');
plot3([-a, a], [0, 0], [0, 0], 'k--');
plot3([0, 0], [-a, a], [0, 0], 'k--');
plot3([0, 0], [0, 0], [-a, a], 'k--');
xlabel('x');
ylabel('y');
zlabel('z');
axis([-1, 1, -1, 1, -1, 1]*a)
% axis square