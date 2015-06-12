function switchCalibrationMUX(aArduino, num )
% Do switching Calibration MUX (ADG809)
% 
writeDigitalPin(aArduino, 'D2', 0);

switch num
    case 1
        tA0 = 0;
        tA1 = 0;
    case 2
        tA0 = 1;
        tA1 = 0;
    case 3
        tA0 = 0;
        tA1 = 1;
    case 4
        tA0 = 1;
        tA1 = 1;
end

writeDigitalPin(aArduino, 'D3', tA0);
writeDigitalPin(aArduino, 'D4', tA1);
writeDigitalPin(aArduino, 'D2', 1);

end

