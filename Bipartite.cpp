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

	/*
	if ( argc != 2 ) {// argc should be 2 for correct execution
		// We print argv[0] assuming it is the program name
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		return -1;
	}
	fileName = argv[1];
	*/
	fileName = "graph5.txt";
	GraphReader reader;
	State *state1 = reader.getFirstStateFromFile(fileName);
	State *state_top;
	stack<State*> state_stack;
	state_stack.push(state1);
	int bipartite_graphs = 0;
	int edgeToGetOutNumber = 0;

	int numberOfVertices = state1->numberOfVertices;
	int states_count_push = 1;
	int states_count_pop = 0;

	while(!state_stack.empty()){
		cout << "pops = " << states_count_pop << ", pushes = " << states_count_push << endl;
		state_top = state_stack.top();
		state_stack.pop();
		if(state_top->isBipartite() == 1){
			bipartite_graphs++;
		}
		states_count_pop++;
		if(edgeToGetOutNumber < numberOfVertices/2){
			state_stack.push(state_top->getCopy());
			states_count_push++;
			state_stack.push(state_top->getStateWithoutEdge(edgeToGetOutNumber));
			states_count_push++;
		}
		edgeToGetOutNumber++;
		delete state_top;
	}

	cout << "States_pop: " << states_count_pop << ". States_push: " << states_count_push <<endl;
	cout << "Bipartite graphs = " << bipartite_graphs << endl;
	return 0;
}


