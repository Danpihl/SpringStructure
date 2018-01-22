clc, clear

load('d2.mat');
conn = conn';
Np = 45;
l = 0.1;

conn2 = cell(size(conn, 1)+Np, 1);

for k = 1:size(conn, 1)
    
    c = conn(k, :);
    
    if c(end) == -1
        c = c(1:end-1);
    end
    
    conn2{k} = c;
    
end

x = (0:(Np-1))*l + 1+l;
y = zeros(1, Np);
z = zeros(1, Np);

R = [vert, [x; y; z]];


% Ball to chain:
conn2{size(vert, 2)} = [conn2{size(vert, 2)}, size(vert, 2)+1];

for k = (size(vert, 2)+1):(size(vert, 2)+Np-1)
    
    conn2{k} = [conn2{k}, k-1, k+1];
    
end

conn2{end} = [conn2{end}, size(conn2, 1)-1];



conn = conn2;

max_length = 0;
N = size(conn, 1);

for k = 1:N
    conn{k} = unique(conn{k});
    if length(conn{k}) > max_length
        max_length = length(conn{k});
    end
end

rc = zeros(N, max_length+1);

for k = 1:N
    
    rc(k, 1:(length(conn{k})+1)) = [length(conn{k}), conn{k}];
end

figure(1); clf; hold on; grid on;
scatter3(x, y, z);
scatter3(R(1, :), R(2, :), R(3, :));
scatter3(R(1, end), R(2, end), R(3, end), 'k');

for k = 1:N
    p = R(:, k);
    
    for a = 1:rc(k, 1)
        p0 = R(:, rc(k, a+1));
        plot3([p(1), p0(1)], ...
              [p(2), p0(2)], ...
              [p(3), p0(3)], 'r');
        
    end
    
end

fname_conn = ['conn_ball_chain', '.txt'];
fname_pos = ['pos_ball_chain', '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);
