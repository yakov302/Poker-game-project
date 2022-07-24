
SRC_DIR = ../src
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = g++
CC = $(CXX)
WARNINGS = -pedantic
WARNINGS += -Wall #-Werror
DEBUG = -g3
CXX_STD = c++17
LDLIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)
CPPFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)


TARGET = main
OBJS = ./main.o ./table.o ./wallet.o ./hand.o ./player.o ./text.o ./players_container.o 
OBJS += ./client_manager.o ./self.o ./router.o ./tcp_client.o ./action_in.o ./action_out.o 
OBJS += ./text_box.o
OBJS += $(PROTOCOL_DIR)/protocol.o

$(TARGET):$(OBJS)

check: $(TARGET)
	./$(TARGET) -v  

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean
