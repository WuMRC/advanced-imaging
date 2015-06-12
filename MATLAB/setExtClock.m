function opClock = setExtClock( aAD5933, flagExtClock, varargin )
% Setting the clock source
% True = External / False = Internal
% Returns clock speed. (Assuming External clock is 16Mhz if not specified)
% Example: opClock = setExtClock( aAD5933, true, 8000000)

if nargin == 3
    extClockRate = varargin{1};
elseif nargin == 2
    extClockRate = 16000000;
else
    error('Extra Argument as External Clock Rate should be only one.');
end

switch flagExtClock
    case true
        writeRegister(aAD5933,hex2dec('81'),4);
        opClock = extClockRate;
    case false
        writeRegister(aAD5933,hex2dec('81'),0);
        opClock = 16776000;
    otherwise
        error('flagExtClock should be T/F');
end


end

