//
// Created by paulp on 5/6/2019.
//

#include "Graph.h"

//Vertex implementation

Vertex::Vertex(Graph* partOf, int uid, int label) : partOf(partOf), uid(uid), label(label)
{}

bool Vertex::operator==(const Vertex &other) const
{
	return partOf==other.partOf&&uid==other.uid;
}

bool Vertex::operator<(const Vertex &other) const
{
	return label<other.label||uid<other.uid;
}

int Vertex::getLabel()
{
	return label;
}

std::set<Vertex*> Vertex::getChildren()
{
	std::set<Vertex*> res;
	
	for (int childUID : adjacencyList)
	{
		res.insert(&partOf->getVertexByUID(childUID));
	}
	
	return res;
}

std::set<Vertex*> Vertex::getParents()
{
	std::set<Vertex*> res;
	
	for (int parentUID : incidenceList)
	{
		res.insert(&partOf->getVertexByUID(parentUID));
	}
	
	return res;
}

Vertex& Vertex::getParent()
{
	int parentUID = *adjacencyList.begin();
	
	return partOf->getVertexByUID(parentUID);
}

//Edge implementation

Edge::Edge(Vertex &src, Vertex &dst) : src(src), dst(dst)
{}

bool Edge::operator==(const Edge &other) const
{
	return src==other.src && dst==other.dst;
}

bool Edge::operator<(const Edge &other) const
{
	return src==other.src ? dst<other.dst : src<other.src;
}

Vertex& Edge::getSrc()
{
	return src;
}

Vertex& Edge::getDst()
{
	return dst;
}

std::set<Edge> Graph::getEdgeSet()
{
	std::set<Edge> allEdges = std::set<Edge>();
	
	for (std::pair<const int,Vertex> srcPair : vertexSet)
	{
		Vertex &src = srcPair.second;
		
		for (int dstID : src.adjacencyList)
		{
			allEdges.insert(Edge(src,getVertexByUID(dstID)));
		}
	}
	
	return allEdges;
}

std::set<Vertex*> Graph::getVertexSet()
{
	std::set<Vertex*> vertexRefs;
	
	for(std::pair<const int,Vertex> vtxPair : vertexSet)
	{
		vertexRefs.insert(&vtxPair.second);
	}
	
	return vertexRefs;
}

std::set<Vertex*> Graph::getHeads()
{
	std::set<Vertex*> vertexRefs;
	
	for(std::pair<const int,Vertex> vtxPair : vertexSet)
	{
		Vertex &vtx = vtxPair.second;
		
		if(vtx.incidenceList.empty()) vertexRefs.insert(&vtx);
	}
	
	return vertexRefs;
}

bool Graph::addEdge(Vertex &src, Vertex &dst)
{
	//if src or dst don't exist or the edge src->dst already exists then return false else add it and return true
	if (vertexSet.count(src.uid)==1&& vertexSet.count(dst.uid)==1&& src.adjacencyList.count(dst.uid)==0)
	{
		src.adjacencyList.insert(dst.uid);
		dst.incidenceList.insert(src.uid);
		return true;
	}
	else
	{
		return false;
	}
}

bool Graph::removeEdge(Vertex &src, Vertex &dst)
{
	//if src, dst, or the edge src->dst don't exist then return false otherwise cut the edge
	if (vertexSet.count(src.uid)==1&& vertexSet.count(dst.uid)==1&& src.adjacencyList.count(dst.uid)==1)
	{
		src.adjacencyList.erase(dst.uid);
		dst.adjacencyList.erase(src.uid);
		return true;
	}
	else
	{
		return false;
	}
}

bool Graph::removeEdge(Edge e)
{
	return removeEdge(e.getSrc(),e.getDst());
}

void Graph::removeEdgeSet(std::set<Edge> edgeSet)
{
	for (Edge e : edgeSet)
	{
		removeEdge(e);
	}
}

Vertex &Graph::createVertex(int label)
{
	int id = lastUID++;
	Vertex vtx = Vertex(this,id,label);
	vertexSet[id] = vtx;
	return vertexSet[id];
}

Vertex &Graph::getVertexByUID(int uid)
{
	return vertexSet[uid];
}

bool Graph::removeVertex(Vertex &v)
{
	if (vertexSet.count(v.uid)==1)
	{
		vertexSet.erase(v.uid);
		return true;
	}
	else
	{
		return false;
	}
}

void Graph::removeIsolatedVertices()
{
	for (auto it = vertexSet.begin();it!=vertexSet.end();it++)
	{
		Vertex vtx = (*it).second;
		if (vtx.adjacencyList.empty() && vtx.incidenceList.empty()) vertexSet.erase(it);
	}
}

void Graph::invert()
{
	std::set<int> swp;
	for(std::pair<const int,Vertex> vtxPair : vertexSet)
	{
		Vertex vtx = vtxPair.second;
		swp = std::move(vtx.incidenceList);
		vtx.incidenceList = std::move(vtx.adjacencyList);
		vtx.adjacencyList = std::move(swp);
	}
}
