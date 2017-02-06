#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <iomanip>
#include <map>
#include <queue>
#include "node.h"

using namespace std;

char alphabet[4] = {'A', 'C', 'G', 'T'};
map < int, Node* > token_map;
vector < string > sq;

string token_track ( Node * token_node, Node * ancestor_node )
{
	string token_sq;
	Node * track = token_node;
	while ( track != ancestor_node && track != NULL )
	{
		token_sq.insert ( 0, track->edge );
		track = track->parent;
	}
	return token_sq;
}

void breadth_first_track ( Node * root, vector < Node * > *bft_nodes )
{
	queue < Node * > q;
	if ( !root ) { return; }
	for ( q.push( root ); !q.empty(); q.pop() )
	{
		Node * temp_node = q.front();
		if ( temp_node->token.size() )
			bft_nodes->push_back ( temp_node );
		for ( unsigned int i = 0; i < temp_node->child.size(); i++ )
			q.push( temp_node->child[i] );
	}
}

void token_request ( Node * root )
{	
	vector < Node * > bft_nodes;
	breadth_first_track ( root, &bft_nodes );
	for ( bft_nodes.back(); !bft_nodes.empty(); bft_nodes.pop_back() )
	{
		Node * T = bft_nodes.back();
		for ( unsigned int i = 0; i < T->token.size(); i++ )
		{
			if ( T->coreNode )
			{
				if ( T->coreNode->token.size() )
				{
					T->token[i] = T->coreNode->token[0];
					T->coreNode->token.erase ( T->coreNode->token.begin() );
				}
				else
				{
					T->token[i] = T->coreNode->subToken();
				}
				string temp = token_track ( token_map[T->token[i]], T->coreNode );
				sq[T->token[i]].insert ( 0, temp );
				token_map[T->token[i]] = T;
			}
		}

	}
}

void root_token ( double z, Node * new_root, Node * root ) 
{	
	int parent_token = floor ( new_root->odds * z );
	int child_token = 0;
	for ( unsigned int i = 0; i < new_root->child.size(); i++ )
		child_token += floor ( new_root->child[i]->odds * z );
	int num_token = parent_token - child_token;
	if ( num_token )
	{
		new_root->token.resize( num_token );
		for ( int i = 0; i < num_token; i++ )
		{
			if ( root->token.size() )
			{
				new_root->token[i] = root->token[0];
				root->token.erase ( root->token.begin() );
			}
			else
			{
				new_root->token[i] = root->subToken();
			}
			string temp = token_track ( token_map[new_root->token[i]], root );
			sq[new_root->token[i]].insert ( 0, temp );
			token_map[new_root->token[i]] = new_root;
		}
	}
}

#if 0
void Node::Compactify( int e )
{
	if ( this->child.size() == 1 )
	{
		if ( this->parent )
		{
			string temp = this->edge;
			temp += this->child[0]->edge;
			this->child[0]->edge = temp;
			this->child[0]->parent = this->parent;
			this->parent->child[e] = this->child[0];
			this->parent->child[e]->Compactify ( e );
		}
	}
	else
	{
		for ( unsigned int i = 0; i < this->child.size(); i++ )
			this->child[i]->Compactify( i );
	}
}
#endif

void print_out ( Node * x )
{
	cout << x << ":\t" << x->edge << " || " << x->odds;
	if ( x->token.size() )
	{
		cout << "\ttoken: ";
		for ( unsigned int i = 0; i < x->token.size(); i++ )
			cout << x->token[i] << " ";
	}
	else
		cout << "\tno token.";
	cout << endl;
	if ( x->child.size() )
	{
		for ( unsigned int i = 0; i < x->child.size(); i++ )
			print_out ( x->child[i] );
	}
}


void weighted_index_building( double ** text, int n, double z, string * sq_return )
{
	Node ** tree;
	tree = new Node * [2];
	tree[0] = new Node;
	tree[0]->parent = NULL;
	tree[0]->odds = 1;
	tree[0]->token.resize ( floor(z) );
	sq.resize ( floor(z) );
	for ( int t = 0; t < floor(z); t++ )
	{
		tree[0]->token[t] = t;
		token_map[t] = tree[0];
	}

	int i = n - 1;
	while ( i>= 0 )
	{
		tree[1] = new Node;
		tree[1]->parent = NULL;
		tree[1]->odds = 1;
		for ( int j = 0; j < 4; j++ )
		{
			if ( text[i][j] >= 1/z )
			{
				Node * branch = new Node;
				branch->Copy ( tree[0] );
				branch->parent = tree[1];
				branch->odds = text[i][j];
				branch->edge.push_back( alphabet[j] );
				tree[1]->child.push_back( branch );
				tree[1]->child.back()->Update( text[i][j], z );
			}
		}
		token_request ( tree[1] );
		root_token( z, tree[1], tree[0] );
		for ( unsigned int t = 0; t < tree[1]->token.size(); t++ )
		{
			int l = ( n- i ) - sq[tree[1]->token[t]].size();
			if ( l > 0 )
				sq[tree[1]->token[t]].insert ( sq[tree[1]->token[t]].begin(), l, 'N' );
		}
		swap ( tree[0], tree[1] );	
		tree[1]->deleteNode();
		i--;
	}

	cout << "finish building" << endl;

#if 1 
	for ( int j = 0; j < z; j++ )
	{
		string temp = token_track(token_map[j], tree[0] );
		sq[j].insert ( 0, temp );
	}
	for ( int j = 0; j < z; j++ )
	{
			if ( sq[j].size() != 0 )
			{
				sq_return->append( sq[j] );
				sq_return->push_back ( '#' );
			}
	}
	sq_return->back() = '$';
#endif
	tree[0]->deleteNode();
	delete[] tree;
}

