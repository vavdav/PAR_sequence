/*
 * GraphReader.cpp
 *
 *  Created on: Mar 14, 2010
 *      Author: smejkalp
 */

#include "GraphReader.h"

State* GraphReader::getFirstStateFromFile(char *fileName){
	FILE *fileIn;
	int numOfvertices;
	int line = 0;
	matrix* adjacency;

	if((fileIn = fopen(fileName, "r")) == NULL) {
	    cout << "***Error: cant open file: " << fileName << endl;
	    exit(1);
	} else {
		cout << "*** Info: file openned: " << fileName << endl;
	}

	fscanf(fileIn, "%d\n", &numOfvertices);

	if(numOfvertices<2){
		cout << "***Error: just one vertice: " << endl;
		exit(1);
	}

	adjacency = new matrix(numOfvertices);

	cout << "*** LOADED MATRIX *** numOfvertices " << numOfvertices << endl;
	while (!feof(fileIn)) {
		adjacency->at(line) = new vector<int>(numOfvertices);
		for(int i = 0; i < numOfvertices; i++) {
			fscanf(fileIn, "%d ", &adjacency->at(line)->at(i));
			cout << adjacency->at(line)->at(i) << " ";
		}
		cout << endl;
		line += 1;
	}
	cout << "**************" << endl;
	fclose(fileIn);

	State *newState = new State(adjacency,0);

	return newState;
}

GraphReader::GraphReader() {
	// TODO Auto-generated constructor stub

}

GraphReader::~GraphReader() {
	// TODO Auto-generated destructor stub
}
