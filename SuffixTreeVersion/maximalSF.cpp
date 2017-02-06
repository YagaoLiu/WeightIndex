#include <string>
#include "defs.h"

using namespace std;

int i = 0;
int j = 0;

double lp ( double ** text, int position, char letter )
{
	unsigned int c;
	switch ( letter )
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
			c = 4;
			break;
	}
	if ( c == 4 )
		return 0;
	else
		return text[position][c];
}

void MaximalSolidFactors ( double ** text, string& sq, int N, int n, double z, int * ME )
{
	// compute maximal solid factor at each position in sq array
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
}


