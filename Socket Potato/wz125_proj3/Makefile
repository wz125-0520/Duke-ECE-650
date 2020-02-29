TARGETS = player ringmaster

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

player:player.cpp potato.cpp server.cpp
	g++ -g -o player player.cpp potato.cpp server.cpp

ringmaster:ringmaster.cpp potato.cpp server.cpp
	g++ -g -o ringmaster ringmaster.cpp server.cpp potato.cpp
