function curTemp = getTemperature(aAD5933)
updateTemperature(aAD5933);
tTemp(1) = double(readRegister(aAD5933, hex2dec('92')));
tTemp(2) = double(readRegister(aAD5933, hex2dec('93')));
tTempVal = (mod(tTemp(1), 12*16) * (16^2)) + tTemp(2);
signBit = mod(bitshift(tTemp(1), -5),2);
if signBit == 0
    % Positive
    curTemp = tTempVal / 32;
else
    % Negative
    curTemp = (tTempVal-16384) / 32;
end

end