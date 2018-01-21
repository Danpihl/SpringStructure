function ecg = create_edge_connectivity(rc, N)

ecg = [];



for k = 1:N
    
    % Number of points
    n = rc(k, 1);
    % Connected indices
    cidx = rc(k, 2:n+1);
    
    for a = 1:n
        % Create some unique index
        
        % Check if edge exists already
        Ne = size(ecg, 1);
        
        e = 1;
        
        for i = 1:Ne
            ce = ecg(i, :);
            
            if ((ce(1) == k) && (ce(2) == cidx(a))) || ((ce(2) == k) && (ce(1) == cidx(a)))
                e = 0;
            end
            
        end
        
        % If edge doesn't already exist, add it to ecg
        if e == 1
            ecg = [ecg; [k, cidx(a)]];
        end
        
        
        
    end
    
    
    

end

end