function setRange( aAD5933, range )
%Changes the value of the excitation volatage at the transmit (output) stage of the AD5933 to one of the 4 selectable ranges.
%  
%  This function also lets you overwrite the contents of the remaining bits in the control register with values of your choosing.
%  
%  @Param rangeToSet Values of 1 - 4, for each excitation range respectively.
%  @Param ctrReg The new contents of the control resister to overwrite prior to bit shifting.
%  
tReg1 = readRegister(aAD5933, hex2dec('80'));
tReg1 = bitand(tReg1, hex2dec('F9'));

switch(range)
    case 1
        tReg1 = bitor(tReg1, 0);
    case 2
        tReg1 = bitor(tReg1, 6);
    case 3
        tReg1 = bitor(tReg1, 4); % Range 3
    case 4
        tReg1 = bitor(tReg1, 2); % Range 4    
end
writeRegister(aAD5933,hex2dec('80'),tReg1);



end

