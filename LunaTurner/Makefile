CC	= /home/john/raspistuff/toolchain/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++

pi:	CC = g++-4.7
CFLAGS	= -Wall -O -std=c++11 $(DEFINES)
LDFLAGS = -lpthread
DEFINES = -D_GLIBCXX_USE_NANOSLEEP=1
ALLOBJS = main.o AbstractCyclicThread.o GPIOPin.o GPIO.o StepperMotor.o Angles.o Server.o
TARGET = LunaTurner

doc:
	doxygen Doxyfile
	
all: $(TARGET)

pi: $(TARGET)
	
$(TARGET) : $(ALLOBJS)
	$(CC) $(ALLOBJS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

main.o : src/main.cpp
	$(CC) -c src/main.cpp $(CFLAGS)

AbstractCyclicThread.o : src/AbstractCyclicThread.cpp
	$(CC) -c src/AbstractCyclicThread.cpp $(CFLAGS)

GPIOPin.o : src/GPIOPin.cpp
	$(CC) -c src/GPIOPin.cpp $(CFLAGS)
	
GPIO.o : src/GPIO.cpp
	$(CC) -c src/GPIO.cpp $(CFLAGS)

StepperMotor.o : src/StepperMotor.cpp
	$(CC) -c src/StepperMotor.cpp $(CFLAGS)
	
Angles.o : src/Angles.cpp
	$(CC) -c src/Angles.cpp $(CFLAGS)
	
Server.o : src/Server.cpp
	$(CC) -c src/Server.cpp $(CFLAGS)