#!/usr/bin/python3

import serial
import time
import struct

port = '/dev/serial0'

ser = serial.Serial(port, baudrate=11500)

while True:
    print(
        "Select a sensor to query [1 - ACCELEROMETER, 2 - MAGNETOMETER or 3 - LIGHT_SENSOR]: ")
    response = None
    while response not in {"1", "2", "3"}:
        response = input("Please enter a number from 1 to 3: ")

    for x in range(0, 100):
        ser.write(struct.pack("B", int(response)))
        packet = ser.readline()
        print(packet[:-1].decode("utf-8"))
        time.sleep(0.1)
