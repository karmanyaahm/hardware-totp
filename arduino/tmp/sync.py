import time
from datetime import datetime
from serial import Serial  # pySerial

port = "/dev/ttyUSB0"
baud = 9600

TIME_HEADER = "T"
GET_TIME_HEADER = "G"
LIST_HEADER = "L"


def send_stuff(serial, payload):
    try:
        payload = payload.encode()
    except TypeError:
        pass

    print("sent")
    serial.write(payload)
    print(payload)


def send_time():

    s.read()  # read sync signal

    # attempt to set as close to on the second as possible
    now = time.time()
    time.sleep(now - int(now))
    payload = TIME_HEADER + str(int(now))
    send_stuff(s, payload)


def get_time():
    send_stuff(s, GET_TIME_HEADER)


def get_services():
    send_stuff(s, LIST_HEADER)
    b = s.readline().decode().strip()
    b = b.split(";")
    print("Services")
    for a, c in enumerate(b):
        print(a + 1, c)
    print()


def readlines():
    o = [a.decode().strip() for a in s.readlines()]
    for i in o:
        print(i)


s = Serial(port, baud, timeout=2)


def main():

    readlines()  # wait to initialize

    get_services()
    readlines()


if __name__ == "__main__":
    main()
