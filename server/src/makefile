
SRC_DIR = ../src
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = g++
CC = $(CXX)
WARNINGS = -pedantic
WARNINGS += -Wall -Werror
DEBUG = -g3
CXX_STD = c++17
LDLIBS   = -lpthread

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)
CPPFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)


TARGET = main
OBJS = ./main.o ./tcp_server.o  ./action_in.o ./rauter.o ./server_manager.o  ./sha256.o 
OBJS += ./subscribs.o ./action_out.o ./deck.o ./players_container.o ./server_socket.o
OBJS += $(PROTOCOL_DIR)/protocol.o

$(TARGET):$(OBJS)

check: $(TARGET)
	./$(TARGET) -v  

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean
