#include <string>
#include <map>

#include "stNode.h"

using namespace std;

class suffixTree
{
	private:
		stNode * root;
		string * T;
		int n;
		int num;
		map<int, stNode*> leaves;
		void bfs_leaves();
		void bfs_Occ( stNode * suffixRoot, vector<int> &Occ );

	public:
		suffixTree( string& x );
		stNode * createNode( stNode * u, int d );
		stNode * createBranch( stNode * u, int i, int d );
		stNode * createLeaf( stNode *u, int i );
		stNode * createNode_trim(stNode * u, int d );
		void ComputeSuffixLink( stNode * u );
		void breadth_first_traversal( stNode * root, vector<stNode*> &leaves);
		int forward_search( string x, vector<int> &Occ );
		stNode * forward_search_node( stNode * s, int i, int l );
		void trimST( int * ME, int m );
		~suffixTree();
};
