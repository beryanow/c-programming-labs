DESCRIPTION:
Print the report of Rabin-Karp's searching for the substring in the string algorithm.

INPUT:
String #1: a pattern of 1 to 16 symbols.
The pattern can contain any symbol except the one of the string's end.
String #2: an arbitrary amount of symbols (text).

OUTPUT:
The report of Rabin-Karp's algorithm for the hash-function h(c[0],...,c[N-1])=sum((c[i]%3)*(3^i)): hash-function's value and every symbol's position in the text, that was ever compared with a symbol from the pattern must be printed.
Positions are counted from 1 in the text. The algorithm searches for all pattern's entries to the text.

