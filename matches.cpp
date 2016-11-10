#include <iostream>
#include <string>
#include <cmath>
#include <list>

#include "defs.h"

#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

int multi_match ( string * pattern, int num_pattern, string & text, int n, int * SA, int * LCP, int * ME, list<int> & Occ )
{
	int N = text.size();
	// RMQ
	vector<int> v ( N, 0 );
	for ( int i = 0; i < N; i++ )
		v[i] = LCP[i];
	rmq_succinct_sct<> rmq ( &v );
	
	for ( int np = 0; np < num_pattern; np++ )
	{
	int P = pattern[np].size();
	int L, R, M;
	int l, r, m;
	int num_Occ = 0;
	int found_flag = 0;
	
	// LCP Match
	l = lcp ( text, SA[0], pattern[np], 0 );
	r = lcp ( text, SA[N-1], pattern[np], 0 );

#if	1 
	if ( l == P )
	{
		if ( ME[SA[0]] >= P )
		{
			Occ.push_back( SA[0] );
		}
		return Occ.size();
	}
	else
	{
		L = 0;
		R = N-1;
		while ( R-L > 1 )
		{
			M = (L+R) / 2 ;
			if ( l >= r )
			{
				if ( LCP[rmq( L+1, M )] >= l )
					m = l + lcp ( text, SA[M]+l, pattern[np], l );
				else
					m = LCP[rmq( L+1, M )];
			}
			else
			{
				if ( LCP[rmq( M+1, R )] >= r )
					m = r + lcp ( text, SA[M] + r, pattern[np], r );
				else
					m = LCP[rmq( M+1, R )];
			}
			if ( m == P )
			{
				found_flag = 1;
				int E = M;
				while ( L+1 < E )
				{
					int J = (L+E)/2;
					if ( LCP[rmq( J+1, E )] < P )
						L = J;
					else
						E = J;
				}
				if ( LCP[rmq( L+1, E )] >= P )
					L = max ( L, 0 );
				else
					L = L+1;
				E = M;
				while( E + 1 < R )
				{
					int J = (E+R)/2;
					if ( LCP[rmq (E+1, J)] < P )
						R = J;
					else
						E = J;
				}
				if ( LCP[rmq (E+1,R)] >= P )
					R = min ( R, N-1 );
				else
					R = R-1;
				break;
			}				
			else if	( pattern[np][m] <= text[SA[M]+m] ) 
			{
				R = M;
				r = m;
			}
			else
			{
				L = M;
				l = m;
			}
		}
	}
	if ( found_flag )
	{
		for ( int i = L; i <= R; i++ )
		{
			if ( ME[SA[i]] >= P )
			{
				Occ.push_back ( SA[i]%(n+1) );
			}
		}
		Occ.sort();
		Occ.unique();
	}
#endif
	}
	return Occ.size();
}




	
