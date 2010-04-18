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
	isWaiting = false;
	hasSentToken = false;
	sizeSetForProccessorID = -1;
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

void Communicator::sendStack(stack<State*> *stack, int proccessorID){

	int numStatesToPop = stack->size()/2;
	int length = sizeof(State) * stateSize;
	int tag = SENDING_WORK_SIZE;

	MPI_Send (&length, 1, MPI_INT, proccessorID, tag, MPI_COMM_WORLD);

	State* state;
	char * buffer = new char[length];

	for(int i=0; i<numStatesToPop; i++){
		state = stack->top();
		stack->pop();
		state->serialize(buffer, i*stateSize);
	}

	tag = SENDING_WORK;
	MPI_Send (buffer, length, MPI_PACKED, proccessorID, tag, MPI_COMM_WORLD);

	delete buffer;
}

void Communicator::receiveStack(){

}
void Communicator::receiveStackSize(){
	MPI_Recv(&stackSize, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	sizeSetForProccessorID = status.MPI_SOURCE;
}

Communicator::~Communicator() {
}
