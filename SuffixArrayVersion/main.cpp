/**
    Weighted Index
    Copyright (C) 2016 Carl Barton, Tomasz Kociumak, Chang Liu, Solon P. Pissis and Jakub Radoszewski.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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

double ** text;

int main (int argc, char ** argv )
{
	TSwitch sw;
	int mod;
	string text_file;
	string pattern_file;
	string output_file;
	double z;
	
	int n;

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

	cout << "After building, sq length is " << sq.size() << endl;
	//Index using Suffix array
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
	cout<<"Indexing time is:  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;

	srand ( time(NULL) );
	result.open ( output_file );
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
			int m;
			string pattern;
			if ( !read_pattern( pattern_file_name, &pattern, &m ) )
			{
				continue;
			}
			list<int> Occ;
			cout << "Pattern length:" << m << endl;
			int num_Occ = match ( pattern, sq, n, SA, LCP, ME, Occ, rmq );
			if ( num_Occ==0 )
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

#if 1
	while ( true )
	{
		string pattern;
		cout << "Enter pattern, or enter \"exit\" to exit:" << endl;
		cin >> pattern;
		if ( pattern == "exit" )
		{
			cout << "Program exit!" << endl;
			end = get_time::now();
			diff = end - begin;
			return 0;
		}
		else
		{
			int m = pattern.size();
			list<int> Occ;
			cout << "Pattern length:" << m << endl;
			int num_Occ = match ( pattern, sq, n, SA, LCP, ME, Occ, rmq );
			ofstream result;
			result.open( output_file, ios::app );
			result << pattern << ":";
			if ( num_Occ == 0 )
			{
				cout << "No found\n";
				result << "No found\n";
			}
			else
			{

				cout << "Number of occurrences:" << Occ.size() << endl;
				cout << "Positions of occurrences: ";
				for ( auto it = Occ.begin(); it != Occ.end(); it++ )
				{
					cout << *it << ' ';
					result << *it << ' ';
				}
				cout << '\n';
				result << '\n';
				result.close();
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

	delete[] SA;
	delete[] LCP;
	delete[] ME;
	delete[] psq;
	
	for ( int i = 0; i < n; i++ )
		delete[] text[i];
	delete[] text;

	return 0;
}

