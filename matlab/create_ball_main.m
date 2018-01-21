clc, clear

load('d3.mat');
drand = load('randstr.mat');

rc = conn';
R = vert;
N = size(rc, 1);


rc = [zeros(N, 1), rc];

for k = 1:N
    
    if rc(k, end) == -1
        rc(k, 1) = 5;
    else
        rc(k, 1) = 6;
    end
    
end

rc(rc == -1) = 0;
rc = [rc(:, 1), 163*ones(N, 1), rc(:, 2:end)];
rc(:, 1) = rc(:, 1) + 1;

center_conn = 1:(N);
rc = [rc, zeros(N, N-7); 162, center_conn];
R = [R, zeros(3, 1)];

N = size(rc, 1);

fname_conn = ['conn_sphere_', num2str(N), '.txt'];
fname_pos = ['pos_sphere_', num2str(N), '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);


figure(1); clf; hold on; grid on;
scatter3(vert(1, :), vert(2, :), vert(3, :), 'r');
axis([-1, 1, -1, 1, -1, 1]*1.4)
axis square