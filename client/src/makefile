
SRC_DIR = ../src
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = g++
CC = $(CXX)
WARNINGS = -pedantic
WARNINGS += -Wall -Werror
DEBUG = -g3
CXX_STD = c++2a
LDLIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)
CPPFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)


TARGET = main
OBJS = ./main.o ./table.o ./wallet.o ./hand.o ./player.o ./text.o ./players_container.o 
OBJS += ./client_manager.o ./self.o ./router.o ./tcp_client.o ./action_in.o ./action_out.o 
OBJS += ./text_box.o ./client_socket.o ./sound.o ./cards_container.o ./chips_container.o
OBJS += ./cli_cmd.o ./wait.o
OBJS += $(PROTOCOL_DIR)/protocol.o

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
