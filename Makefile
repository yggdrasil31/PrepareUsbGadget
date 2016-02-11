INC_DIR= ./include

EXEC = PrepareUsbGadget
SRC= main.c fat12.c usb.c
HEADER= $(wildcard *.h)
OBJ= $(SRC:.c=.o)

CC?=gcc

EXTRACFLAGS := -Wall -pthread -I$(INC_DIR) 
EXTRALDFLAGS := -Wall -pthread -I$(INC_DIR)

.PHONY: all

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(EXTRALDFLAGS)

%.o: %.c
	@echo "----------------*.o files compilation--------------"
	$(CC) -o $@ -c $< $(EXTRACFLAGS) $(EXTRA_LDFLAGS)	
	@echo "----- End compilation ---"

.PHONY: clean

clean:
	rm -rf *.o
	rm -f $(EXEC)
