function [ realComp, imagComp ] = getComplexRawOnce( aAD5933 )
%Gets the raw values of the DFT from the AD5933 after calculation
%
% @Param realComp The real component of the DFT
% @Param imagComp The imaginary component of the DFT

% Assuming that the measurement is started - at START_FREQ_SWEEP
while( bitand(getStatusReg(aAD5933),2) ~= 2 )
end

tRm = readRegister(aAD5933, hex2dec('94'));
tRl = readRegister(aAD5933, hex2dec('95'));
realComp = double(typecast( [tRl tRm], 'int16'));

tIm = readRegister(aAD5933, hex2dec('96'));
tIl = readRegister(aAD5933, hex2dec('97'));
imagComp = double(typecast( [tIl tIm], 'int16'));

end

