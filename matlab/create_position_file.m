function create_position_file(fname, N, R)

fid = fopen(fname, 'w+');

for k = 1:N
%     if k ~= N
        s = [num2str(R(1, k)), ',', num2str(R(2, k)), ',', num2str(R(3, k)), '\n'];
%     else
%         s = [num2str(R(1, k)), ',', num2str(R(2, k)), ',', num2str(R(3, k))];
%     end
    
    fprintf(fid, s);
    
end

fclose(fid);

end