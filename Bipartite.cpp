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

#include <time.h>

using namespace std;

void writeSolution(State *bestSolution){
	cout << "***BEST SOLUTION:" << endl;
	bestSolution->print();
}


int main (int argc, char *argv[] )
{
	char *fileName;
	time_t seconds0;
	time_t seconds1;


	/*if ( argc != 2 ) {
		cout<<"usage: "<< "PAR" <<" <filename>" << endl;
		return -1;
	}
	fileName = argv[1];*/
	fileName = "data/data12-7.txt";
	GraphReader reader;
	State *state1 = reader.getFirstStateFromFile(fileName);
	stack<State*> state_stack;
	state_stack.push(state1);

	double states_count_push = 1;
	double states_count_pop = 0;
	int state1NumberOfEdges = state1->getNumberOfEdges();
	State *state_top;
	State **successors;

	State *bestSolution;
	bestSolution = NULL;
	int bestSolutionNumberOfEdges = 0;
	int currentSolutionNumberOfEdges;
	int bipartityTest;
	//main cycle

	seconds0 = time (NULL);

	if(state1->isBipartite() == 1){
		bestSolution = state1;

	} else {
		while(!state_stack.empty()){
			state_top = state_stack.top();
			state_stack.pop();
			states_count_pop++;
			currentSolutionNumberOfEdges = state_top->getNumberOfEdges();
			bipartityTest = state_top->isBipartite();
			if(currentSolutionNumberOfEdges >= state_top->numberOfVertices-1 && state1NumberOfEdges >= state_top->depth && bipartityTest>-1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
				successors = state_top->getSuccessors();
				if(successors != NULL){
					//push state without edge
					state_stack.push(successors[0]);
					states_count_push++;
					//push state with edge
					state_stack.push(successors[1]);
					states_count_push++;
					delete [] successors;
				}
			}
			if(bipartityTest == 1 && currentSolutionNumberOfEdges > bestSolutionNumberOfEdges){
				bestSolutionNumberOfEdges = currentSolutionNumberOfEdges;
				bestSolution = state_top;
				cout << "have best solution " << bestSolutionNumberOfEdges << endl;
				seconds1 = time (NULL);
				cout << "time:" << (seconds1-seconds0) << endl;
			} else {
				delete state_top;
			}
		}

	}
	cout << "end" << endl;
	seconds1 = time (NULL);
	cout << "time:" << (seconds1-seconds0) << endl;
	cout << "end : states_pop/push:" << states_count_pop << "/" << states_count_push << endl;
	if(bestSolution == NULL){
		cout << "nenalezeno zadne reseni, neni souvisly graf" << endl;
	} else {
		writeSolution(bestSolution);
	}
	delete bestSolution;

	return -1;
}


