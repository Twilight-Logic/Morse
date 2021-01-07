# Makefile for 'morse'
# version 0.01.01
# @ John Chajecki
# 07/01/2021


# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = morse


# build an executable named morse from morse.cpp
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean: 
	  $(RM) $(TARGET)

