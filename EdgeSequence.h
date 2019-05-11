//
// Created by paulp on 5/9/2019.
//

#ifndef MAGILLACSCHALLENGE_EDGESEQUENCE_H
#define MAGILLACSCHALLENGE_EDGESEQUENCE_H


#include <list>
#include <set>
#include "Graph.h"

class EdgeSequence
{
	std::list<std::set<Edge>> backEdgeGroups;

public:
	void pushEdgeGroup(std::set<Edge> edgeGroup);
	std::set<Edge> &peekEdgeGroup();
	std::set<Edge> popEdgeGroup();
	std::list<std::set<Edge>> getFollowingEdgeGroups();
	int getMultiplicity();
	std::string toStringRepresentation();
};


#endif //MAGILLACSCHALLENGE_EDGESEQUENCE_H
