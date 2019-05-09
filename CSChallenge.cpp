#include <iostream>
#include "Graph.h"

void dfsPrint(Graph &g);
Graph generateStandardTree();
void printOutputSequences(Graph g);

int main(int argc, char *argv[])
{
	std::cout << "Magilla CS Coding Challenge" << std::endl;
	
	if (argc!=2)
	{
		std::cout << "Usage: " << std::string(argv[0]) << " <input file>" << std::endl;
	}
	
	Graph g = generateStandardTree();
	
	printOutputSequences(g);
	
	return 0;
}

void printOutputSequences(Graph g)
{
	//first we need to cull the edges the edges which are non-duplicate and therefore not part of a sequence
	std::set<Edge> edges = g.getEdgeSet();
	//we are using logical edges here because I want to use different comparison functions for this map that treat edges between same labeled edges as the same
	std::map<LogicalEdge,int> edgeMultiplicity;
	//in this loop we take a count of howmany logically equivalent edges exist
	for (Edge edge : edges)
	{
		LogicalEdge &logicalEdge = static_cast<LogicalEdge&> (edge);
		++edgeMultiplicity[logicalEdge];
	}
	//and here we remove the multiplicity 1 logical edges
	for (auto edge : edgeMultiplicity)
	{
		if(edge.second==1)
			g.removeEdge(edge.first);
	}
	//and now we clean up the graph by culling the isolated vertices
	g.removeIsolatedVertices();
	
	//invert the graph
	g.invert();
	
	dfsPrint(g);
}

Graph generateStandardTree()
{
	//ASCII art of the graph theat this function returns
	//				   6
	//			     /   \
	//			   4       8
	//			  / \       \
	//			3     5     12
	//			 \     \    / \
	//			 1      3  6  2
	//			/ \    / \  \
	//		   0   2  1   1  4
	//					   \
	//						0
	
	Graph g;
	
	Vertex &root	= g.createVertex(6);
	Vertex &l		= g.createVertex(4);
	Vertex &ll		= g.createVertex(3);
	Vertex &llr		= g.createVertex(1);
	Vertex &llrl	= g.createVertex(0);
	Vertex &llrr	= g.createVertex(2);
	Vertex &lr		= g.createVertex(5);
	Vertex &lrr		= g.createVertex(3);
	Vertex &lrrl	= g.createVertex(1);
	Vertex &lrrr	= g.createVertex(1);
	Vertex &lrrrr	= g.createVertex(0);
	Vertex &r		= g.createVertex(8);
	Vertex &rr		= g.createVertex(12);
	Vertex &rrl		= g.createVertex(6);
	Vertex &rrlr	= g.createVertex(4);
	Vertex &rrr		= g.createVertex(2);
	
	g.addEdge(root,l);
	g.addEdge(l,ll);
	g.addEdge(ll,llr);
	g.addEdge(llr,llrl);
	g.addEdge(llr,llrr);
	g.addEdge(l,lr);
	g.addEdge(lr,lrr);
	g.addEdge(lrr,lrrl);
	g.addEdge(lrr,lrrr);
	g.addEdge(lrrr,lrrrr);
	g.addEdge(root,r);
	g.addEdge(r,rr);
	g.addEdge(rr,rrl);
	g.addEdge(rrl,rrlr);
	g.addEdge(rr,rrr);
	
	return g;
}

void dfsPrintNode(Graph &g, Vertex *vtx, int depth, std::map<Vertex*,bool> &visited)
{
	if(!visited[vtx])
	{
		for (int i = 0; i < depth; ++i) std::cout << " ";
		std::cout << vtx->getLabel() << std::endl;
		visited[vtx] = true;
		for (Vertex *child : vtx->getChildren()) dfsPrintNode(g, child, depth + 1, visited);
	}
}

void dfsPrint(Graph &g)
{
	std::map<Vertex*,bool> visited;
	for (Vertex *vtx : g.getHeads())
	{
		dfsPrintNode(g, vtx, 0, visited);
	}
}
