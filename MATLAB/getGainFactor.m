function [ gainFactor, pShift ] = getGainFactor( aAD5933, calResistance, numCalSamples, varargin )
% Calculates a single gain factor for a single frequency in bi-polar mode. The AD5933 for consecutive 
% measurements must set the control mode to repeat frequency for accurate measurements. (setCtrMode(REPEAT_FREQ)
%  
%   @Param calResistance The calibration resistance
%   @Param numCalSamples The number of times each gain factor should be measured, and then averaged by.
%   @Param gainFactor The double into which the gain factor is saved
%   @Param pShift The array into which the system phase shift is saved
%   @Param retStandBy A boolean instructing the AD5933 to enter standby
%   mode after calculation or not. (true is default)
setCtrMode(aAD5933,'STAND_BY');
setCtrMode(aAD5933,'INIT_START_FREQ');
setCtrMode(aAD5933,'START_FREQ_SWEEP');

tMeas = zeros(1,numCalSamples);
tMeaPhase = zeros(1,numCalSamples);
for t1 = 1:numCalSamples
    [realComp, imagComp] = getComplexRawOnce(aAD5933);
    tMeas(t1) = norm( [realComp imagComp] );
    tMeaPhase(t1) = atan2(imagComp, realComp);
    setCtrMode(aAD5933,'REPEAT_FREQ');
end
mag = mean(tMeas);
pShift = mean(tMeaPhase);
gainFactor = mag * calResistance; 
%Gain Factor is different from one of the datasheet in this program. Reciprocal Value.

repeatFlag = false;
switch nargin
    case 3
        repeatFlag = false;
    case 4
        if varargin{1} == true
            repeatFlag = false;
        else
            repeatFlag = true;
        end
    otherwise
        error('Extra Argument should be single. - retStandBy, which is true or false');
end

switch repeatFlag
    case true
        return;
    case false
        setCtrMode(aAD5933, 'STAND_BY');
        resetAD5933(aAD5933);
end


end

