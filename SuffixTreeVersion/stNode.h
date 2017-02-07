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
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

#ifndef stNode_T
#define stNode_T

class stNode
{
	private:
		int start;
		int depth;
		char label;
		unordered_map<char, stNode*> child;
		stNode * parent;
		stNode * slink;
	public:
		set<int> Occ;
		stNode();
		stNode( int i, int d, char c );
		int getStart();
		int getDepth();
		char getLabel();
		stNode * getChild( char c );
		stNode * getParent();
		stNode * getSLink();
		void setSLink( stNode * slinkNode );
		void addChild( stNode * childNode, char c );
		void addLeaf( int i );
		void removeChild( char c );
		int printOcc();
		void setParent ( stNode * parentNode );
		int numChild();
		void printChild();
		stNode** allChild();
		~stNode();
};

#endif
