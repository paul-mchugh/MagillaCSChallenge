#include <iostream>
#include <stack>
#include <list>
#include "Graph.h"
#include "EdgeSequence.h"

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
	std::map<Edge,int,LogicallyLess> edgeMultiplicity;
	//in this loop we take a count of how many logically equivalent edges exist
	for (Edge edge : edges)
	{
		Edge &logicalEdge = static_cast<Edge&> (edge);
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
	
	edges = g.getEdgeSet();
	//this map stores groups of edges that have the same source and destination labels
	std::map<Edge, std::set<Edge>, LogicallyLess> equivalentEdges;
	//copy the edges into sets of edges that have the same source label as the key to the mapping
	for(Edge edge : edges)
	{
		equivalentEdges[edge].insert(edge);
	}
	//knownStartEdges store a list of already discovered start edges, so we don't double count them
	std::set<Edge,LogicallyLess> knownStartEdges;
	std::list<EdgeSequence> workList;
	std::list<EdgeSequence> doneList;
	//get heads set so we know which edges to start the trace with
	for(Vertex* head : g.getHeads())
	{
		//get the sequences corresponding to the heads, make the incomplete sequence and add it to the work list
		//there is no need to check to see if the adjacency list is empty because we know this has a head as source
		std::set<Edge> startEdgeSet = equivalentEdges[*head->getAdjacentEdges().begin()];
		//check to make sure we are not double (or triple etc) counting an edge
		if(knownStartEdges.count(*startEdgeSet.begin())==0)
		{
			knownStartEdges.insert(*startEdgeSet.begin());
			EdgeSequence sequenceStart;
			sequenceStart.pushEdgeGroup(startEdgeSet);
			workList.insert(workList.begin(),sequenceStart);
		}
	}
	
	//remove incomplete sequences from the work list add to them and put them back in the done list
	while (!workList.empty())
	{
		//pop EdgeSequence off workList
		EdgeSequence seq = *workList.begin();
		workList.pop_front();
		
		//pull next edge sets out and extend/clone the sequences
		std::list<std::set<Edge>> nextEdgeGroups = seq.getFollowingEdgeGroups();
		std::set<Edge> &currentSet = seq.peekEdgeGroup();
		bool sequenceCompleted = true;
		for (const std::set<Edge> &nextEdgeSet : nextEdgeGroups)
		{
			Edge repEdge = *nextEdgeSet.begin();//we don't need to check for nextEdgeSet size b/c the set wouldn't have been created if it was empty
			//if there are more edges that are logically equivalent in the full graph then there are edges in the nextSet then we need to create a new search sequence
			//if this start edge is already known then don add it
			if(nextEdgeSet.size()<equivalentEdges[repEdge].size()&&knownStartEdges.count(repEdge)==0)
			{
				//mark this edge as a known start edge
				knownStartEdges.insert(repEdge);
				//create the new sequence
				EdgeSequence newSequence;
				newSequence.pushEdgeGroup(equivalentEdges[repEdge]);
				workList.push_front(newSequence);
			}
			
			if(currentSet.size()==nextEdgeSet.size())
			{
				//if all the edges in our edge group have edges in the next group then we can just push edge set onto the
				//sequence, push this partial sequence to the work list, and unset set sequence completed
				sequenceCompleted = false;
				seq.pushEdgeGroup(nextEdgeSet);
				workList.push_front(seq);
			}
			else
			{
				//this sequence is splitting up into smaller sequences (or is done) so we set done and (potentially) add extended sequences to the work List
				if(nextEdgeSet.size()>1)
				{
					sequenceCompleted = false;
					EdgeSequence extSeq(seq);
					extSeq.pushEdgeGroup(nextEdgeSet);
					workList.push_front(extSeq);
				}
			}
		}
		if(sequenceCompleted)
			doneList.push_front(seq);
	}
	
	//Print out the sequences
	for(EdgeSequence seq : doneList)
	{
		std::cout << seq.toStringRepresentation() << std::endl;
	}
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
