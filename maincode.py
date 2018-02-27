import serial
import struct
import string
import curses
from curses import wrapper
from time import sleep
import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
TRIG = 23
ECHO = 24
#TRIG is PIN16
#ECHO is PIN18
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)
ser = serial.Serial("/dev/ttyAMA0")
ser.close()
ser.open()
ser.baudrate = 9600

##def distance_check():
##    GPIO.output(TRIG,False)
##    GPIO.output(TRIG,True)
##    GPIO.output(TRIG,False)
##    pulse_start = 0
##    pulse_end = 0
##    while GPIO.input(ECHO)==0:
##        pulse_start = time.time()
##    while GPIO.input(ECHO)==1:
##        pulse_end = time.time()
##        
##    pulse_duration = pulse_end - pulse_start
##    distance = pulse_duration * 17150
##    distance = round(distance,2)
##    print("check")
##    GPIO.cleanup()

def main(manual_input):
    manual_input = curses.initscr()
    curses.noecho()
    curses.cbreak()
    manual_input.keypad(True)
    while True:
        if manual_input.getch() == ord('w'):
            ser.write(struct.pack('B',int(255)))
        elif manual_input.getch() == ord('s'):
            ser.write(struct.pack('B',int(254)))
        elif manual_input.getch() == ord('q'):
            ser.write(struct.pack('B',int(240)))
            break
        else:
            ser.write(struct.pack('B',int(0)))

try:
    while True:  
        user_input = input("Please Enter Desired Mode (Manual / Input / Automatic):\n")
        user_input = user_input.lower()
        if user_input=="input":
            #may need to change range of allowed inputs
            while True:
                user_speed = input("Please Enter Desired Speed(0-220)(cm/s) (Type 'Mode' to change modes):\n");
                if user_speed.lower()=="mode":
                    break
                elif int(user_speed)>6 and int(user_speed)<200:
                    ser.write(struct.pack('B',int(user_speed)))
                else:
                    print("Invalid Input. Please Try Again.\n")
                
        elif user_input=="manual":
            print("You are in the manual control:\n")
            print("Forward-'w' ; Reverse-'s' ; Press 'q' to change mode")
            wrapper(main)
        elif user_input=="automatic":
            print("Automatic")
        else:
            print("Invalid Input. Please Try Again.\n")

finally:
    curses.nocbreak()
    manual_input.keypad(False)
    curses.echo()
    curses.endwin()
    GPIO.cleanup()
    

    