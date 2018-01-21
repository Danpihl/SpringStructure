clc

dup = [];
idx = 0;

ecg_temp = ecg;
% ecg_temp = [ecg_temp; ecg_temp(end-4:end, :)];
for k = 1:size(ecg_temp, 1)
    
    ecgc = ecg_temp(k, :);
    
    for a = 1:size(ecg, 1)
        if k ~= a
        
            ecgc_ = ecg_temp(a, :);
            if ((ecgc(1) == ecgc_(1)) && (ecgc(2) == ecgc_(2))) || ...
               ((ecgc(2) == ecgc_(1)) && (ecgc(1) == ecgc_(2)))
                disp('Duplicate')
                idx = idx + 1;
            end
        end
        
    end 
end

disp(num2str(idx))

