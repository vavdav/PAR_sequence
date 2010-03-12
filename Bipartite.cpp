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

vector<State> getSuccessors (State * state){

}


int main() {
	int size = 5;
	matrix incidence(size, vector<int>(size));

	int incidence_array[5][5] = {
										{0,0,1,0,1},
										{0,0,0,1,0},
										{1,0,0,0,1},
										{0,1,0,0,1},
										{1,0,1,1,0}
	};

	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			incidence[i][j] = incidence_array[i][j];
			cout << incidence[i][j] << " ";
		}
		cout << endl;
	}

	State *state1 = new State(&incidence);


	stack<State*> state_stack;
	state_stack.push(state1);

	cout << "!!!Hello World!!!" << state1 << endl; // prints !!!Hello World!!!

	while(!state_stack.empty()){
		State *state_top = state_stack.top();
		state_stack.pop();
		getSuccessors(state_top);


	}
	return 0;
}


