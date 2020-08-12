#!/usr/bin/python3

import serial
import random
import threading
import time

port = serial.Serial("/dev/serial0", baudrate=921600, timeout=0.5)
send = 0
received = 0
err = [0]


def send_t():
    global send
    for i in range(100):
        port.write(bytes([i]))
        time.sleep(0.005)
        send = send + 1


def read_t(port, err):
    global received
    for i in range(100):
        read = port.read(1)
        received = received + 1
        if bytes([i]) != read:
            print('error encountered, expected ' +
                  str(bytes([i])) + ' but got ' + str(read))
            err[0] = err[0] + 1


st = threading.Thread(target=send_t)
rt = threading.Thread(target=read_t, args=(port, err))

st.start()
rt.start()

while rt.isAlive() or st.isAlive():
    pass

print('Send Packages: ', send)
print('Received Packages: ', received)
print('Error rate = ', str(err[0]), '%')
