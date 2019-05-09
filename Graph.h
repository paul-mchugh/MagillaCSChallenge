//
// Created by paulp on 5/6/2019.
//

#ifndef MAGILLACSCHALLENGE_GRAPH_H
#define MAGILLACSCHALLENGE_GRAPH_H

#include <set>
#include <map>
#include <memory>

class Graph;
class Edge;

class Vertex
{
	Graph *partOf;
	int uid;
	int label;
	std::set<int> adjacencyList;//stores all vertices that are adjacent to this vertex
	std::set<int> incidenceList;//stores all vertices that have edges incident upon this vertex
	//friend the Graph class so it can call our private constructor
	friend class Graph;
	
	Vertex(Graph *partOf, int uid, int label);

public:
	//this constructor is required for std::map to work properly  DON'T USE IT
	Vertex() : partOf(nullptr), uid(-1), label(-1) {}
	
	bool operator==(const Vertex &other) const;
	bool operator<(const Vertex &other) const;
	
	int getLabel();
	std::set<Vertex*> getChildren();
	std::set<Vertex*> getParents();
	Vertex& getParent();
};

class Edge
{
	Vertex &src;
	Vertex &dst;
	//friend the Graph class so it can call our private constructor
	friend class Graph;
protected:
	Edge(Vertex& src, Vertex& dst);
	
public:
	bool operator==(const Edge &other) const;
	bool operator<(const Edge &other) const;
	Vertex& getSrc() const;
	Vertex& getDst() const;
};

class LogicalEdge : public Edge
{
	friend class Graph;
	LogicalEdge(Vertex& src, Vertex& dst) : Edge(src,dst) {};
public:
	bool operator==(const Edge &other) const;
	bool operator<(const Edge &other) const;
};

class Graph
{
	//graph is represented in adjacency list format
	std::map<int,std::unique_ptr<Vertex>> vertexSet;
	int lastUID = 0;

public:
	//default constructor, copy constructor, copy assign constructor, move constructor, and move assign constructor
	Graph() = default;
	Graph(const Graph &other);
	Graph& operator=(const Graph &other);
	Graph(Graph &&other) noexcept;
	Graph& operator=(Graph &&other) noexcept;
	//getters
	std::set<Edge> getEdgeSet();
	std::set<Vertex*> getVertexSet();
	std::set<Vertex*> getHeads();
	//manipulation functions
	bool addEdge(Vertex &src, Vertex &dst);
	bool removeEdge(Vertex &src, Vertex &dst);
	bool removeEdge(Edge e);
	void removeEdgeSet(std::set<Edge> edgeSet);
	Vertex& createVertex(int label);
	Vertex& getVertexByUID(int uid);
	bool removeVertex(Vertex &v);
	void removeIsolatedVertices(); //this method removes all vertices that have an in degree and out degree of 0
	void invert();
	
};

#endif //MAGILLACSCHALLENGE_GRAPH_H
