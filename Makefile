# all after symbol '#' is comment

# === which communication library to use ===
CC	=	mpiCC      # for ethernet and infiniband networks

CFLAGS	=
LIBS	=	-lm

default:   Bipartite

Bipartite : Bipartite.cpp State.o GraphReader.o Communicator.o
	$(CC) $(CFLAGS) -o Bipartite $(LIBS) 
 
GraphReader : GraphReader.cpp
	$(CC) $(CFLAGS) -o GraphReader.o GraphReader.cpp $(LIBS) 

Communicator: Communicator.cpp
	$(CC) $(CFLAGS) -o Communicator.o Communicator.cpp $(LIBS) 

State: State.cpp   
	$(CC) $(CFLAGS) -o State.o State.cpp $(LIBS) 

