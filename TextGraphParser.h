//
// Created by paulp on 5/11/2019.
//

#ifndef MAGILLACSCHALLENGE_TEXTGRAPHPARSER_H
#define MAGILLACSCHALLENGE_TEXTGRAPHPARSER_H


#include "Graph.h"

class TextGraphParser
{
public:
	static Graph readGraphFromFile(std::string inFile);
	static bool validateGraphAsBinaryTree(Graph &g);
};


#endif //MAGILLACSCHALLENGE_TEXTGRAPHPARSER_H
