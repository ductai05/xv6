#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, const char *target_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }
  
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
      continue;
    }
    
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(strcmp(de.name, target_name) == 0){
        printf("%s\n", buf);
    }
    
    struct stat st2;
    if(stat(buf, &st2) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    if(st2.type == T_DIR){
      find(buf, target_name);
    }
  }

  close(fd);
}


int main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "Usage: find <directory> <filename>\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}