/*
 * GraphReader.cpp
 *
 *  Created on: Mar 14, 2010
 *      Author: smejkalp
 */

#include "GraphReader.h"

State* GraphReader::getFirstStateFromFile(char *fileName){
	FILE *fileIn;
	if((fileIn = fopen(fileName, "r")) == NULL) {
	    cout << "***Error: cant open file: " << fileName << endl;
	    exit(1);
	} else {
		cout << "*** Info: file openned: " << fileName << endl;

	}
	//fscanf(fileIn, "%d\n", &vertices);


	fclose(fileIn);

	//cout << "*** Info: fvertices: " << vertices << endl;

	int size = 5;

	matrix* adjacency;

	adjacency = new matrix();

	int adjacency_array[5][5] = {
								{0,1,1,1,1},
								{1,0,1,1,1},
								{1,1,0,1,1},
								{1,1,1,0,1},
								{1,1,1,1,0}
	};


	for(int i = 0; i < size;i++ ){
		adjacency->push_back(new vector<int>);
		for(int j = 0; j < size; j++){
			vector<int>* v = adjacency->at(i);
			int x = adjacency_array[i][j];
			v->push_back(x);
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
