function setNumofIncrement( aAD5933, num )
%Sets the number of frequency increments in a frequency sweep. The AD5933 is rated for 1 - 100 KHZ.
%  You may go higher at the risk of larger errors.
%  
%  NOTE: This is the number of increments, not the total number of points in the sweep. An Increment of 5
%  means a total of 6 frequency points.
%  
%  @Param num Number of increments

if num > hex2dec('1FF')+1
    Disp('Number of Increment Overflow!');
end
lowerHex = mod(num,256);
upperHex = mod(bitshift(num,-8),2);
writeRegister(aAD5933, hex2dec('88'), upperHex);
writeRegister(aAD5933, hex2dec('89'), lowerHex);

end

