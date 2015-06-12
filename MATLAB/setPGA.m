function setPGA( aAD5933, pgaGain )
%Changes the value of the gain at the recieve (input) stage of the AD5933 to one of the 2 selectable gains.
%  
%   This function also lets you overwrite the contents of the remaining bits in the control register with
%   values of your choosing.
%  
%   @Param pgaGain Values of 1 or 5, for each gain respectively.
%   @Param ctrReg The new contents of the control resister to overwrite prior to bit shifting.
tReg1 = readRegister(aAD5933, hex2dec('80'));
tReg1 = bitand(tReg1, hex2dec('FE'));

switch(pgaGain)
    case 1
        tReg1 = bitor(tReg1, 0);
    case 5
        tReg1 = bitor(tReg1, 1);
    otherwise
        error('Invaild PGA Gain');
end
writeRegister(aAD5933,hex2dec('80'),tReg1);


end

