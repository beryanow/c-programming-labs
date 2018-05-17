DESCRIPTION:
Dijkstra's algorithm of finding the shortest path. 

INPUT:
String 1: integer N [0..5000] - number of vertices in the graph. 
String 2: integer S [1..N], integer F [1..N] - path's start and end.
String 3: integer M [0..N*(N-1)/2] - number of edges. 
String [4..M+3]: integer [1..N], integer [1..N], integer [0..INT_MAX] - edge's start, end, length. 

OUTPUT:
If N is not in [0..5000], then print "bad number pf vertices".
If M is not in [0..N*(N+1)/2], then print "bad number of edges".
If number of a vertices is not in [1..N], then print "bad vertex".
If edge's length is not in [0..INT_MAX], then print "bad length".
If number of strings is less than M+3, then print "bad number of lines".

String 1: N values - distances from S to every vertices of the graph. 
Infinity character - if there is no path. 
INT_MAX+ - if the shortest path's length is more than INT_MAX.

String 2: information about the shortest path's (S->F) vertices. 
no path - if there is no path.
overflow - if the shortest path's length is more than INT_MAX and the amount of paths that have lengths that are more than INT_MAX is two or more. 

GOAL: Implementation time - 3 seconds.