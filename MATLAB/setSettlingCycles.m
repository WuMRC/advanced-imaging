function setSettlingCycles( aAD5933, cycles, multiply )
%Setting number of measurement cycles and multiplied number
%Sets the number of settling cycles before a measurement is taken.
% @Param cycles Cycles to ignore before a measurement is taken. Max is 511.
% @Param mult Multiplier for cycles. Can be 1, 2, or 4.

lowerHex = mod(cycles, 256);
upperHex = mod(bitshift(cycles-lowerHex,-8),2);
upperHex = bitor(upperHex, bitshift(multiply-1,1) );
writeRegister(aAD5933, hex2dec('8A'), upperHex );
writeRegister(aAD5933, hex2dec('8B'), lowerHex );

end

