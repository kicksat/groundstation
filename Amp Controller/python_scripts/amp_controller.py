import serial
import time

arduino = serial.Serial('/dev/ttyUSB0', 9600)

def tx():
	arduino.write('0')
	print arduino.readline()

def rx():
	arduino.write('1')
	print arduino.readline()

def __main__():
	# Wait for connection to arduino
	p = arduino.readline()
	while not p:
		print("waiting for arduino to connect...")
		p = arduino.readline()
	print p

	# Prompt user to select amp mode
	while True:
		print("Select mode:  0 (TX), 1 (RX)")
		data = raw_input("0/1: ")
		if data == "0":
			tx()
		elif data == "1":
			rx()
		elif data == "exit":
			return
		else:
			print("Invalid command")

__main__()
