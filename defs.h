#include <string>
#include <list>

#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

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
int * LCParray ( unsigned char * text, int n, int * SA, int * ISA );
void SA_LCP_index ( double ** text, unsigned char * sq, int N, int n, double z, int * SA, int * LCP, int * ME);
int match ( string & pattern, string & text, int n, int * SA, int * LCP, int * ME, list<int> & Occ, rmq_succinct_sct<> &rmq );
int multi_match ( string * pattern, int num_pattern, string & text, int n, int * SA, int * LCP, int * ME, list<int> & Occ );
int lcp ( string & x, int M, string & y, int l );
