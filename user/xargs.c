#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLINE 512  // Max length of an input line

int readline(char *line, int maxlen)
{
  int i = 0;
  char c;
  
  while(i < maxlen - 1){
    // Read 1 byte from stdin (file descriptor 0)
    if(read(0, &c, 1) != 1)
      break;  // EOF or ERROR
    
    if(c == '\n')
      break;
    
    line[i++] = c;
  }
  
  line[i] = '\0';
  return i;
}

int main(int argc, char *argv[])
{
  char line[MAXLINE];     // input line
  char *xargv[MAXARG];    // arguments -> exec
  int i, n;
  
  // check input arguments
  if(argc < 2){
    fprintf(2, "Usage: xargs <command> [args...]\n");
    exit(1);
  }

  // argv[0] : "xargs" => skip this
  // argv[1] : <command>
  // argv[2]... : <static arguments>

  // get <static arguments> from command line
  for(i = 1; i < argc; i++){
    xargv[i-1] = argv[i];
  }
  
  while((n = readline(line, MAXLINE)) > 0){
    
    xargv[argc-1] = line;   // add line as the last argument 
    xargv[argc] = 0;        // NULL terminate (for exec)
    
    int pid = fork();
    
    if(pid < 0){
      fprintf(2, "xargs: fork failed\n");
      exit(1);
    }
    
    if(pid == 0){
      // xargv[0]: command name; xargv: array of arguments
      exec(xargv[0], xargv);
      
      fprintf(2, "xargs: exec %s failed\n", xargv[0]);
      exit(1);
    } else {
      wait(0);
    }
  }
  
  exit(0);
}