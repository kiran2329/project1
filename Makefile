CC = gcc
COMPILER_FLAGS = -g -std=gnu99
LINKER_FLAGS = -g -lm
BINARY = logParse
OBJS = logParse.o
HEADERS = 

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $(BINARY) $(OBJS) $(LINKER_FLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(COMPILER_FLAGS) -c $<

clean:
	/bin/rm $(OBJS) $(BINARY)
