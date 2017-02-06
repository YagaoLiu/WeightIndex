#include <map>
#include <iostream>
#include <string>

#include "defs.h"

#include <divsufsort.h>
#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

int i = 0;
int j = 0;

double lp ( double ** text, int position, char letter )
{
	unsigned int c;
	switch(letter)
	{
		case 'A':
			c = 0;
			break;
		case 'C':
			c = 1;
			break;
		case 'G':
			c = 2;
			break;
		case 'T':
			c = 3;
			break;
		default:
			c = 5;
			break;
	}
	if ( c == 5 )
		return 0;
	else
	{
		return text[position][c];
	}
}

int minimal ( int a, int b, int c )
{
	if ( a<=b && a<= c )
		return a;
	if ( b<=a && b<= c )
		return b;
	else
		return c;
}

void pa ( double * a, int n )
{
	for ( int i = 0; i < n; i++ )
		cout << a[i] << ' ';
	cout << endl;
}

void SA_LCP_index ( double ** text, unsigned char * sq, int N, int n, double z, int * SA, int * LCP, int * ME )
{
	// compute valid SA using slide window:
	double p = 1.0;
	int valid_length = 0;
	int k = 0;

	for ( j = 0; j < z; j++ )
	{
		for ( i = 0; i < n+1; i++ )
		{
			if ( sq[i+j*(n+1)] == '$' || sq[i+j*(n+1)] == 'N' )
				ME[i+j*(n+1)] = 0;
			else if ( ME[i+j*(n+1)-1] <= 1 )
			{
				k = 0;
				p = 1.0;
				for ( k = 0; k < n; k++ )
				{
					if ( p * lp( text, i+k, sq[i+j*(n+1)+k] ) >= 1/z )
						p *= lp( text, i+k, sq[i+k+j*(n+1)]);
					else
						break;
				}
				ME[i+j*(n+1)] = k;
			}
			else
			{
				k = ME[i+j*(n+1)-1];
				p /= lp ( text, i-1, sq[i-1+j*(n+1)] );
				if ( p < 1/z )
					ME[i+j*(n+1)] = k -1;
				else
				{
					for ( k = k-1; k < n; k++ )
					{
						if ( p * lp( text, i+k, sq[i+k+j*(n+1)] ) >= 1/z )
							p *= lp( text, i+k, sq[i+k+j*(n+1)] );
						else
							break;
					}
				}
				ME[i+j*(n+1)] = k;
			}
		}
	}

	// compute SA:
	if ( divsufsort ( sq, SA, N ) != 0 )
	{
		fprintf ( stderr, " Error: Cannot allocate memory.\n" );
		exit ( EXIT_FAILURE );
	}

	int * iSA = new int [N];
	// compute inverseSA:
	for ( i = 0; i < N; i++ )
	{
		iSA[SA[i]] = i;
	}
	
	// compute LCP:
	LCP[0] = 0;
	i = 0;
	j = 0;
	for ( i = 0; i < N; i++ )
	{
		if ( iSA[i] != 0 )
		{
			if ( i == 0 )
				j = 0;
			else
				j = ( LCP[iSA[i-1]] >= 2 ) ? LCP[iSA[i-1]]-1 : 0;
			while ( sq[i+j] == sq[SA[iSA[i]-1]+j] )
				j++;
			LCP[iSA[i]] = j;
		}
	}

#if 0
	for ( i = 1; i < N; i++ )
	{
		LCP[i] = minimal ( LCP[i], ME[i-1], ME[i] );
	}
#endif

	delete[] iSA;
}


				



