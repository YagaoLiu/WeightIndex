Weighted Index: Suffix Tree version
===

<b>Basic Instructions:</b>

No pre-installation is needed for this version.

The shell command 'make' should compile this program.

After compilation, the binary 'index' will be created in the working directory, e.g. you can call it in this directory via 
```
  ./index -t ../exampledata/text.fa -z 8 -o output.txt
```
<b>Usage:</b>
```
Usage: index <options>
Standard (Mandatory):
	-t,	--text		<str>	Filename for the Weighted String.
	-o,	--output	<str>	Filename for Output.
	-z,	--threshold	<dbl>	cumulative weighted threshold.
	-m,	--mod		<int>	0：mod 0 will run without pattern matching, only the final z-string and Maximal solid factor array will be writtern to output file;
							1: mod 1 will run pattern matching after indexing and allow user to input pattern directly. The pattern and occurrences will be writtern to output file;
							2: mod 2 allow user to input a file containing one or more patterns, and run pattern matching for each pattern in the file. The patterns and occurrences will be writtern to output file.
```
