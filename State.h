/*
 * State.h
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include <vector>
#include <queue>
#include "mpi.h"

#ifndef STATE_H_24563457568768
#define STATE_H_24563457568768


typedef std::vector<  std::vector<int>* > matrix;

class State {
public:

	int numberOfVertices;
	matrix* adjacency;
	int depth;
	int edgeIndex;

	State(matrix *adjacency, int depth, int edgeIndex);

	State** getSuccessors();

	int getNextEdgeIndex();

	int isBipartite();
	int getNumberOfEdges();
	int getNumberOfSuccessors(int index);
	void print();
	virtual ~State();

	char* serialize(char * buffer, int position);

	static State* deserialize(char* buffer, int position, int numberOfVertices);

};

#endif /* STATE_H_24563457568768 */
