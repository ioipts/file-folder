EXECUTABLE=file
CC=g++
OBJ_PATH=.
ifndef OS
OS=$(shell uname)
endif

ifeq ($(OS),Darwin)
LOCAL_PATH=..
GAME_PATH=.
SEP=/
CN=rm -rf 
CFLAGS=-c -Wall -F/Library/Frameworks -std=c++11
LDFLAGS=-F/Library/Frameworks -lm 
else ifeq ($(OS),Linux)
LOCAL_PATH=..
GAME_PATH=.
SEP=/
CN=rm -rf
CFLAGS=-c -Wall -O3
LDFLAGS=-lm 
else ifeq ($(OS),Windows_NT)
LOCAL_PATH=..
GAME_PATH=.
SEP=\\
CN=del /q
CFLAGS=-c -Wall -O3
LDFLAGS=-lm 
endif

INCFLAGS=-I$(LOCAL_PATH) -I$(GAME_PATH) 

SOURCES=$(GAME_PATH)$(SEP)sample.cpp 
SOURCES+=$(LOCAL_PATH)$(SEP)axisfile.cpp 

OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS+=$(CSOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(addprefix $(OBJ_PATH)$(SEP),$(notdir $(OBJECTS))) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(INCFLAGS) $(CFLAGS) $< -o $(addprefix $(OBJ_PATH)$(SEP),$(notdir $@))
.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) $< -o $(addprefix $(OBJ_PATH)$(SEP),$(notdir $@))
	
clean:
	$(CN) $(OBJ_PATH)$(SEP)*.o
