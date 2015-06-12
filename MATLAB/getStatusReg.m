function statusReg = getStatusReg( aAD5933 )
%Obtaining status reg value
%returns the value of lower three digits at Register 0x8F
%
statusReg = readRegister(aAD5933, hex2dec('8F'));
statusReg = bitand(statusReg, 7);
end

