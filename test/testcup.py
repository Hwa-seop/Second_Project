import RPi.GPIO as gpio
import time

TRIGER = 24
ECHO = 23

gpio.setmode(gpio.BCM)
gpio.setup(TRIGER, gpio.OUT)
gpio.setup(ECHO, gpio.IN)

startTime = time.time()

try:
    while True:
        gpio.output(TRIGER, gpio.LOW)
        time.sleep(0.1)
        gpio.output(TRIGER, gpio.HIGH)
        time.sleep(3)
        gpio.output(TRIGER, gpio.LOW)
        
        while gpio.input(ECHO) == gpio.LOW:
            startTime = time.time()
            
        while gpio.input(ECHO) == gpio.HIGH:
            endTime = time.time()
            
        period = endTime - startTime
        dist1 = round(period * 1000000 /58, 2)
        dist2 = round(period *17241, 2)
        
        print("Dist1", dist1, "cm", ", Dist2", dist2, "cm")
        
        if dist1 < 10 and dist2 < 10:
            print("detect")
            
except:
    gpio.cleanup