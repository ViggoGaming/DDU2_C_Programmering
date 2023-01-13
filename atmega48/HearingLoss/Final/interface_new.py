import time
import serial  # pip install pyserial
import codecs


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


# End Of Transmission Character
EOT = ':'
EOL = ';'
# Timeout in milliseconds

ser = serial.Serial(
    # COM port på windows
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=None,
)

ser.flushInput()
ser.reset_input_buffer()


def readMessage():
    message = ""
    while True:
        char = ser.read()
        if char == EOT:
            return message.decode("utf-8")
        message += char


print(bcolors.BOLD + "Valdemar 3.E & Victor 3.M's høretabs projekt \n" + bcolors.ENDC)


while True:
    # Write
    command = (str(input(bcolors.HEADER + "> " + bcolors.ENDC)).lower())
    # unencoded
    if command == "help" or command == "?":
        print(r""" 
        Help menu:
        - lights [enabled]
            Set button lights on / off.
        - tone [duration] [frequency]
            Play a tone through headphones with a given frequency and duration.
        - test [ear] [frequncy]
            Test an ear with a given frquency. Return user input.
        - setEar [left/right]
            Turns on left or right ear.
        """)
    else:
        ser.write((command+EOT+EOL).encode())
        print((command+EOT+EOL).encode())

        # Listen
        while True:
            # All lines are terminated with \n
            line = ser.read_until(';').decode("utf-8")[:-1]
            print(f"Linje modtaget: {line}")
            print(bcolors.OKGREEN + line + bcolors.ENDC, end='\n')
            # If the last character (before \n) is the EOT, we stop listening
           # if str(line)[-1] == EOT.decode("utf-8"):
           #     break
