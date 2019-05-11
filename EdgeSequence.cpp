//
// Created by paulp on 5/9/2019.
//

#include "EdgeSequence.h"

void EdgeSequence::pushEdgeGroup(std::set<Edge> edgeGroup)
{
	backEdgeGroups.push_front(edgeGroup);
}

std::set<Edge> &EdgeSequence::peekEdgeGroup()
{
	return *backEdgeGroups.begin();
}

std::set<Edge> EdgeSequence::popEdgeGroup()
{
	std::set<Edge> group = *backEdgeGroups.begin();
	backEdgeGroups.pop_front();
	return group;
}

//this function gets a list of upcoming edge groups which are edges coming out of the dst vertex which have dst nodes with the same label
//there may be multiple edge groups that come out of one edge group
//
//	6	6		6	6
//	1\	2\		3\	4\		current edge group contains edges:{1,2,3,4}
//	  4   4		  4   4 		next edge groups are {{5,6}, {7,8}}
//	  5\  6\	  7\  8\
//		3	3		2	2

std::list<std::set<Edge>> EdgeSequence::getFollowingEdgeGroups()
{
	std::set<Edge> &currentEdgeGroup = *backEdgeGroups.begin();
	std::map<int, std::set<Edge>> edgeSeperator;
	
	//here we separate all the edges following our edge group into edge groups
	for (Edge edge : currentEdgeGroup)
	{
		Vertex &nextEdgeSrc = edge.getDst();
		//here we know that we have either 1 or 0 children because we are using an inverted tree
		std::set<Edge> childEdges = nextEdgeSrc.getAdjacentEdges();
		if (!childEdges.empty())
		{
			Edge nextEdge = *childEdges.begin();
			edgeSeperator[nextEdge.getDst().getLabel()].insert(nextEdge);
		}
	}
	
	//now we insert these edge groups into a list
	std::list<std::set<Edge>> nextEdgeGroups;
	for (std::pair<const int, std::set<Edge>> edgeSetPair : edgeSeperator)
	{
		nextEdgeGroups.insert(nextEdgeGroups.begin(), edgeSetPair.second);
	}
	
	return nextEdgeGroups;
}

int EdgeSequence::getMultiplicity()
{
	return (int)(*backEdgeGroups.begin()).size();
}

std::string EdgeSequence::toStringRepresentation()
{
	std::string result;
	bool first = true;
	
	//reverse iterate through the list and append the sequence elements to the result description
	for (std::set<Edge> &backEdgeGroup : backEdgeGroups)
	{
		const Edge& rEdge = *backEdgeGroup.begin();
		if(first)
		{
			result += std::to_string(rEdge.getDst().getLabel());
			first = false;
		}
		result += "->" + std::to_string(rEdge.getSrc().getLabel());
	}
	//append the multiplicity
	result += "(multiplicity:"+std::to_string(getMultiplicity())+")";
	
	return result;
}
