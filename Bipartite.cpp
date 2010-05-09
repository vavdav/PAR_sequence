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
int bestSolutionNumberOfEdgesIncome;
int maxEdgeIndex;

double t1, t2;

void processMessages(){
	switch(communicator->getMessageType()){
		case Communicator::NEW_CURRENT_BEST:
			bestSolutionNumberOfEdgesIncome = communicator->receiveNewBestSoultionNumberOfEdges();
			if(bestSolutionNumberOfEdges < bestSolutionNumberOfEdgesIncome){
				bestSolutionNumberOfEdges = bestSolutionNumberOfEdgesIncome;
				delete bestSolution;
				bestSolution = NULL;
			} else {
				communicator->sendNewBestSolutionNumberOfEdgesToAll(bestSolutionNumberOfEdges);
			}
			break;
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
			//cout << "p" << communicator->rank << " stack:" << state_stack->size() << endl;
			communicator->receiveStack(state_stack, communicator->status.MPI_SOURCE);
			//cout << "p" << communicator->rank << " rstack:" << state_stack->size() << endl;
			communicator->hasRequestedWork = false;
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

				//procesor ktery odeslal praci si ulozi flag komu, kdyz prijde pesek a on poslal praci nizsimu procesoru tak MUSI poslat cerny token
				if(communicator->isWaiting){
					communicator->sendTokenWhite();
				} else {
					communicator->sendTokenBlack();
				}
			}
		break;
	}
}
void processSolution(int * counter){
	switch(communicator->getMessageType()){
		case Communicator::SOLUTION:
			State * receivedBest;
			receivedBest = communicator->receiveBestSolution();
			if(bestSolutionNumberOfEdges < receivedBest->getNumberOfEdges()){
				bestSolution = receivedBest;
			}
			counter--;
			break;
		case Communicator::NO_SOLUTION:
			communicator->receiveNoSolution();
			counter--;
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

	//ma graf vic hran nez uzlu? && pokud je hloubka vetsi nez pocet hran, tak uz nema cenu jet dal && je graf souvisly && muze byt toto reseni vubec lepsi nez nejlepsi nalezene?
	if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges > state_top->depth && bipartityTest>-1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
		successors = state_top->getSuccessors();
		if(successors != NULL){
			//push state without edge
			state_stack->push(successors[0]);
			states_count_push++;
			//push state with edge
			state_stack->push(successors[1]);
			states_count_push++;
			delete [] successors;
		}
	}
	if(bipartityTest == 1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
		//cout << communicator->rank << " best sol:" << currentSolutionNumberOfEdges << endl;
		bestSolutionNumberOfEdges = currentSolutionNumberOfEdges;
		bestSolution = state_top;
		communicator->sendNewBestSolutionNumberOfEdgesToAll(bestSolutionNumberOfEdges);
	} else {
		delete state_top;
	}
}

void compute(){
	communicator->synchronizeBarrier();
	t1=MPI_Wtime();

	cout << "compute start" << communicator->rank << endl;
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
	cout << "compute end " << communicator->rank << endl;

	communicator->synchronizeBarrier();
	t2=MPI_Wtime();
	cout << (t2-t1) << endl;
	if(communicator->rank == 0){
		int counter = communicator->numProcesses;
		int maxCycle = 0;
		/*while(counter != 1){
			processSolution(&counter);
			if(maxCycle == 200) {
				break;
			}
			maxCycle++;
		}*/
		if(bestSolution != NULL){
			bestSolution->getBipartiteGroups();
			delete bestSolution;
		}
	} else {
		//communicator->sendBestSolution(bestSolution);
		if(bestSolution != NULL){
			bestSolution->getBipartiteGroups();
			delete bestSolution;
		}
	}

}

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
			if(successors != NULL){
  			//push state without edge
  			distribute_stack.push(successors[0]);
  			//push state with edge
  			distribute_stack.push(successors[1]);
  			delete [] successors;
			}
		}
		delete state_top;
	}

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
	maxEdgeIndex = -1;
	State *state1 = NULL;
	state_stack = new stack<State*>();
	communicator = new Communicator(argc, argv);

	bestSolutionNumberOfEdges = 0;
	cout << "hello from " << communicator->rank << " numProc: " << communicator->numProcesses << endl;
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


