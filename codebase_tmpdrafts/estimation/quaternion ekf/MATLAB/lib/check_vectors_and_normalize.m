function [B] = check_vectors_and_normalize(C)
    
    B = zeros(1,10);       %check data and normalize it if valid, returns zeros/NaN if corrupted
    B(2) = NaN;
    
    if length(C) == 10              
        a = norm(C(5:7));
        b = norm(C(8:10));
        if (a ~= 0) && (b ~= 0)
            C(5:7) = C(5:7)/a;
            C(8:10) = C(8:10)/b;
            if sum(isnan(C)) == 0
                B = C;
            end
        end
    end
    
end
