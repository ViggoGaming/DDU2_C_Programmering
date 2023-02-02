import time
import random
import serial  # pip install pyserial
import csv

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

# Test subject profile
profileAge = "Unkown"
profileGender = "Unkown"

# Range for frequencies
rangeTestFreqs = [100, 200, 400, 800, 1600, 3200, 6400, 10000]
numFreqs = len(rangeTestFreqs)

# 2-D array. testResult[Frequency][Ear] = testResult
fullTestResults = [[0]*2 for i in range(numFreqs)]

# Simple test result class.
class TestResult:
    def __init__(self, toneFrequency, reactionTime=0, pressedButton="NONE") -> None:
        self.toneFrequency = toneFrequency
        self.reactionTime = reactionTime
        self.pressedButton = pressedButton # "LEFT", "RIGHT" or "NONE"

# End Of Transmission Character
EOT = '\x04'
EOL = '\n'

ser = serial.Serial(
    # COM port på windows
    port='/COM3',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=None,
)

ser.flushInput()
ser.reset_input_buffer()

def exportResults(name):
    with open("./Results/"+name+".csv", 'w', newline='') as file:
        writer = csv.writer(file, delimiter=';')
        # Write subject profile
        writer.writerow(["Age", "Gender"])
        writer.writerow([profileAge, profileGender])
        # Write test results
        writer.writerow(["Test Frequency", "Test Ear", "Pressed Button", "Reaction Time"])
        for pair in fullTestResults:
            left = True
            for v in pair:
                writer.writerow([v.toneFrequency, ('LEFT' if left else 'RIGHT'), v.pressedButton, v.reactionTime])
                left = False
        

def fullTest():
    testsToDo = [[i, 'LEFT'] for i in range(numFreqs)]+[[i, 'RIGHT'] for i in range(numFreqs)]
    random.shuffle(testsToDo)
    print(testsToDo)
    for test in testsToDo:
        # Write command to do single test
        command = f"test {'right' if test[1]=='RIGHT' else 'left'} {rangeTestFreqs[test[0]]}"
        ser.write((command+EOT+EOL).encode())
        # Read response and print
        line = ser.read_until(EOL.encode()).decode("utf-8")[:-1]
        print(bcolors.OKGREEN + line + bcolors.ENDC + "\n")
        # Save actual frequency. Word 8 is frequency in Hz
        actualFreq = line.split()[7]
        # Read response and write
        resultLine = ser.read_until(EOL.encode()).decode("utf-8")[:-1]
        print(bcolors.OKGREEN + resultLine + bcolors.ENDC + "\n")
        # Save user input
        result = TestResult(actualFreq)
        if "button pressed" in resultLine:
            words = resultLine.split()
            # First word is pressed button, "LEFT" or "RIGHT"
            result.pressedButton = words[0]
            # Fith word is reaction time in ms
            result.reactionTime = words[4]
        # Index is found with [FrequncyIndex][Ear]
        fullTestResults[test[0]][0 if test[1]=="LEFT" else 1] = result
        # Delay next test
        time.sleep(3)
    # Test is over. Blink a little
    for i in range(8):
        ser.write(("lights on"+EOT+EOL).encode())
        time.sleep(0.1)
        ser.write(("lights off"+EOT+EOL).encode())
        time.sleep(0.1)
    print(bcolors.OKGREEN + "Full Test Finished" + bcolors.ENDC + "\n")
    # Print results
    # for pair in fullTestResults:
    #     left = True
    #     for v in pair:
    #         print(f"Frequency: {v.toneFrequency}\tEar: {'LEFT' if left else 'RIGHT'}\tPressed Button: {v.pressedButton}\tReactionTime: {v.reactionTime}")
    #         left = False

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
        - fullTest
            Starts a full test on both ears (100 Hz - 10000 Hz).
        - setProfile [Age] [M/F]
            Updates test subject profile.
        - exportResults [Name]
            Exports results along with subject profile in a csv.
        """)
    elif command == "fulltest":
        fullTest()
    elif "setprofile" in command:
        words = command.split()
        profileAge = words[1]
        profileGender = words[2]
    elif "exportresults" in command:
        words = command.split()
        name = "temp"
        if len(words) > 1:
            name = words[1]
        exportResults(name)
    else:
        ser.write((command+EOT+EOL).encode())
        print((command+EOT+EOL).encode())

        # Listen
        while True:
            # All lines are terminated with \n
            line = ser.read_until(EOL.encode()).decode("utf-8")[:-1]
            print(bcolors.OKGREEN + line + bcolors.ENDC + "\n")
            # If the last character (before \n) is the EOT, we stop listening
            if line[-1] == EOT:
                break
