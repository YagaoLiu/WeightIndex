#include <string>
#include <list>

using namespace std;

#ifndef TSWITCH_H
#define TSWITCH_H

struct TSwitch
{
	double z;
	string text_file_name;
	string pattern_file_name;
	string output_file_name;
};

#endif


int decode_switches ( int argc, char * argv[], TSwitch * sw );
void usage ( void );
int read ( string filename, double *** text, int * n );
int read_pattern ( string filename, string * pattern, int * m );
void MaximalSolidFactors ( double ** text, string& sq, int N, int n, double z, int * ME );
