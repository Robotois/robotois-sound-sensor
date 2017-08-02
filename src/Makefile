CC=g++

COMPILE_FLAGS = -g -std=c++11 -Wall -c
LINKER_FLAGS = -g
LIBS = -l bcm2835 -l rt

OBJECTS = \
	test.o \
	ADS1015.o \
	SoundSensor.o \

#OBJECTS = $(subst .cpp,.o,$(SOURCES))

all: ads.o sound.o main.o
	$(CC) $(LINKER_FLAGS) -o runner $(OBJECTS) $(LIBS)

main.o: test.cpp
	$(CC) $(COMPILE_FLAGS) test.cpp
ads.o: libraries/robotois-ADS1015/ADS1015.cpp libraries/robotois-ADS1015/ADS1015.h
	$(CC) $(COMPILE_FLAGS) libraries/robotois-ADS1015/ADS1015.cpp
sound.o: SoundSensor.cpp SoundSensor.h
	$(CC) $(COMPILE_FLAGS) SoundSensor.cpp

clean:
	$(RM) $(OBJECTS) runner
