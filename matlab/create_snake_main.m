clc, clear

Nc = 6;
r = 1;
Np = 10;

phi = linspace(0, 2*pi - 2*pi/Nc, Nc);

xc = r*cos(phi);
yc = r*sin(phi);
dz = 1;

X = [];
Y = [];
Z = [];

for k = 1:Np
    
    X = [X, xc];
    Y = [Y, yc];
    Z = [Z, (k-1)*dz*ones(1, Nc)];
    
    
end
X = [X, zeros(1, Np)];
Y = [Y, zeros(1, Np)];
Z = [Z, (0:(Np-1))*dz];
N = size(X, 2);


rc = cell(N, 1);
max_length = 0;

for k = 1:N
    
    R = [X(k) - X;
         Y(k) - Y;
         Z(k) - Z];
    d = sum(R.*R, 1);
    [s, sidx] = sort(d);
    s = s(2:end);
    sidx = sidx(2:end);

    connidx = sidx(s < 1.01);

    rc{k} = [rc{k}, connidx];
    
    if (abs(X(k)) < 0.01) && (abs(Y(k)) < 0.01) % Center
        
        
        aa = 1;
%         rc{k} = [rc{k}, connidx];
%         R = [X(k) - X;
%              Y(k) - Y;
%              Z(k) - Z];
%         d = sum(R.*R, 1);
%         [s, sidx] = sort(d);
%         s = s(2:end);
%         sidx = sidx(2:end);
        
    else  % Not center
        aa = 1;
        
    end
    
    if length(rc{k}) > max_length
        max_length = length(rc{k});
    end
    
end

rcm = zeros(N, max_length+1);

for k = 1:N
    rcm(k, 1:(length(rc{k})+1)) = [length(rc{k}), rc{k}];
    
end
rc = rcm;
%%

R = [X; Y; Z];

figure(1); clf; hold on; grid on;
scatter3(X, Y, Z)
axis equal
xlabel('x');
ylabel('y');
zlabel('z');

for k = 1:N
    p = R(:, k);
    
    for a = 1:rc(k, 1)
        p0 = R(:, rc(k, a+1));
        plot3([p(1), p0(1)], ...
              [p(2), p0(2)], ...
              [p(3), p0(3)], 'r');
        
    end
    
end

fname_conn = ['conn_snake', '.txt'];
fname_pos = ['pos_snake', '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);

