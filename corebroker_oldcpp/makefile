TARGET = test.bin
CROSS := arm-none-linux-gnueabi-
CROSS_INCLUDE := -I system/
CC = g++ 
OPT = -Wall -g 
INCLUDE = -I. -Iinc -I../common/jsoncpp -I../common/
CC_FLAGS = $(OPT) $(INCLUDE)

LD_FLAGS = -lpthread  -lfile
LIB_PATH := -L ../common/file

CPP_FILES := $(wildcard src/*.cpp) main.cpp 
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))


$(TARGET): $(OBJ_FILES)
	$(CC) $(LIB_PATH) -o $@ $^ $(LD_FLAGS)


obj/tfile.o: ../common/file/tfile.cpp
	@mkdir -p obj
	$(CC) $(CC_FLAGS) -c -o $@ $<
	
obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CC_FLAGS) -c -o $@ $<

all:	$(TARGET)
	

clean:
	@echo Cleaning up...
	@find obj -name '*.o' -exec rm {} \;
	@rm $(TARGET)
	@echo Done.

