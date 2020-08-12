import serial
import time
import struct
import csv
import matplotlib.pyplot as plt

port = '/dev/serial0'
ser = serial.Serial(port, baudrate=115200)

data_count = 0
fieldnames = ['time', 'X', 'Y', 'Z']
t = time.time()


if __name__ == "__main__":
    data = []

    for i in range(0, 10000):
        ser.write(struct.pack("B", 1))
        line = ser.readline()

        data_point = eval(line[:-1].decode("utf-8"))
        data_point['time'] = time.time() - t
        data.append(data_point)

        data_count += 1
        print("Read {} datapoints".format(data_count))

    with open('data.csv', mode='w') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()

        for data_point in data:
            writer.writerow(data_point)

    print("wrote {} data points to csv".format(data_count))

    for achses in ["X", "Y", "Z"]:
        plt.clf()
        time = []
        acceleration = []

        for data_point in data:
            time.append(data_point['time'])
            acceleration.append(data_point[achses])

        plt.plot(time, acceleration, label='Acceleration')
        plt.xlabel('time')
        plt.ylabel('Acceleration')
        plt.title('Acceleration Data from {} achses'.format(achses))
        plt.savefig('{}.png'.format(achses), dpi=1200)
        print("wrote {} achses plot".format(achses))
