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


# End Of Line Character

EOL = ';'

# Timeout in milliseconds

timeout = 100


ser = serial.Serial(

    # COM port på windows
    port='/dev/ttyUSB0',

    baudrate=9600,

    parity=serial.PARITY_NONE,

    stopbits=serial.STOPBITS_ONE,

    bytesize=serial.EIGHTBITS,

    timeout=0.1,

)


ser.flushInput()

ser.reset_input_buffer()


def readMessage():

    message = ""

    while True:

        char = ser.read().decode("utf-8")

        if char == EOL:

            return message

        message += char


print(bcolors.BOLD + "Valdemar 3.E & Victor 3.M's høretabs projekt \n" + bcolors.ENDC)


while True:
    # Write
    command = (str(input(bcolors.HEADER + "> " + bcolors.ENDC)).lower()+EOL)

    if command == "help;" or command == "?;":
        print(r""" 
        Help menu:
        - lights [enabled]
            Sets button lights on / off.
        - tone [duration] [frequency]
            play a tone through headphones with a given frequency and duration
        - test [ear] [frequncy]
            take user input

        """)
    else:
        ser.write(command.encode())
        print(command.encode())

    # Listen
    line = ser.read_until(EOL).decode("utf-8")[:-1]
    print(bcolors.OKGREEN + line + bcolors.ENDC, end='\n')
