clear
run('ArduinoInitializeScript.m');

%variables
calResistance = 806;

% Disable Ext. Clock
opClock = setExtClock(aAD5933, false);

% Reset AD5933
resetAD5933(aAD5933);

% Starting Freq Setting
setStartFreq(aAD5933,opClock,50000);

% Cycles & Multiply
setSettlingCycles(aAD5933, 511, 4);

% Set step size
setStepSizeInHex(aAD5933, 1);

% Setting # of increment
setNumofIncrement(aAD5933, 2);

% set Range - Voltage Signal Amplitude
setRange(aAD5933, 4);

% set PGA Gain
setPGA(aAD5933, 1);

% get Temperature
curTemp = getTemperature(aAD5933)

% Calibration!
switchCalibrationMUX(aMega,1);
[ gainFactor, pShift ] = getGainFactor(aAD5933, calResistance, 10, false)
switchCalibrationMUX(aMega,4);

updateTemperature(aAD5933);
setCtrMode(aAD5933, 'REPEAT_FREQ');
[ impVal, phase ] = getComplex(aAD5933, gainFactor, pShift)

%clear aMega aAD5933