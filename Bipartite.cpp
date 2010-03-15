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



int main() {
	GraphReader reader;
	State *state1 = reader.getFirstStateFromFile("graph30.txt");
	stack<State*> state_stack;
	state_stack.push(state1);

	int states_count = 1;
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
			if(state_top){
				delete state_top;
			}
			state_top = state_stack.top();
			State **successors = state_top->getSuccessors();
			state_stack.pop();

			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				if(state_top->getNumberOfEdges() >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth){ //musi existovat reseni s |F|=|V|-1 a max hloubka |E|
					graphTest = successors[i]->isBipartite();
					//if(graphTest > -1){
						state_stack.push(successors[i]);
						states_count++;
					//}
					if(graphTest == 1){
						best = successors[i];
						best_solution = successors[i]->getNumberOfEdges();
						writeSolution();
						cout << "states:" << states_count <<endl;
						while(!state_stack.empty()) { // delete the rest of the states on stack
							State* stateToBeDeleted = state_stack.top();
							state_stack.pop();
							delete stateToBeDeleted;
						}
						return 0;
					}
				}
			}
		}
	}
	cout << "Error : states:" << states_count <<endl;
	return -1;
}


