DESCRIPTION:
Topological graph sorting.

INPUT:
String #1: an integer N [0..1000] - number of vertices. 
String #2: an integer M [0..N * (N + 1) / 2] - number of edges.
String #3..M+2: two integers - edge's start and end.

OUTPUT:
If N's not in [0..1000], print "bad number of vertices".
If M's not in [0..N*(N+1)/2], print "bad number of edges".
If edge's number is not in [1..N], print "bad vertex".
If overall strings' amount is less than M + 2, print "bad number of lines".

If it's possible to implement topological sorting, print graph's vertices.
Otherwise, print "impossible to sort". 
