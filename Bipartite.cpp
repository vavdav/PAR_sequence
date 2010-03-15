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
	State *state1 = reader.getFirstStateFromFile("zk.txt");
	stack<State*> state_stack;
	state_stack.push(state1);

	int states_count = 1;
	int best_solution = 0;
	int state1NumberOfEdges = state1->getNumberOfEdges();
	State *state_top;

	if(state1->isBipartite()){
		best = state1;
		writeSolution();
	} else {
		while(!state_stack.empty()){
			//cout << "states = " << states_count << endl;
			state_top = state_stack.top();
			//cout << "b0" << endl;
			//cout << " bla :" << state_top << endl;
			State **successors = state_top->getSuccessors();
			//cout << "b1" << endl;
			states_count++;
			state_stack.pop();

			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				//successors[i]->print();
				//cout << "dbg1 - " << i << endl;
				state_stack.push(successors[i]);
				//cout << "dbg2 - " << i << endl;
			}
		}
	}/*
			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				states_count++;
				cout << "dbg1" <<endl;
				if(state_top->getNumberOfEdges() >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth){ //musi existovat reseni s |F|=|V|-1 a max hloubka |E|
					cout << "dbg2" <<endl;
					state_stack.push(successors[i]);
					cout << "dbg3" <<endl;
					if(successors[i]->isBipartite()){
						//successors[i]->print();
						cout << "dbg4" <<endl;
						if(best_solution < successors[i]->getNumberOfEdges()){ //nalezt maximalni podmnozinu hran F (tady to asi muze skoncit)
							cout << "dbg5" <<endl;
							best = successors[i];
							best_solution = successors[i]->getNumberOfEdges();
							//cout << "***** BETTER SOLUTION ***** " << best_solution << endl;
						}
						//cout << "edges = " << successors[i]->getNumberOfEdges() << " depth:" << successors[i]->depth << endl;
						//return 0;
					}
				} else {
					cout << "dbg2" <<endl;
					writeSolution();
					cout << "states:" << states_count <<endl;
					return 0;
				}

			}
			cout << "bla5" << endl;

			//cout << "bla6" << endl;

		}
	}
	cout << "states:" << states_count <<endl;
*/
	return 0;
}


