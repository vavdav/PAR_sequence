/*
 * GraphReader.h
 *
 *  Created on: Mar 14, 2010
 *      Author: smejkalp
 */
#ifndef GRAPHREADER_H_23462456345737
#define GRAPHREADER_H_23462456345737

#include <iostream>
#include <fstream>
#include "State.h"
#include <string>

using namespace std;

class GraphReader {

private:
	ifstream *inputStream;
	bool fileLoaded;
public:
	GraphReader();
	static State* getFirstStateFromFile(char * fileName);
	virtual ~GraphReader();
};

#endif /* GRAPHREADER_H_23462456345737 */

