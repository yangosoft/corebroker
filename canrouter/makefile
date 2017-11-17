MODULE_NAME := \"CANROUTER\"
TARGET = canrouter.bin
CROSS := arm-none-linux-gnueabi-
CROSS_INCLUDE := -I system/
CC = g++ 
OPT = -Wall -g 
INCLUDE = -I. -Iinc -I../common/jsoncpp -I../common/ -I../common/tcan/inc -I../common/tnodenet/inc/ -I../common/tmessage/inc/ -I../external -I../common/tconfigmng/inc
CC_FLAGS = $(OPT) $(INCLUDE) -DCPPLOG_FILTER_LEVEL=$(LOG_LEVEL) -DVERSION_STRING=$(VERSION_STRING) -DMODULE_NAME=$(MODULE_NAME)

LD_FLAGS = -lpthread  -lfile -lcansocket -lnodesocket -lnodemessage -lconfigmng
LIB_PATH := -L ../common/file -L../common/tcan -L../common/tnodenet -L../common/tmessage -L../common/tconfigmng

CPP_FILES := $(wildcard src/*.cpp) main.cpp 
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

LOG_LEVEL :=LL_TRACE

#LOG_LEVEL :=LL_TRACE    
#LOG_LEVEL :=LL_DEBUG    
#LOG_LEVEL :=LL_INFO     
#LOG_LEVEL :=LL_WARN     
#LOG_LEVEL :=LL_ERROR    
#LOG_LEVEL :=LL_FATAL    


VERSION_STRING := \"0.1\"
	
	


$(TARGET): $(OBJ_FILES)
	$(CC) $(LIB_PATH) -o $@ $^ $(LD_FLAGS)



obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CC_FLAGS) -c -o $@ $<

all:	$(TARGET)
	

clean:
	@echo Cleaning up...
	@find obj -name '*.o' -exec rm {} \;
	@rm $(TARGET)
	@echo Done.

