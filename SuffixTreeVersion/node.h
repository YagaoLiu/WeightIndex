#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifndef NODE_H
#define NODE_H

struct Node
{
		Node * parent;				//parent node of this node
		Node * coreNode;			//corresponding node of this node
		vector < Node* > child;		//all children nodes of this node
		string edge;				//the edge between this node and its parent
		double odds;				//the probability of this node
		vector < int > token;		//the token given to this node
		void Copy( Node * x );		//deep copy of a node
		void Update( double p, double z );	//update this node when add a new letter in the tree
		int subToken();				//get all tokens in the subtree of this node
		void deleteNode();			//destructor
};

void weighted_index_building( double ** text, int n, double z, string * sq_return );

#endif


