#include "node.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void Node::Copy ( Node * x )
{
	this->coreNode = x;
	this->odds = x->odds;
	this->edge = x->edge;
	for ( int i = 0; i < (int) x->child.size(); i++ )
	{
		Node * child = new Node;
		child->Copy ( x->child[i] );
		child->parent = this;
		this->child.push_back( child );
	}
}

void Node::Update( double p, double z )
{
	int parent_token = floor ( this->odds * z );
	int child_token = 0;
	int num_token;
	for ( int i = 0; i < (int) this->child.size(); i++ )
	{
		this->child[i]->odds *= p;
		if ( this->child[i]->odds < 1/z )
		{
			this->child[i]->deleteNode();
			this->child.erase( this->child.begin() + i );
			i--;
		}
		else
		{
			child_token += floor ( z * this->child[i]->odds );
		}
	}
	num_token = parent_token - child_token;
	if ( num_token )
		this->token.resize( num_token );
	for ( unsigned int i = 0; i < this->child.size(); i++ )
		this->child[i]->Update( p, z );
}

int Node::subToken()
{
	int token = -1;
	for ( unsigned int i = 0; i < this->child.size(); i++ )
	{
		if ( this->child[i]->token.size() )
		{
			token = this->child[i]->token[0];
			this->child[i]->token.erase ( this->child[i]->token.begin() );
			return token;
		}
		else
		{
			token = this->child[i]->subToken();
			if ( token != -1 )
				return token;
		}
	}
	return token;
}

void Node::deleteNode()
{
	if ( this->child.size() == 0 )
	{
		delete this;
	}
	else
	{
		while ( this->child.size() )
		{
			this->child.back()->deleteNode();
			this->child.pop_back();
		}
		delete this;
	}
}



