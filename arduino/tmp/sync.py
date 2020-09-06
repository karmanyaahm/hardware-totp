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
DONE_HEADER = "O"
READY_HEADER = "setup"


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
    print(int(time.time()))
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
    o = ""
    while DONE_HEADER not in o:
        o = [a.decode().strip() for a in s.readlines()]
        for i in o:
            print(i)


def addSmth(name: str, code: bytes, l: int = 10):
    assert len(name) <= 10
    assert len(code) <= 10
    payload = "A" + name + ":" + (b"\0" * (l - len(code)) + code).hex()
    payload = payload.encode()
    send_stuff(s, payload)


def wait_begin():
    o = ""
    while READY_HEADER not in o:
        o = []
        for a in s.readlines():
            try:
                o.append( a.decode().strip())
            except UnicodeDecodeError:
                pass
        for i in o:
            print(i)


def remove(i: int):
    send_stuff(s, ("R" + str(i - 1)).encode())


def reset():
    remove(-69 + 1)
    readlines()
    addSmth("Timeis", b"default")


s = Serial(port, baud, timeout=2)
import base64


def main():
    reset()
    readlines()
    addSmth("TingTest", base64.b32decode("KT7D6UTLYKSNPIWV"), 10)
    readlines()
    readlines()
    # save_changes()
    # readlines()
    # send_time()
    # readlines()
    # get_time()
    # readlines()
    # # delay(0.1)
    # print(base64.b32decode("KT7D6UTLYKSNPIWV").hex())
    # # remove(3)

    # get_services()
    # readlines()

    # save_changes()

    # readlines()
    # addSmth("string", b"hellowo")
    # readlines()
    # get_services()
    # save_changes()


if __name__ == "__main__":
    wait_begin()
    main()
