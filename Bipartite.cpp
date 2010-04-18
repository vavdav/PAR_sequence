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
#include <stack>
#include <vector>
#include "GraphReader.h"
#include <fstream>
#include "Communicator.h"
#include <string>

using namespace std;

#define MESSAGE_SIZE = 4;

void logMeIntoFile(int rank, int numProcesses){
	ofstream myfile;
	string fileName = "log_";
	fileName += rank + ".txt";
	myfile.open (fileName.c_str());
	myfile << "I'm processor no. "<< rank << " out of " << numProcesses << ".\n";
	myfile.close();
}

void logMeIntoCout(int rank, int numProcesses){
	cout << "I'm processor no. "<< rank << " out of " << numProcesses << ".\n";
}

State* distributeStates(Communicator *communicator, State * stateStart){
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
	state_top->print();

	return state_top;
}

void mainProccessor(Communicator *communicator, stack<State*> *state_stack, State * state1, char *fileName){
	GraphReader reader;
	State *stateStart = reader.getFirstStateFromFile(fileName);

	state1 = distributeStates(communicator, stateStart);
	cout << "p" << communicator->rank << " got first State" << endl;
	state1->print();
}

void otherProccessor(Communicator *communicator, stack<State*> *state_stack, State * state1){
	state1 = communicator->receiveState();
	cout << "p" << communicator->rank << " got first State" << endl;
	state1->print();
}

int main (int argc, char *argv[] )
{

	if ( argc != 2 ) {
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		return -1;
	}

	stack<State*> state_stack;
	State *state1 = NULL;
	Communicator communicator(argc, argv);

	if(communicator.rank == 0){
		mainProccessor(&communicator, &state_stack, state1, argv[1]);
	} else {
		otherProccessor(&communicator, &state_stack, state1);
	}

	if(state1 != NULL){
		delete state1;
	}

	communicator.finalize();

	return 0;
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
	return 0;*/
}


