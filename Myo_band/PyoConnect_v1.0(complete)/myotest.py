#!/usr/bin/python
''' DON'T TOUCH THESE FIRST LINES! '''
''' ============================== '''
from PyoConnect import *
myo = Myo(sys.argv[1] if len(sys.argv) >= 2 else None) 
''' ============================== '''
import RPi.GPIO as GPIO
import time
''' OK, edit below to make your own fancy script ^.^ '''

# Edit here:


def onPoseEdge(pose, edge):
	# active at all times
	if (pose == "waveOut"):
		if (edge == "on"): 
			print("fist ON")
                        GPIO.setmode(GPIO.BCM)
                        GPIO.setup(23,GPIO.OUT)
                        try:
                                print("set GPIO HIGH")
                                GPIO.output(23,True)
                                time.sleep(1)
                       
                        except:
                                print("error")
                        finally:
                                print("clean up")
                                GPIO.cleanup(23)
                
		if (edge == "off"): 
			print("fist ON")
                        GPIO.setmode(GPIO.BCM)
                        GPIO.setup(23,GPIO.OUT)
                        try:
                                print("set GPIO LOW")
                                GPIO.output(23,False)
                                time.sleep(1)
                        except:
                                print("error")
                        finally:
                                print("clean up")
                                GPIO.cleanup(23)		
	'''if (pose == "waveIn"):
		if (edge == "on"): 
			myo.keyboard("left_arrow","down","")
			myo.unlock("hold")
		if (edge == "off"): 
			myo.keyboard("left_arrow","up","")
			myo.unlock("timed")			
	if (pose == "fist") and (edge == "on"): 
		myo.keyboard("space","press","")
'''
# Stop editting


# Comment out below the events you are not using
#myo.onLock = onLock
#myo.onUnlock = onUnlock
#myo.onPoseEdge = onPoseEdge
#myo.onPeriodic = onPeriodic
#myo.onWear = onWear
#myo.onUnwear = onUnwear
#myo.onEMG = onEMG
#myo.onBoxChange = onBoxChange

''' DON'T TOUCH BELOW THIS LINE! '''
''' ============================ '''
myo.connect()
while True:
	myo.run()
	myo.tick()
