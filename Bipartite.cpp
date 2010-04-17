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

int main (int argc, char *argv[] )
{
	Communicator communicator(argc, argv);
	//logMeIntoFile(communicator.rank, communicator.numProcesses);
	logMeIntoCout(communicator.rank, communicator.numProcesses);

/*
	string message = "Ahoj, tady je pedro!";
	int message_length = message.length() + 1;
	char* buffer = new char[message_length];
	strcpy(buffer, message.c_str());


	if(communicator.rank == 0){

		int tag = 1;
		//char[MESSAGE_SIZE] buffer;
		for(int receiver = 1; receiver < communicator.numProcesses; receiver++){
			cout << "Processor " << communicator.rank << ": Sending [" << buffer << "] to processor " << receiver << endl;
			MPI_Send (buffer, message_length, MPI_CHAR, receiver, tag, MPI_COMM_WORLD);
			cout << "Processor " << communicator.rank << ": message sent to processor " << receiver << endl;
		}
	}
	else
	{
		MPI_Status status;
		int length;
		MPI_Recv(buffer, message_length, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		      /* getting info about received message */
		      //MPI_Get_count(&status, MPI_CHAR, &length);
		      /* message *//*
		      cout << "Processor " << communicator.rank << ": Received message: [" << buffer << "]" << endl;
	}

	cout << "Processor " << communicator.rank << " ending" << endl;
	delete buffer;


	communicator.finalize();
	return 0;
*/

	char *fileName;


	if ( argc != 2 ) {
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		return -1;
	}
	fileName = argv[1];
	//fileName = "graph10.txt";
	GraphReader reader;
	State *state1 = reader.getFirstStateFromFile(fileName);
	stack<State*> state_stack;
	state_stack.push(state1);

	int length = state1->numberOfVertices*state1->numberOfVertices + sizeof(int)*2;
	char *buffer = new char[length];

	state1->serialize(buffer, 0);

	State *deserializedState = State::deserialize(buffer, 0, state1->numberOfVertices);

	deserializedState->print();

	communicator.finalize();
	return 0;

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
	}
	cout << "Error : states_pop:" << states_count_pop <<endl;

	cout << "***BEST SOLUTION:" << endl;
	bestSolution->print();
	delete bestSolution;

	return -1;
}


