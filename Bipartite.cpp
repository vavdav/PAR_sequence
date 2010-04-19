//============================================================================
// Name        : Bipartite.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stack>
#include "State.h"
#include <vector>
#include "GraphReader.h"
#include <fstream>
#include "Communicator.h"
#include <string>

using namespace std;

int MIN_STACK_SIZE_TO_SEND_WORK = 20;

Communicator *communicator;
stack<State*> *state_stack;
State *bestSolution;
int bestSolutionNumberOfEdges;

void processMessages(){
	switch(communicator->getMessageType()){
		case Communicator::REQUEST_WORK:
			communicator->receiveWorkRequest();
			if(state_stack->size() > 4){
				communicator->sendStack(state_stack, communicator->status.MPI_SOURCE);
			} else {
				communicator->sendNoWork(communicator->status.MPI_SOURCE);
			}
			break;
		case Communicator::NO_WORK:
			communicator->receiveNoWork();
			communicator->hasRequestedWork = false;
			break;
		case Communicator::SENDING_WORK_SIZE:
			communicator->receiveStackSize();
			break;
		case Communicator::SENDING_WORK:
			cout << "p" << communicator->rank << " stack:" << state_stack->size();
			communicator->receiveStack(state_stack, communicator->status.MPI_SOURCE);
			cout << "p" << communicator->rank << " rstack:" << state_stack->size();
			communicator->hasRequestedWork = false;
			break;
		case Communicator::SOLUTION:
			/*State* bestSolutionReceived = communicator->receiveBestSolution();
			if(bestSolutionReceived->getNumberOfEdges() > bestSolution->getNumberOfEdges()){
				bestSolution = bestSolutionReceived;
				bestSolutionNumberOfEdges = bestSolution->getNumberOfEdges();
			}*/
			break;
		case Communicator::TERMINATE:
			communicator->receiveTerminate();
			communicator->hasReceivedTerminationRequest = true;
			break;
		case Communicator::TOKEN_BLACK:
			communicator->receiveTokenBlack();
			if(communicator->rank == 0){
				communicator->hasSentToken = false;
				//dalsi bily token se posle za dalsich X cyklu
			} else {
				communicator->sendTokenBlack();
			}
			break;
		case Communicator::TOKEN_WHITE:
			communicator->receiveTokenWhite();
			if(communicator->rank == 0){
				communicator->hasSentToken = true;
				communicator->sendTerminateToAll();
				communicator->hasReceivedTerminationRequest = true;
			} else {
				if(communicator->isWaiting){
					communicator->sendTokenWhite();
				} else {
					communicator->sendTokenBlack();
				}
			}
		break;
	}
}
int states_count_push;
int states_count_pop;
int state1NumberOfEdges;
State *state_top;
State **successors;

int currentSolutionNumberOfEdges;
int bipartityTest;

void expandState(){
	state_top = state_stack->top();
	state_stack->pop();
	states_count_pop++;
	currentSolutionNumberOfEdges = state_top->getNumberOfEdges();
	bipartityTest = state_top->isBipartite();
	if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth && bipartityTest>-1){
		successors = state_top->getSuccessors();

		//push state without edge
		state_stack->push(successors[0]);
		states_count_push++;
		//push state with edge
		state_stack->push(successors[1]);
		states_count_push++;
		delete successors;
	}

	if(bipartityTest == 1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
		bestSolutionNumberOfEdges = currentSolutionNumberOfEdges;
		bestSolution = state_top;
	} else {
		delete state_top;
	}
}

void compute(){
	communicator->synchronizeBarrier();
	cout << "p" << communicator->rank << " compute" << endl;
	states_count_push = 1;
	states_count_pop = 0;
	state1NumberOfEdges = state_stack->top()->getNumberOfEdges();

	int cycleCounter = 0;
	int cycleCounter1 = 100;

	communicator->stateSize = state_stack->top()->stateSize;
	communicator->numOfVertices = state_stack->top()->numberOfVertices;

	communicator->hasReceivedTerminationRequest = false;
	communicator->hasRequestedWork = false;

	while(!communicator->hasReceivedTerminationRequest){
		communicator->isWaiting = false;
		while(!state_stack->empty()){
			expandState();

			cycleCounter++;
			if (cycleCounter == 100){
				if(communicator->hasReceivedMessages()){
					processMessages();
				}
				cycleCounter = 0;
			}
		}
		communicator->isWaiting = true;

		//p0 posle White Token
		if(!communicator->hasSentToken && communicator->rank == 0){
			if(communicator->numProcesses > 1){
				communicator->hasSentToken = true;
				communicator->sendTokenWhite();
			}
		}

		if (cycleCounter1 == 200){
			if(!communicator->hasRequestedWork){
				communicator->hasRequestedWork = true;
				communicator->sendWorkRequest();
			}
			cycleCounter1 = 0;
		}
		cycleCounter1++;

		while(communicator->hasReceivedMessages()){
			processMessages();
		}

	}

	cout << "p" << communicator->rank << "'s best solution" << endl;
	bestSolution->print();

	if(communicator->rank == 0){
		/*State* globalBestSolution = NULL;
		globalBestSolution = selectBestSolution();
		cout << "global best solution:" << endl;
		globalBestSolution->print();
		delete globalBestSolution;
		*/
	} else {
		//sendBestSolution();
	}

	delete bestSolution;

}



/*if(state_stack->top()->isBipartite() == 1){
		bestSolution = state_stack->top();
		state_stack->pop();
	} else {
	if(!state_stack->empty()){
		communicator->isWaiting = false;
		communicator->hasWorkToDo = true;
	}
	while(){
	while(!state_stack->empty()){

		state_top = state_stack->top();
		state_stack->pop();
		states_count_pop++;
		//cout << "vertices:" << state_top->numberOfVertices << " edge index:" << state_top->edgeIndex << " depth:" << state_top->depth << endl;
		currentSolutionNumberOfEdges = state_top->getNumberOfEdges();
		bipartityTest = state_top->isBipartite();
		if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth && bipartityTest>-1){
			successors = state_top->getSuccessors();

			//push state without edge
			state_stack->push(successors[0]);
			states_count_push++;
			//push state with edge
			state_stack->push(successors[1]);
			states_count_push++;
			delete successors;
		}

		if(bipartityTest == 1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
			bestSolutionNumberOfEdges = currentSolutionNumberOfEdges;
			bestSolution = state_top;
		} else {
			delete state_top;
		}

		cycleCounter++;
		if ((cycleCounter % 100)==0){
			if(communicator->hasReceivedMessages()){
				processMessages();
			}
		}
	}

		cout << "p" << communicator->rank << " stack.empty()" << endl;

		// tak zasobnik je prazdny a co ted
		communicator->isWaiting = true;

		if(!communicator->hasSentToken && communicator->rank == 0){
			if(communicator->numProcesses > 1){
				cout << "p" << communicator->rank << " sending White Token" << endl;
				communicator->hasSentToken = true;
				communicator->sendTokenWhite();
			}
		}





		cout << "end while" << endl;
	//}
	cout << "states_pop:" << states_count_pop <<endl;

	return bestSolution;*/

State* distributeStates(State * stateStart){
	stack<State*> distribute_stack;
	distribute_stack.push(stateStart);
	State **successors;
	State * state_top;
	int state1NumberOfEdges = stateStart->getNumberOfEdges();
	int currentSolutionNumberOfEdges;
	int bipartityTest;

	while(distribute_stack.size() != communicator->numProcesses){
		state_top = distribute_stack.top();
		distribute_stack.pop();
		currentSolutionNumberOfEdges = state_top->getNumberOfEdges();
		bipartityTest = state_top->isBipartite();

		if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth && bipartityTest>-1){
			successors = state_top->getSuccessors();
			//push state without edge
			distribute_stack.push(successors[0]);
			//push state with edge
			distribute_stack.push(successors[1]);
			delete successors;
		}
		delete state_top;
	}
	cout << "distribute_stack.size " << distribute_stack.size() << endl;

	for(int i = 1; i<communicator->numProcesses; i++){
		state_top = distribute_stack.top();
		distribute_stack.pop();
		communicator->sendState(state_top, i);
		delete state_top;
	}
	state_top = distribute_stack.top();
	distribute_stack.pop();
	cout << "firstState p0" << endl;
	//state_top->print();

	return state_top;
}

void mainProccessor(State * state1, char *fileName){
	GraphReader reader;
	State *stateStart = reader.getFirstStateFromFile(fileName);

	state1 = distributeStates(stateStart);
	cout << "p" << communicator->rank << " got first State" << endl;

	state_stack->push(state1);
	compute();
}

void otherProccessor(State * state1){
	state1 = communicator->receiveState();
	cout << "p" << communicator->rank << " got first State" << endl;

	state_stack->push(state1);
	compute();
}



int main (int argc, char *argv[] )
{

	if ( argc != 2 ) {
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		return -1;
	}

	State *state1 = NULL;
	state_stack = new stack<State*>();
	communicator = new Communicator(argc, argv);

	bestSolutionNumberOfEdges = 0;
	if(communicator->rank == 0){
		mainProccessor(state1, argv[1]);
	} else {
		otherProccessor(state1);
	}

	if(state1 != NULL){
		delete state1;
	}

	cout << "p" << communicator->rank << " ending" << endl;

	communicator->finalize();

	delete communicator;

	return 0;
}
 // --- KONEC
/*
	if(communicator.rank == 0){

		char *fileName;
		if ( argc != 2 ) {
			cout<<"usage: "<< "PAR" <<" <filename>" << endl;
			return -1;
		}
		fileName = argv[1];
		//fileName = "graph10.txt";
		GraphReader reader;
		state1 = reader.getFirstStateFromFile(fileName);

		int length = state1->numberOfVertices*state1->numberOfVertices + sizeof(int)*2;
		buffer = new char[length];
		state1->serialize(buffer, 0);

		int tag = 1;
		//char[MESSAGE_SIZE] buffer;
		for(int receiver = 1; receiver < communicator.numProcesses; receiver++){
			//state1->print();
			MPI_Send (&state1->numberOfVertices, 1, MPI_INT, receiver, tag, MPI_COMM_WORLD);
			MPI_Send (buffer, length, MPI_PACKED, receiver, tag, MPI_COMM_WORLD);
		}
	} else {
		MPI_Status status;
		int numVertices;
		MPI_Recv(&numVertices, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int length = numVertices*numVertices + sizeof(int)*2;
		buffer = new char[length];
		MPI_Recv(buffer, length, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		state1 = State::deserialize(buffer, 0, numVertices);
		state1->print();
	}
	state_stack.push(state1);

	//delete buffer;

	int states_count_push = 1;
	int states_count_pop = 0;
	int state1NumberOfEdges = state1->getNumberOfEdges();
	State *state_top;
	State **successors;

	State *bestSolution;
	int bestSolutionNumberOfEdges = 0;
	int currentSolutionNumberOfEdges;
	int bipartityTest;
	//main cycle

	if(state1->isBipartite() == 1){
		bestSolution = state1;
	} else {
		while(!state_stack.empty()){
			state_top = state_stack.top();
			state_stack.pop();
			states_count_pop++;
			//cout << "vertices:" << state_top->numberOfVertices << " edge index:" << state_top->edgeIndex << " depth:" << state_top->depth << endl;
			currentSolutionNumberOfEdges = state_top->getNumberOfEdges();
			bipartityTest = state_top->isBipartite();
			if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth && bipartityTest>-1){
				successors = state_top->getSuccessors();

				//push state without edge
				state_stack.push(successors[0]);
				states_count_push++;
				//push state with edge
				state_stack.push(successors[1]);
				states_count_push++;
				delete successors;
			}

			if(bipartityTest == 1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
				bestSolutionNumberOfEdges = currentSolutionNumberOfEdges;
				bestSolution = state_top;
			} else {
				delete state_top;
			}
		}
		cout << "end while" << endl;
	}
	cout << "Error : states_pop:" << states_count_pop <<endl;

	cout << "***BEST SOLUTION p" << communicator.rank << endl;
	bestSolution->print();
	delete bestSolution;

	communicator.finalize();
	return 0; }*/



