CC=g++
CCFLAGS=-g -std=c++11 -Wall -Wextra -pedantic -pthread

BOT_TOKEN = NzY4ODExNTcxMzQyMjEzMTIw.X5F5kg.u3Sw7HN2aDhTnofCcAfOY9cDDdg

MKDIR_P = mkdir -p

BINARIES=obj/isabot.o obj/argHandler.o obj/errorHandler.o obj/SSLClient.o obj/parser.o obj/stack.o

all: obj isabot

obj:
	${MKDIR_P} obj

isabot: $(BINARIES)
	$(CC) $(CCFLAGS) -o isabot $(BINARIES) -lssl -lcrypto

###############################################

obj/isabot.o: src/isabot.cpp src/isabot.h
	$(CC) $(CCFLAGS) -c src/isabot.cpp -o obj/isabot.o

obj/argHandler.o: src/argHandler.cpp src/argHandler.h
	$(CC) $(CCFLAGS) -c src/argHandler.cpp -o obj/argHandler.o

obj/errorHandler.o: src/errorHandler.cpp src/errorHandler.h
	$(CC) $(CCFLAGS) -c src/errorHandler.cpp -o obj/errorHandler.o

obj/SSLClient.o: src/SSLClient.cpp src/SSLClient.h
	$(CC) $(CCFLAGS) -c src/SSLClient.cpp -o obj/SSLClient.o

obj/parser.o: src/parser.cpp src/parser.h
	$(CC) $(CCFLAGS) -c src/parser.cpp -o obj/parser.o

obj/stack.o: src/stack.cpp src/stack.h
	$(CC) $(CCFLAGS) -c src/stack.cpp -o obj/stack.o

################################################

run: obj isabot
	./isabot -t ${BOT_TOKEN}

################################################

run_verbose: obj isabot
	./isabot --verbose -t ${BOT_TOKEN}

################################################

clean:
	rm -f isabot; rm -rf obj
