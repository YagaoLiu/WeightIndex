Weighted Index
===

<b> Description: </b> 
SuffixTree version does pattern matching with suffix tree of sq.
SuffixArray version does pattern matching using SuffixArray, Longest Common Prefix Array and RMQ to get the occurrence with fault positive, and then check them with Maximal Solid Factor Array of the weighted string.

<b>Installation</b>
SuffixTreeVersion: Directly run shell command 'make' to compile this program.
SuffixArrayVersion: Follow 'INSTALL' file in the folder.

<b>Usage</b>

```
Usage: index <options>
Standard (Mandatory):
	-t,	--text		<str>	Filename for the Weighted String.
	-o,	--output	<str>	Filename for Output.
	-z,	--threshold	<dbl>	cumulative weighted threshold.
```
