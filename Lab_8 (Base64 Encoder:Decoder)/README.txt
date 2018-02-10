DESCRIPTION:
Create a program to encode and decode files in base64 .txt file. 

USAGE:
-e infilename outfilename
to convert a random file infilename to the base64-encoded .txt file.

-d infilename outfilename
to decode base64-encoded .txt infilename file to the binary outfilename. If there are none-base64 alphabet symbols, then print an abort message. 

-d -i infilename outfilename
to decode base64-encoded .txt infilename file to the binary outfilename. If the -i flag is available, then ignore none-base64 alphabet symbols while decoding. 

-e -f N infilename outfilename
to convert a random infilename file to the base64-encoded .txt file. If the -f N option is available, then put the line-feed symbol every N symbols. 

If the input string is not correct, then show the help message.
Show an abort message if the infilename file is not available or doesn't exist. 

GOALS:
You are to encode a picture, an .mp3 file and a .txt file to the base-64 encoded .txt, and then decode it back with an original name provided and open the file got. 