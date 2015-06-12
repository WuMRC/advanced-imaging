function [ impVal, phase ] = getComplex( aAD5933, gainFactor, pShift )
%Gets the impedance magnitude and phase angle for the AD5933 in a bi-polar configuration.
%  
%   @Param gainFactor The gainfactor caluclulated at calibration
%   @Param pShift The system phase shift calculated at calibration
%   @Param impVal The magnitude of the impedance
%   @Param phase The phase angle of the impedance

[rComp, iComp] = getComplexRawOnce(aAD5933);
magSq = norm( [rComp iComp] );
impVal = gainFactor / magSq;
phase = atan2(iComp, rComp) - pShift;

end

