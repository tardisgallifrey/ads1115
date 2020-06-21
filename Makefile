#Always remember to use <tab> character on command lines

#Make ADS read executable

#Compiler
CC = gcc

#Compiler flags
CFLAGS = -Wall

#Target Executable
TARGET = ads_read

default: $(TARGET)

$(TARGET): adsloop.o ads_read.o
	$(CC) $(CFLAGS) -o ads_read adsloop.o ads_read.o

adsloop.o: adsloop.c
	$(CC) $(CFLAGS) -c adsloop.c

ads_read.o: ads_read.c
	$(CC) $(CFLAGS) -c ads_read.c

clean:
	$(RM) $(TARGET) *.o
