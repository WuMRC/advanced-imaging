function setStepSize( aAD5933, opClock, increment )
%  Sets the step size for a frequency sweep in hertz.
%  
%   NOTE: because the increment frequency should be converted into unique hexadecimal number,
%   it approximately calculates the Hex value.
%  
%   @Param increment The stepSize in hertz

freqHex = floor(increment / (opClock / 2^29));
setStepSizeInHex( aAD5933, freqHex );

end

