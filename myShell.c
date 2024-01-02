// authors : shay finegold 311165609 , dan monsonego 313577595


#include "Function.c"


int main (int argc, char **argv) 
{ 
  char *cmdLine; 
  char **info; 
  printf("---------- Welcome To Our Custom Shell ----------\n"); 
  while(1)
  { 
  			cmdLine = readLine(">>"); 
  			info = parse(cmdLine); 
			  executeCommand(info); 
  } 
}



