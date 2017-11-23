TARGET = test.bin
CC = g++ 
OPT = -Wall -g -std=c++11
# OPT = -Wall -g -std=gnu++0x
INCLUDE = -I. -Iinc -I../common/jsoncpp
CC_FLAGS = $(OPT) $(INCLUDE)

LD_FLAGS = -lpthread -lboost_system -lboost_filesystem
LIB_PATH :=

CPP_FILES := $(wildcard src/*.cpp) main.cpp 
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))



$(TARGET): $(OBJ_FILES)
	$(CC) $(LIB_PATH) -o $@ $^ $(LD_FLAGS)

obj/main.o: main.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<
	
obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<
	

	
all:	$(TARGET)


	

clean:
	@echo Cleaning up...
	@rm obj/*.o -f
	@rm -f $(TARGET)
	@echo Done.

