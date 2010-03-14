/*
 * GraphReader.cpp
 *
 *  Created on: Mar 14, 2010
 *      Author: smejkalp
 */

#include "GraphReader.h"



State* GraphReader::getFirstStateFromFile(string fileName){
	int size = 5;
	int adjacency_array[5][5] = {
								{0,1,1,1,1},
								{1,0,1,1,1},
								{1,1,0,1,1},
								{1,1,1,0,1},
								{1,1,1,1,0}
	};

	//matrix* adjacency = new *vector<vector<int>* >();

	/*
	for(int i = 0; i < size; i++){
		adjacency->add(new vector<int>);
		for(int j = 0; j < size; j++){
			adjacency[i][j] = adjacency_array[i][j];
		}
	}

	*/
	//State *newState = new State(adjacency,0);

	//return newState;
}

GraphReader::GraphReader() {
	// TODO Auto-generated constructor stub

}

GraphReader::~GraphReader() {
	// TODO Auto-generated destructor stub
}
