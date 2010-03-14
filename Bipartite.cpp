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

using namespace std;


int main() {
	int size = 5;
	matrix incidence(size, vector<int>(size));

	/*int incidence_array[5][5] = {
										{0,1,0,1,0},
										{1,0,1,0,1},
										{0,1,0,1,0},
										{1,0,1,0,1},
										{0,1,0,1,0}
	};*/

	int incidence_array[5][5] = {
											{0,1,1,1,1},
											{1,0,1,1,1},
											{1,1,0,1,1},
											{1,1,1,0,1},
											{1,1,1,1,0}
		};

	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			incidence[i][j] = incidence_array[i][j];
		}
	}
	State *state1 = new State(&incidence);
	cout << "state1->isBipartite() = " << state1->isBipartite() << endl;
	cout << "state1->getNumberOfEdges() = " << state1->getNumberOfEdges() << endl;
	stack<State*> state_stack;
	state_stack.push(state1);
	int states_count = 1;
	while(!state_stack.empty()){
		State *state_top = state_stack.top();

		State **succesors = state_top->getSuccessors();
		for(int i = 0; i<state_top->getNumberOfEdges(); i++){
			//succesors[i]->print();

		}
		state_stack.pop();
		for(int i = 0; i<state_top->getNumberOfEdges(); i++){
			states_count++;
			state_stack.push(succesors[i]);
			if(succesors[i]->isBipartite()){
				succesors[i]->print();
				return 0;

			}

		}
		//cout << "bla5" << endl;

		//cout << "bla6" << endl;

	}
	cout << "states:" << states_count <<endl;

	return 0;
}


