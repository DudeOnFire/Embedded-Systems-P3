CC = g++
CC_FLAGS = -Wall $(EXTRA)
LD_FLAGS = $(EXTRA)
LIBS = -lpthread -ltinyb

SRC = $(wildcard *.cpp)
OBJ = $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))

TARGET = termin3

all: obj $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LD_FLAGS) -o $@ $(OBJ) $(LIBS)
obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

obj:
	mkdir obj

clean:
	rm -rf obj
	rm -rf $(TARGET)
