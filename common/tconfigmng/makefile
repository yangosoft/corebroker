TARGET = libconfigmng.so
VERSION = 1
SONAME = $(TARGET).$(VERSION)
CROSS := arm-none-linux-gnueabi-
CROSS_INCLUDE := -I system/
CC = g++ 
LD = g++
LD_OPTS = -Wl,-soname=$(SONAME)
OPT = -Wall -g -fPIC -shared
INCLUDE = -I. -Iinc -I../jsoncpp -I../ -I../tmessage/inc
CC_FLAGS = $(OPT) $(INCLUDE) -DVERSION_STRING=$(VERSION_STRING) -DMODULE_NAME=$(MODULE_NAME)

LD_FLAGS =  -lnodemessage 
LIB_PATH := -L../tmessage

CPP_FILES := $(wildcard src/*.cpp) 
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

MODULE_NAME := \"CONFIGMANAGER\"
VERSION_STRING := \"0.1\"



$(TARGET): $(OBJ_FILES)
	$(LD) $(LIB_PATH) $(OPT)  $(LD_OPTS)  -o $(TARGET) $^ $(LD_FLAGS)
	@ln -s $(TARGET) $(SONAME)


obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CC_FLAGS) -c -o $@ $<

all:	$(TARGET)
	

clean:
	@echo Cleaning up...
	@find obj -name '*.o' -exec rm {} \;
	@rm $(TARGET)
	@rm $(SONAME)
	@echo Done.

