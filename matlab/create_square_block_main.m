clc, clear

n = 6;
rng(1);

x = linspace(0, 1, 5);
y = linspace(0, 1, 5);
z = linspace(0, 1, 5);

dx = x(2) - x(1);
[x, y, z] = meshgrid(x, y, z);

R = [x(:)'; y(:)'; z(:)'];
N = size(R, 2);
rc = zeros(N, n+1);

for k = 1:N
    
    rk = R(:, k);
    d2 = [rk(1) - R(1, :);
          rk(2) - R(2, :);
          rk(3) - R(3, :)];
    d = sum(sqrt(d2.*d2), 1);
    [s, sidx] = sort(d);
    

    sidx = sidx(2:n+1);
    cs = d(sidx);
    chosen_idx = sidx(cs < dx*1.5);
    rc(k, 1:size(chosen_idx, 2)+1) = [size(chosen_idx, 2), chosen_idx];
    aa = 1;
    
end

ecg = create_edge_connectivity(rc, N);

axl = 1;

figure(1); clf; hold on; grid on;
scatter3(R(1, :), R(2, :), R(3, :), 'r');

for k = 1:N
    cidx = rc(k, :);
    rk = R(:, k);
    for a = 2:cidx(1)+1
        rkc = R(:, cidx(a));
        
        plot3([rk(1), rkc(1)], ...
               [rk(2), rkc(2)], ...
               [rk(3), rkc(3)], 'b');
        
    end
    
end


plot3([-axl, axl], [0, 0], [0, 0], 'k--')
plot3([0, 0], [-axl, axl], [0, 0], 'k--')
plot3([0, 0], [0, 0], [-axl, axl], 'k--')
axis([-1, 1, -1, 1, -1, 1]*axl)
axis square


for k = 1:size(ecg, 1)
    
    i1 = ecg(k, 1);
    i2 = ecg(k, 2);
    p1 = R(:, i1);
    p2 = R(:, i2);
    
    plot3([p1(1), p2(1)], [p1(2), p2(2)], [p1(3), p2(3)], 'r--');
    
end


%% Create txt file

fname_conn = ['conn_cube_', num2str(N), '.txt'];
fname_pos = ['pos_cube_', num2str(N), '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);



