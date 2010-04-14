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

void logMeIntoFile(int rank, int numProcesses){
	ofstream myfile;
	string fileName = "log_";
	fileName += rank + ".txt";
	myfile.open (fileName.c_str());
	myfile << "I'm processor no. "<< rank << " out of " << numProcesses << ".\n";
	myfile.close();
}

int main (int argc, char *argv[] )
{
	Communicator communicator(argc, argv);
	logMeIntoFile(communicator.rank, communicator.numProcesses);
	return 0;


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


