DESCRIPTION:
Find minimal spanning tree. Kruskal's Algorithm.

INPUT:
String #1: an integer number N [0..5000] - a number of vertices in the graph.
String #2: an integer number M [0..N*(N-1)/2] - a number of edges in the graph.
String #3..M+2: an integer number [1..N], an integer number [1..N], an integer number [0..INT_MAX] - initial, final vertices and length accordingly.

OUTPUT:
If N is not in [0..5000], then print "bad number of vertices".
If M is not in [0..N*(N-1)/2], then print "bad number of edges".
If any number of vertices is not in [1..N], then print "bad vertex".
If any edges' length is not in [0..INT_MAX], then print "bad length".
If there is no spanning tree available, then print "no spanning tree", else print numbers of vertices involved in every edge.