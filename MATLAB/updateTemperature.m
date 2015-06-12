function updateTemperature( aAD5933 )
%UNTITLED4 ? ??? ?? ?? ??
%   ??? ?? ??
setCtrMode(aAD5933, 'TEMP_MEASURE');

while( bitand(getStatusReg(aAD5933),1) ~= 1 )
     % wait!
end

end

