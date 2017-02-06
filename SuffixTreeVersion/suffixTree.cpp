#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_set>

#include "suffixTree.h"

using namespace std;

suffixTree::suffixTree( string& x )
{
	this->T = &x;
	this->n = x.size();
	
	this->root = new stNode;
	this->root->setSLink( this->root );
	
	stNode * u = this->root;
	stNode * v = NULL;
	stNode * w = NULL;
	int d = 0;
	for ( int i = 0; i < this->n; i++ )
	{
		while ( d == u->getDepth() && u->getChild( x[i+d] ) != NULL )
		{
			u = u->getChild( x[i+d] );
//			u->addOcc( i );
			d = d+1;
			while (	d < u->getDepth() && x[u->getStart()+d] == x[i+d] )
				d = d + 1;
		}
		if ( d < u->getDepth() )
		{
			v = createNode ( u, d );
			u = v;
		}
		w = createBranch ( u, i, d );
		
		if ( u->getSLink() == NULL )
		{
			ComputeSuffixLink( u );
		}
		u = u->getSLink();
		d = max( d-1, 0 );
	}
}

void suffixTree::bfs_leaves()
{
	stNode * u = NULL;
	stNode * v = NULL;
	stack<stNode*> s;
	u = this->root;
	s.push( u );
	while( !s.empty() )
	{
		v = s.top();
		s.pop();
		if ( v->numChild() == 0 )
		{
			pair<int, stNode*> leaf ( v->getStart(), v );
			this->leaves.insert( leaf );
		}
		else
		{
			stNode ** children = v->allChild();
			for ( int i = 0; i < v->numChild(); i++ )
				s.push( children[i] );
			delete[] children;
		}
	}
}

void suffixTree::bfs_Occ( stNode * suffixRoot, vector<int> &Occur )
{
	stNode * u = suffixRoot;
	stNode * v = NULL;
	stack<stNode*> s;
	s.push( u );
	while( !s.empty() )
	{
		v = s.top();
		s.pop();
		if( v->numChild() == 0 )
		{
			for( auto it = v->Occ.begin(); it != v->Occ.end(); it++ )
			{
				Occur.push_back( *it );
			}
		}
		else
		{
			stNode ** children = v->allChild();
			for ( int i = 0; i < v->numChild(); i++ )
				s.push( children[i] );
			delete[] children;
		}
	}
}

suffixTree::~suffixTree()	
{
}

void suffixTree::ComputeSuffixLink( stNode * u )
{
	int d = u->getDepth();
	stNode * v = u->getParent()->getSLink();
	while ( v->getDepth() < d-1 )
	{	
		v = v->getChild( this->T->at( u->getStart() + v->getDepth() + 1 ) );
	}
	if ( v->getDepth() > d-1 )
	{
		v = createNode ( v, d-1 );
		v->setSLink ( NULL );
	}
	u->setSLink ( v );
}

stNode * suffixTree::createNode( stNode * u, int d )
{
	int i = u->getStart();
	stNode * p = u->getParent();
	stNode * v = new stNode( i, d, this->T->at( i+p->getDepth() ) );
	v->addChild( u, this->T->at( i+d ) );
	p->addChild( v, this->T->at( i+p->getDepth() ) );
	return v;
}	

stNode * suffixTree::createNode_trim( stNode * u, int d )
{
	int i = u->getStart();
	stNode * p = u->getParent();
	stNode * v = new stNode( i, d, this->T->at( i+p->getDepth() ) );
	v->addChild( u, this->T->at( i+d ) );
	p->addChild( v, this->T->at( i+p->getDepth() ) );
	return v;
}	

stNode * suffixTree::createBranch( stNode * u, int i, int d )
{
	stNode * w = new stNode(i, this->n-i, this->T->at( i+d ) ) ;
	u->addChild( w, this->T->at( i+d ) );
//	w->setParent( u, this->T->at( i+d ) );
	return w;
}

int suffixTree::forward_search( string x, vector<int> &Occur )
{
	int m = x.size();
	int d = 0;
	stNode * u = this->root;
	stNode * v = NULL;
	bool match = true;
	while ( match )
	{
		v = u->getChild( x[d] );
		if ( v == NULL )
		{
			match = false;
			return 0;
		}
		int i = u->getDepth();
		for ( i ; i < m && i < v->getDepth(); i++ )
		{
			if ( x[i] != this->T->at( v->getStart()+i ) )
			{
				match = false;
				return 0;
			}
		}
		if( m <= v->getDepth() )
		{
			bfs_Occ( v, Occur );
			return 1;
		}
		else
		{
			d = v->getDepth();
			u = v;
		}
	}
	return 0;
}

stNode * suffixTree::forward_search_node( stNode * s, int i, int l )
{
	stNode * u = s;
	int start = s->getStart();
	int position = i + u->getDepth();
	int extent = l - u->getDepth();
	while ( extent > 0 )
	{
		u = u->getChild( this->T->at( position ) );
		extent = l - u->getDepth();
		position = i + u->getDepth();
	}
	return u;	
}

void suffixTree::trimST( int * ME, int m )
{
	bfs_leaves();
	stNode * u = this->root;
	int node_move_type = 0;
	map <int, stNode*> newLeaves;
	for ( int i = 0; i < this->n; i++ )
	{
		u = forward_search_node( u, i, ME[i] );
		if ( ME[i] < u->getDepth() )
		{
			u = createNode( u, ME[i] );
			u->addLeaf( i%m );
			pair<int, stNode*> newLeaf (i, u->getChild('$'));
			newLeaves.insert( newLeaf );
			u = u->getParent()->getSLink();
		}
		else
		{
			u->addLeaf( i%m );
			pair<int, stNode*> newLeaf (i, u->getChild('$'));
			newLeaves.insert( newLeaf );
			while ( u->getSLink() == NULL )
			{
				u = u->getParent();
			}
			u = u->getSLink();
		}
	}
#if 1
	for ( auto it = this->leaves.begin(); it != this->leaves.end(); it++ )
	{
		u = it->second;

		if ( u != newLeaves[it->first] && u->Occ.empty() )
		{
			char label_remove = u->getLabel();
			u = u->getParent();
			u->removeChild( label_remove );
			while ( u->numChild() == 0 )
			{
				label_remove = u->getLabel();
				u = u->getParent();
				u->removeChild( label_remove );
			}
		}
	}
#endif
}

