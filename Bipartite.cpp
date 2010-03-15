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

	//GraphReader reader;

	int size = 5;
		int adjacency_array[5][5] = {
									{0,1,1,1,1},
									{1,0,1,1,1},
									{1,1,0,1,1},
									{1,1,1,0,1},
									{1,1,1,1,0}
		};

		matrix adjacency(size, vector<int>(size));
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				adjacency[i][j] = adjacency_array[i][j];
			}
		}


		State *state1 = new State(&adjacency,0);


	//State *state1 = reader.getFirstStateFromFile("zk.txt");
	cout << "state1->isBipartite() = " << state1->isBipartite() << endl;
	cout << "state1->getNumberOfEdges() = " << state1->getNumberOfEdges() << endl;
	stack<State*> state_stack;
	state_stack.push(state1);

	int states_count = 1;
	int best_solution = 0;
	int state1NumberOfEdges = state1->getNumberOfEdges();

	if(state1->isBipartite()){
		best = state1;
		writeSolution();
	} else {

		while(!state_stack.empty()){
			State *state_top = state_stack.top();
			State **successors = state_top->getSuccessors();
			state_stack.pop();
			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				//succesors[i]->print();

			}
			for(int i = 0; i<state_top->getNumberOfEdges(); i++){
				states_count++;
				if(state_top->getNumberOfEdges() >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth){ //musi existovat reseni s |F|=|V|-1 a max hloubka |E|
					state_stack.push(successors[i]);
					if(successors[i]->isBipartite()){
						//successors[i]->print();
						if(best_solution < successors[i]->getNumberOfEdges()){ //nalezt maximalni podmnozinu hran F (tady to asi muze skoncit)
							best = successors[i];
							best_solution = successors[i]->getNumberOfEdges();
							//cout << "***** BETTER SOLUTION ***** " << best_solution << endl;
						}
						//cout << "edges = " << successors[i]->getNumberOfEdges() << " depth:" << successors[i]->depth << endl;
						//return 0;
					}
				} else {
					writeSolution();
					cout << "states:" << states_count <<endl;
					return 0;
				}

			}
			//cout << "bla5" << endl;

			//cout << "bla6" << endl;

		}
	}
	cout << "states:" << states_count <<endl;

	return 0;
}


