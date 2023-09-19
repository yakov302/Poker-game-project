
SRC_DIR = ../src
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = g++
CC = $(CXX)
WARNINGS = -pedantic
WARNINGS += -Wall -Werror
DEBUG = -g3
CXX_STD = c++2a
LDLIBS   = -lpthread

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)
CPPFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)


TARGET = main
OBJS = ./main.o  ./cli_cmd.o ./tcp_server.o  ./action_in.o ./rauter.o ./server_manager.o  ./sha256.o 
OBJS += ./subscribs.o ./action_out.o ./deck.o ./players_container.o ./server_socket.o
OBJS +=  ./game.o ./card_round.o ./bet_round.o ./table.o ./wait.o ./winner.o ./player.o 
OBJS += ./table_manager.o ./tables_container.o ./msg_memory.o $(PROTOCOL_DIR)/protocol.o

$(TARGET):$(OBJS)

check: $(TARGET)
	./$(TARGET) -v  

clean:
	@$(RM) $(TARGET) $(OBJS)

debug:
	gdb ./$(TARGET)  

debug1:
	valgrind --log-file=V1.log ./$(TARGET)  

debug2:
	valgrind --log-file=V2.log --leak-check=full --track-origins=yes ./$(TARGET)  

debug3:
	valgrind --log-file=V3.log --leak-check=full --track-origins=yes -s ./$(TARGET) 
	
.PHONY: check clean
