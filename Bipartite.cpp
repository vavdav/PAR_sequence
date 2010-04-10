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

using namespace std;

State *best;

void writeSolution(){
	cout << "***BEST SOLUTION:" << endl;
	best->print();
}


int main (int argc, char *argv[] )
{
	char *fileName;


	if ( argc != 2 ) {// argc should be 2 for correct execution
		// We print argv[0] assuming it is the program name
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		//return -1;
	}
	fileName = argv[1];
	GraphReader reader;
	//State *state1 = reader.getFirstStateFromFile(fileName);
	State *state1 = reader.getFirstStateFromFile("zk.txt");
	stack<State*> state_stack;
	state_stack.push(state1);

	state1->getStateWithoutEdge(1);


	int states_count_push = 1;
	int states_count_pop = 0;
	int best_solution = 0;
	int state1NumberOfEdges = state1->getNumberOfEdges();
	State *state_top;

	int graphTest;
	if(state1->isBipartite() == 1){
		best = state1;
		writeSolution();
		return 0;
	} else {
		while(!state_stack.empty()){
			state_top = state_stack.top();
			State **successors = state_top->getSuccessors();
			state_stack.pop();
			states_count_pop++;
			cout << "here" << endl;
			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				if(state_top->getNumberOfEdges() >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth){ //musi existovat reseni s |F|=|V|-1 a max hloubka |E|
					graphTest = successors[i]->isBipartite();
					if(graphTest > -1){
						state_stack.push(successors[i]);
						states_count_push++;
					}
					if(graphTest == 1){
						best = successors[i];
						best_solution = successors[i]->getNumberOfEdges();
						writeSolution();
						cout << "states-push:" << states_count_push << ", states-pop:" << states_count_pop <<endl;
						while(!state_stack.empty()) { // delete the rest of the states on stack
							State* stateToBeDeleted = state_stack.top();
							state_stack.pop();
							delete stateToBeDeleted;
						}
						return 0;
					}
				}

			}
			delete state_top;
		}
	}
	cout << "Error : states_pop:" << states_count_pop <<endl;
	return -1;
}


