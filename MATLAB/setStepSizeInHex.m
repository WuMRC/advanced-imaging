function setStepSizeInHex( aAD5933, freqHex )
if freqHex > hex2dec('FFFFFF')
    Disp('Step Size Frequency Overflow!');
end
lowerHex = mod(freqHex,256);
writeRegister(aAD5933, hex2dec('87'), lowerHex );
writeRegister(aAD5933, hex2dec('86'), mod( bitshift(freqHex - lowerHex, -8), 256) );
writeRegister(aAD5933, hex2dec('85'), bitshift(freqHex, -16) );

end