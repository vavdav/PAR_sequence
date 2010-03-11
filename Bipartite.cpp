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

using namespace std;

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
		}
	}

	//State *firstState = new State(incidence);
	//State state2 = new State(incidence);
	State state2(&incidence);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
