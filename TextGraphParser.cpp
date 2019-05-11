//
// Created by paulp on 5/11/2019.
//

#include <regex>
#include <list>
#include <fstream>
#include <iostream>
#include "TextGraphParser.h"

struct LineRep
{
	std::string name;
	int label;
	std::list<std::string> children;
};

Graph TextGraphParser::readGraphFromFile(std::string inFile)
{
	Graph g;
	//store all the elements read from the file
	std::list<LineRep> lines;
	std::ifstream inStream(inFile);
	std::regex lineFormat("\\s*([a-zA-Z0-9]+)\\s*\\(([0-9]+)\\)\\s*:\\s*(([a-zA-Z0-9]+)(\\s*,\\s*([a-zA-Z0-9]+))*)?\\s*");
	std::regex nameFormat("[a-zA-Z0-9]+");
	
	if(inStream.is_open())
	{
		int lineNo = 0;
		while (true)
		{
			std::string currentLine;
			std::getline(inStream,currentLine);
			lineNo++;
			if(currentLine.empty())break;
			//ignore comments
			if (currentLine[0]=='#') continue;
			std::smatch sm;
			//if we match then add it to the list else report an error
			if(std::regex_match(currentLine, sm, lineFormat))
			{
				//read the line's contents
				std::string fVertexName = sm[1];
				std::string labelStr = sm[2];
				//gets the full children string
				std::string childrenString = sm[3];
				std::list<std::string> children;
				//extracts the children from the string
				std::smatch cm;
				while (std::regex_search(childrenString,cm,nameFormat))
				{
					children.push_back(cm[0]);
					childrenString = cm.suffix();
				}
				
				int label;
				try
				{
					label = std::stoi(labelStr);
				}
				catch (std::invalid_argument &e)
				{
					std::cerr << "Invalid input on line " << lineNo << ".  Vertex can't have id of " << label << "." << std::endl;
					continue;
				}
				lines.push_back({.name=fVertexName,.label=label,.children=children});
			}
			else
			{
				std::cerr << "Invalid input on line " << lineNo << "." << std::endl;
			}
		}
	}
	else
	{
		throw std::runtime_error("Could not open file \'" + inFile + "\'");
	}
	inStream.close();
	//done reading user input
	//now we need a mapping between the user names and actual vertices in the graph
	std::map<std::string, Vertex*> vertexNameMapping;
	for (LineRep line : lines)
	{
		//if we have not already added this to the grapah then do so now
		//on this pass we just add vertices on the next once all the vertices are in we add the edges
		if(vertexNameMapping.count(line.name)==0)
		{
			vertexNameMapping[line.name] = &g.createVertex(line.label);
		}
	}
	//now we add the edges on this pass
	for (LineRep line : lines)
	{
		Vertex *src = vertexNameMapping[line.name];
		for(std::string childName : line.children)
		{
			Vertex *dst = vertexNameMapping[childName];
			g.addEdge(*src,*dst);
		}
	}
	//done
	
	return g;
}

bool dfsTreeValidator(Vertex &vtx, std::map<Vertex*,bool> &marked)
{
	//if we are in a node that has already been marked then we have found a cycle, so we return false
	if(marked[&vtx])
		return false;
	//mark it
	marked[&vtx] = true;
	
	std::set<Vertex*> children = vtx.getChildren();
	//a binary tree must only have two children
	if(children.size()>2) return false;
	
	bool result = true;
	for (Vertex* child : children)
	{
		result &= dfsTreeValidator(*child, marked);
	}
	return result;
}

bool TextGraphParser::validateGraphAsBinaryTree(Graph &g)
{
	std::set<Vertex*> heads = g.getHeads();
	std::map<Vertex*,bool> marked;
	//a tree has only one head
	if(heads.size()!=1) return false;
	Vertex &root = **heads.begin();
	
	return dfsTreeValidator(root, marked);
}



