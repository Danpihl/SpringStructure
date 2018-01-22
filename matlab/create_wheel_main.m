clc, clear

Np = 20;
ri = 2.5;
ro = 3.1;
dz = 1;
phi = linspace(0, 2*pi - 2*pi/Np, Np);


xo = ro*cos(phi);
yo = ro*sin(phi);

xi = ri*cos(phi);
yi = ri*sin(phi);
X = [xi, xo];
Y = [yi, yo];
Z = zeros(size(X));
X = [X, X];
Y = [Y, Y];
Z = [Z, ones(size(Z))*dz];
N = size(Z, 2);
rc = cell(N, 1);
max_length = 0;

% 11

for k = 1:N
    
    R = [X(k) - X;
         Y(k) - Y;
         Z(k) - Z];
    d = sum(R.*R, 1);
    [s, sidx] = sort(d);
    s = s(2:end);
    sidx = sidx(2:end);
    
    connidx = sidx(1:11);
    
    rc{k} = [rc{k}, connidx];

    if length(rc{k}) > max_length
        max_length = length(rc{k});
    end
    
end

rcm = zeros(N, max_length+1);

for k = 1:N
    rcm(k, 1:(length(rc{k})+1)) = [length(rc{k}), rc{k}];
end
rc = rcm;

R = [X; Y; Z];

figure(1); clf; hold on; grid on;
% scatter(xi, yi);
% scatter(xo, yo);
scatter3(X, Y, Z)
axis equal

for k = 1:N
    p = R(:, k);
    
    for a = 1:rc(k, 1)
        p0 = R(:, rc(k, a+1));
        plot3([p(1), p0(1)], ...
              [p(2), p0(2)], ...
              [p(3), p0(3)], 'r');
        
    end
    
end

fname_conn = ['conn_wheel', '.txt'];
fname_pos = ['pos_wheel', '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);
