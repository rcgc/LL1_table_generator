# LL1_table_generator
## Abstract
Predictive parsers, that is, recursive-descent parsers needing no backtracking, can be constructed for a class of grammars called LL(1). The first "L" in LL(1) stands for scanning the input from left to rigth, the second "L" for producing a leftmost derivation, and the "1" for using one input symbol of lookahead at each step to make parsing action decisions[1].

The class of LL(1) grammars is rich enough to cover most programming constructs, although care is needed in writing a suitable grammar for the source language. For example, no left-recursive or ambiguous grammar can be LL(1)[1].

## How to use it
Execute linux commands in the following order:
- `g++ generador_tablas_LL1_linux.cpp` : compiles .cpp file
- `./a.out` : runs the compiled file
- paste input and hit enter
- If the given grammar is LL(1) report.html will be generated, otherwise not and console will display a warning message
- Open report.html (if generated) in preferred browser

## LL(1) Table Generator
### Input
5 4 <br>
goal -> A <br>
A -> ( A ) <br>
A -> two <br>
two -> a <br>
two -> b <br>
( ( a ) ) <br>
( a ) ) <br>
( ( ( ( ( b ) ) ) ) ) <br>
( ( ( ( ( a b ) ) ) ) ) <br>

### Result
![screenshot.png](https://github.com/rcgc/LL1_table_generator/blob/master/screenshot.png)
<p><b>Image 1. </b>Input grammar is LL(1)</p><br>

![screenshot.png](https://github.com/rcgc/LL1_table_generator/blob/master/screenshot_NoLL1.png)
<p><b>Image 2. </b>Input grammar is no LL(1)</p><br>


## Terminals and Nonterminals Generator
In order to get the exact output uncomment lines 1345 and 1346 from generador_tablas_LL1_linux.cpp.
### Input
5 4 <br>
goal -> A <br>
A -> ( A ) <br>
A -> two <br>
two -> a <br>
two -> b <br>
( ( a ) ) <br>
( a ) ) <br>
( ( ( ( ( b ) ) ) ) ) <br>
( ( ( ( ( a b ) ) ) ) ) <br>

### Result
![screenshot.png](https://github.com/rcgc/LL1_table_generator/blob/master/screenshot_firsts_follows.png)
<p><b>Image 3. </b>Input grammar Firsts and Follows</p><br>

## Firsts and Follows Generator
In order to get the exact output uncomment lines 1342, 1343 and 1344 from generador_tablas_LL1_linux.cpp.
### Input
5 4 <br>
goal -> A <br>
A -> ( A ) <br>
A -> two <br>
two -> a <br>
two -> b <br>
( ( a ) ) <br>
( a ) ) <br>
( ( ( ( ( b ) ) ) ) ) <br>
( ( ( ( ( a b ) ) ) ) ) <br>

### Result
![screenshot.png](https://github.com/rcgc/LL1_table_generator/blob/master/screenshot_terminals_nonterminals.png)
<p><b>Image 4. </b>Input grammar Terminals and Nonterminals</p><br>

## References
[1]Aho, A., 2007. Compilers. 2nd ed. Boston: Pearson/Addison Wesley.
