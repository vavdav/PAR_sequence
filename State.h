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


typedef std::vector<  std::vector<int> > matrix;

class State {
public:

	int numberOfVertices;
	matrix* incidence;
	State(matrix *incidence, int depth);
	int depth;
	State** getSuccessors();

	bool isBipartite();
	int getNumberOfEdges();
	int getNumberOfSuccessors(int index);
	void print();
	virtual ~State();
	std::ostream& operator<<(std::ostream& os);

};

#endif /* STATE_H_24563457568768 */
