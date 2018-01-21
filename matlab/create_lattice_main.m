clc, clear

Nz = 3;   % Number of layers
Nx = 3;
Ny = 10;

l = 0.5;
h = sqrt(l^2 - (l^2)/4);
h3 = l/2*tan(pi/6);
dl = sqrt( (l/2)^2 + h3^2);
hz = sqrt(l^2 - dl^2);

px = (0:(Nx-1))*l;

Px = [];
Py = [];

for k = 1:Ny
    
    if mod(k, 2) == 1
        Px = [Px, px];
    else
        Px = [Px, px+l/2];
    end
    Py = [Py, (k-1)*h*ones(1, Nx)];
end

Pxf = [];
Pyf = [];
Pzf = [];

% h2 = ;

for k = 1:Nz
    
    if mod(k, 2) == 1
        Pxf = [Pxf, Px];
        Pyf = [Pyf, Py];
    else
        Pxf = [Pxf, Px+l/2];
        Pyf = [Pyf, Py+h3];
    end
    
    Pzf = [Pzf, (k-1)*hz*ones(1, size(Px, 2))];
    
end


a = 0.2;

% 0.01
N = size(Pxf, 2);
rc = zeros(N, 13);

for k = 1:N
    
    R = [Pxf(k) - Pxf;
         Pyf(k) - Pyf;
         Pzf(k) - Pzf];
    d = sum(R.*R, 1);
    [s, sidx] = sort(d);
    s = s(2:end);
    sidx = sidx(2:end);
    aa = 1;
    connidx = sidx(s < (l^2)*1.01);
    rc(k, 1:(size(connidx, 2)+1)) = [size(connidx, 2), connidx];
    
%     s(1:15)
    
end

R = [Pxf;
     Pyf;
     Pzf];
figure(2); clf; hold on; grid on;

scatter3(Pxf, Pyf, Pzf);
scatter3(Pxf(k), Pyf(k), Pzf(k), 'r');

for k = 1:N
    p = R(:, k);
    
    for a = 1:rc(k, 1)
        p0 = R(:, rc(k, a+1));
        plot3([p(1), p0(1)], ...
              [p(2), p0(2)], ...
              [p(3), p0(3)], 'r');
        
    end
    
end

% axis([-1, 1, -1, 1]*a)
% axis square
axis equal

%%

fname_conn = ['conn_lattice', '.txt'];
fname_pos = ['pos_lattice', '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);
