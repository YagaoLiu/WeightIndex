#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifndef NODE_H
#define NODE_H

struct Node
{
		Node * parent;
		Node * coreNode;
		vector < Node* > child;
		string edge;
		double odds;
		vector < int > token;
		void Copy( Node * x );
		void Update( double p, double z );
		void Print();
		void fPrint(ofstream& result);
		void rootToken( double z, Node *root );
		void Compactify( int e );
		int subToken();
		void deleteNode();
};

void weighted_index_building( double ** text, int n, double z, string * sq_return );

#endif


