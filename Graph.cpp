#include <memory>

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

Vertex& Edge::getSrc() const
{
	return src;
}

Vertex& Edge::getDst() const
{
	return dst;
}

std::set<Edge> Graph::getEdgeSet()
{
	std::set<Edge> allEdges = std::set<Edge>();
	
	for (std::pair<const int,std::unique_ptr<Vertex>> &srcPair : vertexSet)
	{
		Vertex &src = *srcPair.second;
		
		for (int dstID : src.adjacencyList)
		{
			allEdges.insert(LogicalEdge(src,getVertexByUID(dstID)));
		}
	}
	
	return allEdges;
}

bool LogicalEdge::operator==(const Edge &other) const
{
	return getSrc().getLabel() == other.getSrc().getLabel() && getDst().getLabel() == other.getDst().getLabel();
}

bool LogicalEdge::operator<(const Edge &other) const
{
	return getSrc().getLabel() != other.getSrc().getLabel() ? getSrc().getLabel() < other.getSrc().getLabel() : getDst().getLabel() < other.getDst().getLabel();
}

std::set<Vertex*> Graph::getVertexSet()
{
	std::set<Vertex*> vertexRefs;
	
	for(std::pair<const int,std::unique_ptr<Vertex>> &vtxPair : vertexSet)
	{
		vertexRefs.insert(&(*vtxPair.second));
	}
	
	return vertexRefs;
}

std::set<Vertex*> Graph::getHeads()
{
	std::set<Vertex*> vertexRefs;
	
	for(std::pair<const int,std::unique_ptr<Vertex>> &vtxPair : vertexSet)
	{
		Vertex &vtx = *vtxPair.second;
		
		if(vtx.incidenceList.empty()) vertexRefs.insert(&vtx);
	}
	
	return vertexRefs;
}

Graph::Graph(const Graph &other) : lastUID(other.lastUID)
{
	for (const std::pair<const int, std::unique_ptr<Vertex>> &vtxPair : other.vertexSet)
	{
		vertexSet[vtxPair.first] = std::make_unique<Vertex>(*vtxPair.second);
	}
}

Graph& Graph::operator=(const Graph &other)
{
	lastUID = other.lastUID;
	vertexSet = std::map<int,std::unique_ptr<Vertex>>();
	for (const std::pair<const int, std::unique_ptr<Vertex>> &vtxPair : other.vertexSet)
	{
		vertexSet[vtxPair.first] = std::make_unique<Vertex>(*vtxPair.second);
	}
	return *this;
}

Graph::Graph(Graph &&other) noexcept : lastUID(other.lastUID), vertexSet(std::move(other.vertexSet))
{}

Graph& Graph::operator=(Graph &&other) noexcept
{
	lastUID = other.lastUID;
	vertexSet = std::move(other.vertexSet);
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
		dst.incidenceList.erase(src.uid);
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
	vertexSet[id] = std::unique_ptr<Vertex>(new Vertex(this,id,label));
	
	return *vertexSet[id];
}

Vertex &Graph::getVertexByUID(int uid)
{
	return *vertexSet[uid];
}

bool Graph::removeVertex(Vertex &v)
{
	if (vertexSet.count(v.uid)==1)
	{
		//remove self from children's incidence lists
		for (Vertex *child : v.getChildren())
		{
			child->incidenceList.erase(v.uid);
		}
		//remove self from parents adjacency list
		for (Vertex *parent : v.getParents())
		{
			parent->adjacencyList.erase(v.uid);
		}
		
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
	for (auto it = vertexSet.begin();it!=vertexSet.end();)
	{
		Vertex &vtx = *(*it).second;
		if (vtx.adjacencyList.empty() && vtx.incidenceList.empty())
		{
			it = vertexSet.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Graph::invert()
{
	std::set<int> swp;
	for(std::pair<const int,std::unique_ptr<Vertex>> &vtxPair : vertexSet)
	{
		Vertex &vtx = *vtxPair.second;
		swp = std::move(vtx.incidenceList);
		vtx.incidenceList = std::move(vtx.adjacencyList);
		vtx.adjacencyList = std::move(swp);
	}
}
