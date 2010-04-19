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
	hasReceivedTerminationRequest = false;
	sizeSetForProccessorID = -1;

	processorToAskForWork = (rank + 1) % numProcesses;
	processorToSendTokenTo = (rank+1)%numProcesses;
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
	for(int i = 1; i < this->numProcesses; i++){
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

void Communicator::sendWorkRequest(){
	int x = 0;
	MPI_Send (&x, 1, MPI_INT, this->processorToAskForWork, Communicator::REQUEST_WORK, MPI_COMM_WORLD);

	processorToAskForWork = (processorToAskForWork+1) % numProcesses;
	if(processorToAskForWork == rank){
		processorToAskForWork = (processorToAskForWork+1) % numProcesses;
	}
}

int Communicator::getMessageType(){
	return status.MPI_TAG;
}
void Communicator::sendNoWork(int processorID){
	int x = 0;
	MPI_Send (&x, 1, MPI_INT, processorID, Communicator::NO_WORK, MPI_COMM_WORLD);
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
		delete state;
	}

	tag = SENDING_WORK;
	MPI_Send (buffer, length, MPI_PACKED, proccessorID, tag, MPI_COMM_WORLD);

	delete buffer;
}

void Communicator::receiveStack(stack<State*> *stack, int proccessorID){
	//nemuze nastat situace, ze by prisel stack od jineho procesoru, nez poslal stackSize, protoze vysleme max 1 zadost. -> proto zakomentovany if
	//if(proccessorID == sizeSetForProccessorID)
	{
		char * buffer = new char[stackSize];
		MPI_Recv(buffer, stackSize, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		State *state;
		int pointer = 0;
		while(pointer<stackSize){
			state = State::deserialize(buffer, pointer, numOfVertices);
			stack->push(State::deserialize(buffer, pointer, numOfVertices));
			pointer += stateSize;
		}
		//sizeSetForProccessorID = -1;
	}
}
void Communicator::receiveStackSize(){
	MPI_Recv(&stackSize, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	//id procesoru si nemusime ukladat, vzdy se jedna jen o 1 zadost o praci
	//sizeSetForProccessorID = status.MPI_SOURCE;
}

State* Communicator::receiveBestSolution(){
	State* receivedState = NULL;
	/*MPI_Status status;
	int numVertices;
	MPI_Recv(&numVertices, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	int length = numVertices*numVertices + sizeof(int)*2;
	char * buffer = new char[length];
	MPI_Recv(buffer, length, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	receivedState = State::deserialize(buffer, 0, numVertices);

	delete buffer;
*/
	return receivedState;
}

void Communicator::sendTokenWhite(){
	int x = 0;
	MPI_Send (&x, 1, MPI_INT, processorToSendTokenTo, Communicator::TOKEN_WHITE, MPI_COMM_WORLD);
}
void Communicator::sendTokenBlack(){
	int x = 0;
	MPI_Send (&x, 1, MPI_INT, processorToSendTokenTo, Communicator::TOKEN_BLACK, MPI_COMM_WORLD);
}
void Communicator::receiveTokenWhite(){
	int x;
	MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}
void Communicator::receiveTokenBlack(){
	int x;
	MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}


void Communicator::receiveWorkRequest(){
	int x;
	MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}
void Communicator::receiveNoWork(){
	int x;
	MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}
void Communicator::receiveTerminate(){
	int x;
	MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}

Communicator::~Communicator() {
}
