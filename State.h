/*
 * State.h
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include <vector>

#ifndef STATE_H_24563457568768
#define STATE_H_24563457568768


typedef std::vector<  std::vector<int> > matrix;
class State {
public:

	matrix *incidence;
	State(matrix *incidence);
	virtual ~State();
	std::ostream& operator<<(std::ostream& os);

};

#endif /* STATE_H_24563457568768 */
