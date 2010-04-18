/*
 * Communicator.cpp
 *
 *  Created on: 12.4.2010
 *      Author: dawe
 */

#include "Communicator.h"

Communicator::Communicator(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	this->hasSentToken = false;
	this->isWaiting = false;
}

int Communicator::getRank(){
	return rank;
}

int Communicator::getNumProcesses(){
	return numProcesses;
}

void Communicator::finalize(){
	MPI_Finalize();
}

void Communicator::sendTerminateToAll(){
	int tag = Communicator::TERMINATE;
	for(int i = 1; i < this->numProcesses){
		MPI_Send (&i, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
	}
}

void Communicator::sendState(State* stateToSend, int proccessorID){
	int length = stateToSend->numberOfVertices*stateToSend->numberOfVertices + sizeof(int)*2;
	char * buffer = new char[length];
	stateToSend->serialize(buffer, 0);

	int tag = 1;
	MPI_Send (&(stateToSend->numberOfVertices), 1, MPI_INT, proccessorID, tag, MPI_COMM_WORLD);
	MPI_Send (buffer, length, MPI_PACKED, proccessorID, tag, MPI_COMM_WORLD);

	delete buffer;
}

State* Communicator::receiveState(){
	State* receivedState;
	MPI_Status status;
	int numVertices;
	MPI_Recv(&numVertices, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	int length = numVertices*numVertices + sizeof(int)*2;
	char * buffer = new char[length];
	MPI_Recv(buffer, length, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	receivedState = State::deserialize(buffer, 0, numVertices);

	delete buffer;

	return receivedState;
}

int Communicator::hasReceivedMessages() {
	int flag;
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	return flag;
}
void Communicator::sendWhiteToken(int toProccessor){

}
void Communicator::sendBlackToken(int toProccessor){

}

int Communicator::getMessageType(){
	return status.MPI_TAG;
}

void Communicator::sendStack(){
	//todo
}

void Communicator::receiveStack(){
	//todo
}

Communicator::~Communicator() {

}
