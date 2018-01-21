clc, clear

N = 10;
n = 9;
rng(1);
R = (rand(3, N)-0.5)*2;
% rc = zeros(N, n*2+1);
rc = cell(N, 1);


for k = 1:N
    
    rk = R(:, k);
    d2 = [rk(1) - R(1, :);
          rk(2) - R(2, :);
          rk(3) - R(3, :)];
    d = sum(sqrt(d2.*d2), 1);
    [s, sidx] = sort(d);
    
%     idx = randi(n, [1, 1]);
%     idx = max([idx, n-2]);
    idx = n;
    sidx = sidx(2:end);

%     rc(k, 1:idx+1) = [idx, sidx(1:idx)];
    rc{k} = [rc{k}, sidx(1:idx)];
    
    for a = sidx(1:idx)
        rc{a} = [rc{a}, k];
    end
    
    aa = 1;
    
end

ml = 0;

for k = 1:N
    rc{k} = unique(rc{k});
    rc{k} = [length(rc{k}), rc{k}];
    if length(rc{k}) > ml
        ml = length(rc{k});
    end
end

rcm = zeros(N, ml);

for k = 1:N
    
    rcm(k, 1:length(rc{k})) = rc{k};
end
rc = rcm;

%%

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

fname_conn = ['conn_rand', '.txt'];
fname_pos = ['pos_rand', '.txt'];
create_connectivity_file(fname_conn, N, rc);
create_position_file(fname_pos, N, R);



