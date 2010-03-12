/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>


State::State(matrix *incidence) {
	this->incidence = incidence;
}

State::~State() {
	// TODO Auto-generated destructor stub
}

std::ostream& State::operator<<(std::ostream& os){
	return os << "state wooooohooooo";
}
