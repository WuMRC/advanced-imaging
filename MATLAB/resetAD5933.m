function resetAD5933( aAD5933 )
% Resetting AD5933
%   
tReg1 = bitand(readRegister(aAD5933,hex2dec('81')), 4);
writeRegister(aAD5933,hex2dec('81'), bitor(tReg1 , hex2dec('10')) );

end

