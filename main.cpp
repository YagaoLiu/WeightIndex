#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <cstdlib>
#include <ctime>

#include "node.h"
#include "defs.h"

#include <divsufsort.h>
#include <sdsl/rmq_support.hpp>

using namespace std;
using get_time = chrono::steady_clock;

int main (int argc, char ** argv )
{
	TSwitch sw;
	int mod;
	string text_file;
	string pattern_file;
	string output_file;
	double z;
	
	int n;
	double ** text;

	ofstream result;

	unsigned int k;

	/* Decodes the arguments */

	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 5 )
	{
		usage();
		return 1;
	}
	else
	{
		if ( sw.text_file_name.empty() )
		{
			cout << "Error: No Text input!\n";
			return 0;
		}
		else
	{
			text_file = sw.text_file_name;
		}
		

/*
		if ( sw.pattern_file_name.empty() )
		{
			cout << "Error: No pattern input!\n";
			return 0;
		}
		else
		{
			pattern_file = sw.pattern_file_name;
		}
*/
		if ( sw.output_file_name.empty() )
		{
			cout << "Error: No Output file\n";
			return 0;
		}
		else
		{
			output_file = sw.output_file_name;
		}

		if ( sw.z > 0 )
		{
			z = sw.z;
		}
		else
		{
			cout << "Error: z must be a positive number!\n";
		}
	}

	/* read input */
	if ( !read ( text_file, &text, &n ) )
	{
		cout << "Error: Failed to read input!\n";
		return 0;
	}
	cout << "text length:" << n << endl;
	cout << "Finish reading input" << endl;

	auto begin = get_time::now();
	cout << "index begin" << endl;

	string sq;
	weighted_index_building ( text, n, z, &sq );

	cout << "After indexing, sq length is " << sq.size() << endl;

	int N = sq.size();

	int * SA	= new int [N];
	int * LCP	= new int [N];
	int * ME	= new int [N];
	unsigned char * psq	= new unsigned char [N+1];
	strcpy ( ( char * )psq, sq.c_str() );

	SA_LCP_index ( text, psq, N, n, z, SA, LCP, ME );

	vector < int > v ( N, 0 );
	for ( int i = 0; i < N; i++ )
		v[i] = LCP[i];
	rmq_succinct_sct<> rmq(&v);

	auto end = get_time::now();
	auto diff = end - begin;
	cout<<"Indexing Elapsed time is :  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;

	srand ( time(NULL) );
	result.open ( output_file );
	ifstream input_pattern ( "pattern.fa" );
#if 1
	string pattern;
//	cout << "Enter pattern, or enter \"exit\" to exit:" << endl;
//	cin >> pattern;
	pattern = "CTCACGAT";
	if ( pattern == "exit" )
	{
		cout << "Program exit!" << endl;
		return 0;
	}
	else
	{
		int m = pattern.size();
		list<int> Occ;
		cout << "Pattern length:" << m << endl;
		int num_Occ = match ( pattern, sq, n, SA, LCP, ME, Occ, rmq );
		if ( num_Occ==0 )
			cout << "No found\n";
		else
		{
			for ( auto it = Occ.begin(); it != Occ.end(); it++ )
				cout << *it << ' ';
			cout << '\n';
		}
		Occ.clear();

	}
#endif

#if 0
	int times = 0;
	while ( true )	
	{

		string pattern;
		int m;
		list<int> Occ;
		input_pattern >> pattern;
		if ( input_pattern.eof() ) break;
		result << pattern << ":";

		//			auto search_begin = get_time::now();

		int num_Occ = match ( pattern, sq, n, SA, LCP, ME, Occ, rmq );
		if ( !num_Occ )
		{
			result << "No found\n";
		}
		else
		{
			for ( auto it = Occ.begin(); it != Occ.end(); it++ )
				result << *it << ' ';
			result << '\n';
		}
		Occ.clear();

		times ++;
	}
#endif
	end = get_time::now();
	diff = end - begin;
	cout<<"Total Elapsed time is :  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;

	delete[] SA;
	delete[] LCP;
	delete[] ME;
	delete[] psq;
	for ( int i = 0; i < n; i++ )
		delete[] text[i];
	delete[] text;

	return 0;
}

