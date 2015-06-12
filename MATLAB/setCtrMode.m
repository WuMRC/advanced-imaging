function setCtrMode( aAD5933, modeString )
%Setting control mode of AD5933
%
tReg1 = readRegister(aAD5933, hex2dec('80'));
tReg1 = bitand(tReg1, hex2dec('F'));

switch modeString
    case 'INIT_START_FREQ'
        tReg1 = bitor(tReg1, hex2dec('10'));
    case 'START_FREQ_SWEEP'
        tReg1 = bitor(tReg1, hex2dec('20'));
    case 'INCR_FREQ'
        tReg1 = bitor(tReg1, hex2dec('30'));
    case 'REPEAT_FREQ'
        tReg1 = bitor(tReg1, hex2dec('40'));    
    case 'POWER_DOWN'
        tReg1 = bitor(tReg1, hex2dec('A0'));
    case 'STAND_BY'
        tReg1 = bitor(tReg1, hex2dec('B0'));
    case 'TEMP_MEASURE'
        tReg1 = bitor(tReg1, hex2dec('90'));
    otherwise
        returnFlag = false;
        error('Invaild usage of the setCtrlMode - invaild modeString (2nd argument)');
        return;
end

writeRegister(aAD5933,hex2dec('80'),tReg1);

end

