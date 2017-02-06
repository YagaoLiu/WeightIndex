#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <cstdlib>
#include <ctime>

#include "node.h"
#include "defs.h"
#include "suffixTree.h"

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

	int N = sq.size();
	int * ME = new int [N];
	MaximalSolidFactors ( text, sq, N, n, z, ME );

	for ( auto i = 0; i < N; i++ )
		cout << ME[i] << ' ';
	cout << endl;

	cout << "After building, sq length is " << sq.size() << endl;

	//build trimmed suffix tree of sq
	suffixTree ST(sq);
	ST.trimST(ME, n+1);


#if 0 
	while ( true )
	{
		string pattern_file_name;
		cout << "Enter pattern file name, or enter \"exit\" to exit:" << endl;
		cin >> pattern_file_name;
		if ( pattern_file_name == "exit" )
		{
			cout << "Program exit!" << endl;
			return 0;
		}
		else
		{
			begin = get_time::now();
			int m;
			string pattern;
			if ( !read_pattern( pattern_file_name, &pattern, &m ) )
			{
				continue;
			}
			list<int> Occ;
			cout << "Pattern length:" << m << endl;
			int num_Occ = match ( pattern, sq, n, SA, LCP, ME, Occ, rmq );
			end = get_time::now();
			diff = end - begin;
			if ( num_Occ==0 )
				cout << "No found\n";
			else
			{
				cout << "Number of occurrences:" << Occ.size() << endl;
				cout << "Positions of occurrences: ";
				for ( auto it = Occ.begin(); it != Occ.end(); it++ )
					cout << *it << ' ';
				cout << '\n';
				cout << "Searching time: " << chrono::duration_cast<chrono::milliseconds>(diff).count() << " ms" << endl;
			}
			Occ.clear();
		}
	}
#endif

#if 1 
	while ( true )
	{
		string pattern;
		cout << "Enter pattern, or enter \"exit\" to exit:" << endl;
		cin >> pattern;
		if ( pattern == "exit" )
		{
			cout << "Program exit!" << endl;
			auto end = get_time::now();
			auto diff = end - begin;
			cout<<"Total Elapsed time is :  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;
			return 0;
		}
		else
		{
			int m = pattern.size();
			vector<int> Occ;
			cout << "Pattern length:" << m << endl;
			int occ = ST.forward_search( pattern, Occ );
			if ( occ==0 )
				cout << "No found\n";
			else
			{
				cout << "Number of occurrences:" << Occ.size() << endl;
				cout << "Positions of occurrences: ";
				for ( auto it = Occ.begin(); it != Occ.end(); it++ )
					cout << *it << ' ';
				cout << '\n';
			}
			Occ.clear();
		}
	}
#endif

#if 0 
	ifstream input_pattern ( "pattern.fa" );
	int times = 0;
	while ( true )	
	{
		string pattern;
		int m;
		list<int> Occ;
		input_pattern >> pattern;
		if ( input_pattern.eof() ) break;
		result << pattern << ":";

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
//	end = get_time::now();
//	diff = end - begin;
//	cout<<"Total Elapsed time is :  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;

	for ( int i = 0; i < n; i++ )
		delete[] text[i];
	delete[] text;

	return 0;
}

