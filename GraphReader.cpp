/*
 * GraphReader.cpp
 *
 *  Created on: Mar 14, 2010
 *      Author: smejkalp
 */

#include "GraphReader.h"

State* GraphReader::getFirstStateFromFile(string fileName){

	int size = 5;

	vector<vector<int>* >* adjacency;

	adjacency = new vector<vector<int>* >();

	int adjacency_array[5][5] = {
								{0,1,1,1,1},
								{1,0,1,1,1},
								{1,1,0,1,1},
								{1,1,1,0,1},
								{1,1,1,1,0}
	};

	for(int i = 0; i < size;i++ ){
		adjacency->push_back(new vector<int>);
		for(int j = 0; j < 5; j++){
			adjacency->at(i)->at(j) = adjacency_array[i][j];
		}
	}

	State *newState = new State(adjacency,0);

	return newState;
}

GraphReader::GraphReader() {
	// TODO Auto-generated constructor stub

}

GraphReader::~GraphReader() {
	// TODO Auto-generated destructor stub
}
