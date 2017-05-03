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
#include <set>

#include "node.h"
#include "defs.h"
#include "suffixTree.h"

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

	unsigned int k;

	/* Decodes the arguments */

	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 7 )
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
			return 0;
		}

		if ( sw.mod < 0 || sw.mod > 2 )
		{
			cout << "Error: Wrong mod select!\n";
			return 0;
		}
		else
		{
			mod = sw.mod;
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

	auto end = get_time::now();
	auto diff = end - begin;
	cout << "time:" << chrono::duration_cast<chrono::milliseconds>(diff).count() << " ms "<< endl;
	//build trimmed suffix tree of sq
	cout << "build suffix tree" << endl;
	suffixTree ST(sq);
	cout << "trim suffix tree " << endl;
	ST.trimST(ME, n+1);
#if 1 
	ofstream result ( output_file );
	switch ( mod )
	{
		case 0:
			{
				end = get_time::now();
				diff = end - begin;
	cout << "time:" << chrono::duration_cast<chrono::milliseconds>(diff).count() << " ms "<< endl;
			cout << "The final z-sting and Maximal solid factor array will output to " << output_file << endl;
//			result << sq << endl;
//			for ( auto it = 0; it < N; it++ )
//				result << ME[it] << ' ';
//			result << endl;
			return 0;
			}
		case 1:
			while ( true )
			{
				string pattern;
				cout << "Enter pattern, or enter \"exit\" to exit:" << endl;
				cin >> pattern;
				if ( pattern == "exit" )
				{
					cout << "Program exit!" << endl;
				//	auto end = get_time::now();
				//	auto diff = end - begin;
				//	cout<<"Total Elapsed time is :  "<< chrono::duration_cast<chrono::milliseconds>(diff).count()<<" ms "<<endl;
					return 0;
				}
				else
				{
					result << pattern << ":\t";
					int m = pattern.size();
					set<int> Occ;
					cout << "Pattern length:" << m << endl;
					int occ = ST.forward_search( pattern, Occ );
					if ( occ==0 )
					{
						cout << "No found\n";
						result << "No found\n";
					}
					else
					{
						cout << "Number of occurrences:" << Occ.size() << endl;
						for ( auto it = Occ.begin(); it != Occ.end(); it++ )
						{
							cout << *it << ' ';
							result << *it << ' ';
						}
						cout << '\n';
						result << '\n';
					}
					Occ.clear();
				}
			}
			return 0;
		
		case 2:
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
					ifstream input_pattern ( pattern_file_name );
					while ( true )
					{
						string pattern;
						set<int> Occ;
						input_pattern >> pattern;
						int m = pattern.size();
						if ( input_pattern.eof() ) break;
						result << pattern << ":\t";
						cout << pattern << ":\t";
						int occ = ST.forward_search( pattern, Occ );
						if ( occ == 0 )
						{
							cout << "No found\n";
							result << "No found\n";
						}
						else
						{
							for ( auto it = Occ.begin(); it != Occ.end(); it++ )
							{
								cout << *it << ' ';
								result << *it << ' ';
							}
							cout << '\n';
							result << '\n';
						}
						Occ.clear();
					}
				}
			}
			return 0;
	}
#endif

	delete ME;
	for ( int i = 0; i < n; i++ )
		delete[] text[i];
	delete[] text;

	return 0;
}

