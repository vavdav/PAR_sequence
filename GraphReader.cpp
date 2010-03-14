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

	matrix	adjacency(size, vector<int>(size));
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			adjacency[i][j] = adjacency_array[i][j];
		}
	}


	State *newState = new State(&adjacency,0);
}

GraphReader::GraphReader() {
	// TODO Auto-generated constructor stub

}

GraphReader::~GraphReader() {
	// TODO Auto-generated destructor stub
}
