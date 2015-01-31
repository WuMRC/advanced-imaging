
a = arduino;

% MUX control pins
as0 = 11; as1 = 10; as2 = 9; as3 = 8;
bs0 = 7; bs1 = 6; bs2 = 5; bs3 = 4;

configureDigitalPin(a,as0,'output');
configureDigitalPin(a,as1,'output');
configureDigitalPin(a,as2,'output');
configureDigitalPin(a,as3,'output');
configureDigitalPin(a,bs0,'output');
configureDigitalPin(a,bs1,'output');
configureDigitalPin(a,bs2,'output');
configureDigitalPin(a,bs3,'output');

% MUX in "SIG" pin
SIG_pinA = 0;
SIG_pinB = 1;

writeDigitalPin(a,as0,0);
writeDigitalPin(a,as1,0);
writeDigitalPin(a,as2,0);
writeDigitalPin(a,as3,0);
writeDigitalPin(a,bs0,0);
writeDigitalPin(a,bs1,0);
writeDigitalPin(a,bs2,0);
writeDigitalPin(a,bs3,0);

controlPinA = [as0, as1, as2, as3];
controlPinB = [bs0, bs1, bs2, bs3];

muxChannel = logical([0,0,0,0; % channel 0
    1,0,0,0; %channel 1
    0,1,0,0; %channel 2
    1,1,0,0; %channel 3
    0,0,1,0; %channel 4
    1,0,1,0; %channel 5
    0,1,1,0; %channel 6
    1,1,1,0; %channel 7
    0,0,0,1; %channel 8
    1,0,0,1; %channel 9
    0,1,0,1; %channel 10
    1,1,0,1; %channel 11
    0,0,1,1; %channel 12
    1,0,1,1; %channel 13
    0,1,1,1; %channel 14
    1,1,1,1;]);  %channel 15


%% The basic loop

i = 0; j = 0;

while i < 16
    j = i+1;
    while j < 16
        for n = 1:4
            writeDigitalPin(a,controlPinA(n),muxChannel(i,n))
        end
        
        for m = 1:4
            writeDigitalPin(a,controlPinB(m),muxChannel(j,m))
        end
        
        valA = readVoltage(a,SIG_pinA);
        valB = readVoltage(a,SIG_pinB);
        
        j = j+1;
    end
    i = i+1;
end


