## Execution

Execute [sensor.py](sensor.py) with `sudo python3 ./sensors.py` to read out the Sensor Board.  
In the [main.cpp](main.cpp#28) file you can select the employed Board Version (3 or 4).

### Light sensor calibration

If the first led (the closest to the screen) shines green, that means the light sensor
is in calibration mode. By pressing the Joystick down the light sensor reads the current value and saves it as 
lower baseline. The second led should now be activated and shining red. The first led is turned of now.  
By pressing the joystick again the light sensor reads out the current value a second time and stores
it as upper base line. The second led should now be turned off.
If for whatever reason a an illegal state is reached (the light sensor is model as a finet state machine)
both leds shine at the same time. This should never happen.  
After calibration the light sensor is now able to read out the current values as percent regarding 
the upper and lower baselines.  
You can always calibrate it again by simplly pressing the joystick key again which starts the 
calibration mode.  

### UART commands for the microcontroller

The microcontroller receives commands by a one byte message.
0x1 represents the accelerometer, 0x2 represents the magnetometer and 0x03 represents the light sensor.
If a different command is issued the microcontroller respons with `error, wrong command!` . Each line
including the sensor read outs are terminated by a new line charater `\n` .  
For an example implementation have a look at [sensor.py](sensor.py) .
