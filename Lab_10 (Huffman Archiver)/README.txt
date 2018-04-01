DESCRIPTION:
Create a simple archiver that fulfils Huffman algorithm.  

USAGE:
-c infilename outfilename
to encode a random file infilename to the Huffman-encoded outfilename file.

-d infilename outfilename
to decode the Huffman-encoded infilename file to outfilename of the initial state. 

If the input string is not correct or the infilename is unreachable, then show the help message.

INPUT:
File with random data, containing symbols of [0..255] codes.

OUTPUT:
Encoded or decoded file. 

RESTRICTIONS:
Executable file size and data size are not more than 1024 Kb (each taken separately).