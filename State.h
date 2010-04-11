/*
 * State.h
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include <vector>
#include <queue>

#ifndef STATE_H_24563457568768
#define STATE_H_24563457568768


typedef std::vector<  std::vector<int>* > matrix;

class State {
public:

	int numberOfVertices;
	matrix* adjacency;
	State(matrix *adjacency, int depth, int edgeIndex);
	int depth;
	State** getSuccessors();
<<<<<<< HEAD

	int edgeIndex;

	int getNextEdgeIndex();

=======
	State* getStateWithoutEdge(int edgeNumber);
	State* getCopy();
>>>>>>> 13e4021ddee162d0c3798a3f48f36d803669c3dd
	int isBipartite();
	int getNumberOfEdges();
	int getNumberOfSuccessors(int index);
	void print();
	virtual ~State();

};

#endif /* STATE_H_24563457568768 */
