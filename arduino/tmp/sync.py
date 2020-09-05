import time
from time import sleep as delay
from datetime import datetime
from serial import Serial  # pySerial

port = "/dev/ttyUSB0"
baud = 9600

TIME_HEADER = "T"
GET_TIME_HEADER = "G"
LIST_HEADER = "L"
SAVE_HEADER = "S"


def send_stuff(serial, payload):
    try:
        payload = payload.encode()
    except (TypeError, AttributeError):
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


def save_changes():
    send_stuff(s, SAVE_HEADER)


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


def addSmth(name: str, code: bytes):
    assert len(name) <= 10
    assert len(code) <= 10
    payload = "A" + name + ":" + code.decode().ljust(10, "\0").encode().hex()
    payload = payload.encode()
    send_stuff(s, payload)


def remove(i: int):
    send_stuff(s, ("R" + str(i - 1)).encode())


s = Serial(port, baud, timeout=2)


def main():

    readlines()  # wait to initialize

    get_services()
    delay(0.1)
    remove(3)
    delay(0.1)

    get_services()
    delay(0.1)

    save_changes()
    delay(0.1)

    readlines()
    # addSmth("string", b"hellowo")
    # readlines()
    # get_services()
    # save_changes()


if __name__ == "__main__":
    main()
