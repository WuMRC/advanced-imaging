function setStartFreq( aAD5933, opClock, freqInHz )
% Setting start Frequency
% Need: AD5933 object, Operating Clock, Desired Start Frequency

freqHex = floor(freqInHz / (opClock / 2^29));
if freqHex > hex2dec('FFFFFF')
    Disp('Starting Frequency Overflow!');
end
lowerHex = mod(freqHex,256);
writeRegister(aAD5933, hex2dec('84'), lowerHex );
writeRegister(aAD5933, hex2dec('83'), mod( bitshift(freqHex - lowerHex, -8), 256) );
writeRegister(aAD5933, hex2dec('82'), bitshift(freqHex, -16) );


end

