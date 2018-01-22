function create_connectivity_file(fname, N, rc)

fid = fopen(fname, 'w+');

for k = 1:N
    
    % Number of points
    n = rc(k, 1);
    % Connected indices
    cidx = rc(k, 2:n+1);
    s = [num2str(n), ','];
    
    for a = 1:n
        if a ~= n
            s = [s, num2str(cidx(a)-1), ','];
        else
            s = [s, num2str(cidx(a)-1)];
        end
    end
%     if k ~= N
        fprintf(fid, [s, '\n']);
%     else
%         fprintf(fid, s);
%     end
    
end

fclose(fid);

end