#!/usr/bin/env python
import time
import serial
import codecs
import webbrowser

ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1                
)

counter=0
rx=""
tx=""
ser.reset_input_buffer()

while True:
    # Læser hvert linje der bliver modtaget fra atmega48 via UART
    # Dataene der modtages dekodes som utf-8 standarden
    rx=str(ser.readline().decode("utf-8"))
    print(rx)
    # Her bliver der tjekket om 
    if rx == "RICKROLL":
        webbrowser.open("https://www.youtube.com/watch?v=dQw4w9WgXcQ")