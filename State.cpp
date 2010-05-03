/*
 * State.cpp
 *
 *  Created on: Mar 4, 2010
 *      Author: smejkalp
 */

#include "State.h"
#include <iostream>

using namespace std;


State::State(matrix *adjacency, int depth, int edgeIndex)
{
	this->depth = depth;
	this->adjacency = adjacency;
	this->edgeIndex = edgeIndex;
	this->numberOfVertices = this->adjacency->size();

}


State** State::getSuccessors(){
	matrix *newAdjacency0 = new matrix(this->numberOfVertices);
	matrix *newAdjacency1 = new matrix(this->numberOfVertices);
	State** states = new State*[2];
	int iToChange = -1;
	int jToChange = -1;
	bool successorFound;

	for(int i = 0; i < this->numberOfVertices; i++){
		newAdjacency0->at(i) = new vector<int>(this->numberOfVertices);
		newAdjacency1->at(i) = new vector<int>(this->numberOfVertices);
	}
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = i; j < this->numberOfVertices; j++){
			newAdjacency0->at(i)->at(j) = this->adjacency->at(i)->at(j);
			newAdjacency0->at(j)->at(i) = this->adjacency->at(i)->at(j);
			newAdjacency1->at(i)->at(j) = this->adjacency->at(i)->at(j);
			newAdjacency1->at(j)->at(i) = this->adjacency->at(i)->at(j);

			//POROVNAM ZDA JSEM ZA INDEXEM AKTUALNI HO SOUSEDA
			if(!successorFound && this->numberOfVertices*i+j > edgeIndex){
				if(this->adjacency->at(i)->at(j) == 1){
					iToChange = i;
					jToChange = j;
					successorFound = true;
				}
			}
		}
	}
	if (successorFound){
		newAdjacency0->at(iToChange)->at(jToChange) = 0;
		newAdjacency0->at(jToChange)->at(iToChange) = 0;

		states[0] = new State(newAdjacency0, this->depth+1, this->numberOfVertices*iToChange+jToChange);
		states[1] = new State(newAdjacency1, this->depth+1, this->numberOfVertices*iToChange+jToChange);

		newAdjacency0 = NULL;
		newAdjacency1 = NULL;
		delete newAdjacency0;
		delete newAdjacency1;
	} else {
		for(int i = 0; i < this->numberOfVertices; i++){
			delete newAdjacency0->at(i);
			delete newAdjacency1->at(i);
		}
		delete newAdjacency0;
		delete newAdjacency1;
		delete [] states;
		states = NULL;
	}

	return states;
}

void State::print(){
	//cout << "State : number of verticies = " << this->numberOfVertices << endl << "*******************" << endl;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			cout << this->adjacency->at(i)->at(j) << " ";
		}
		cout << endl;
	}
	cout << "*******************" << endl;
}

int State::isBipartite(){
	int* color = new int[this->numberOfVertices];
	int* d = new int[this->numberOfVertices];
	int* p = new int[this->numberOfVertices];
	for (int i=0; i<this->numberOfVertices; i++){
		color[i] = 0;
		d[i] = 99999;
		p[i] = 0;
	}
	int pom;
	int start = 0;
	queue <int> bfsFront;
	color[start] = 1;
	d[start] = 0;
	bfsFront.push(start);
	int path1Length;
	int path2Length;

	int bipartite = 1;

	while (!bfsFront.empty()){
		pom = bfsFront.front();
		//cout << "in vertex " << pom << endl;
		bfsFront.pop();
		for (int i=0; i<this->numberOfVertices; i++){
			if(this->adjacency->at(pom)->at(i) == 1){
				if (color[i] == 0) {
					color[i] = 1;
					d[i] = d[pom] + 1;
					p[i] = pom;
					bfsFront.push(i);
				} else {
					//cout << "I was here " << i << " old d=" << d[i]%2 << " ?= " << (d[pom] + 1)%2 << endl;
					path1Length = (d[i])%2;
					path2Length = (d[pom] + 1)%2;
					if(path1Length != path2Length){
						//return false;
						bipartite = 0;
						//return 0;
					}
				}
			}
		color[pom] = 2;
		}
	}
	for(int i = 0; i<this->numberOfVertices; i++){
		if(color[i] == 0) {
			bipartite = -1;
		}
	}
	delete [] color;
	delete [] d;
	delete [] p;

	return bipartite;
}

int State::getNumberOfSuccessors(int index){
	int numOnes = 0;
	for(int j = 0; j < this->numberOfVertices; j++){
		if (this->adjacency->at(index)->at(j) == 1){
			numOnes++;
		}
	}
	return numOnes;
}

int State::getNumberOfEdges(){
	int numOnes = 0;
	for(int i = 0; i < this->numberOfVertices; i++){
		for(int j = 0; j < this->numberOfVertices; j++){
			if (this->adjacency->at(i)->at(j) == 1){
				numOnes++;
			}
		}
	}
	return numOnes/2;
}


State::~State() {
	for(int i = 0; i < this->numberOfVertices; i ++){
		delete this->adjacency->at(i);
	}
	delete this->adjacency;
}
