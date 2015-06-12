%aMega = arduino('/dev/tty.usbmodemfd121', 'Mega2560');
aMega = arduino();

% Preparing Calibration MUX
configurePin(aMega,'D2', 'DigitalOutput');
configurePin(aMega,'D3', 'DigitalOutput');
configurePin(aMega,'D4', 'DigitalOutput');
writeDigitalPin(aMega,'D3', 0);
writeDigitalPin(aMega,'D4', 0);
writeDigitalPin(aMega, 'D2', 1); % Routing to Calibration Resistor!

% Preparing Tomography two MUXes
configurePin(aMega, 'D22','DigitalOutput');
configurePin(aMega, 'D23','DigitalOutput');
configurePin(aMega, 'D24','DigitalOutput');
configurePin(aMega, 'D25','DigitalOutput');
configurePin(aMega, 'D26','DigitalOutput');
configurePin(aMega, 'D27','DigitalOutput');
configurePin(aMega, 'D28','DigitalOutput');
configurePin(aMega, 'D29','DigitalOutput');
configurePin(aMega, 'D30','DigitalOutput');
configurePin(aMega, 'D31','DigitalOutput');
writeDigitalPin(aMega, 'D22', 1);
writeDigitalPin(aMega, 'D27', 1); % Turning off now

% Initialization Paramters - I2C Start!
aAD5933 = i2cdev(aMega, '0xD');