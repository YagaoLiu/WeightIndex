/**
    Weighted Index
    Copyright (C) 2016 Carl Barton, Tomasz Kociumak, Chang Liu, Solon P. Pissis and Jakub Radoszewski.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
		void bfs_Occ( stNode * suffixRoot, set<int> &Occ );

	public:
		suffixTree( string& x );
		stNode * createNode( stNode * u, int d );
		stNode * createBranch( stNode * u, int i, int d );
		stNode * createLeaf( stNode *u, int i );
		stNode * createNode_trim(stNode * u, int d );
		void ComputeSuffixLink( stNode * u );
		int forward_search( string x, set<int> &Occ );
		stNode * forward_search_node( stNode * s, int i, int l );
		void trimST( int * ME, int m );
		~suffixTree();
};
