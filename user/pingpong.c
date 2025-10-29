#include "kernel/types.h"
#include "user/user.h"
int main()
{
  int p2c[2], c2p[2]; // [0] - read, [1] - write
  char buf[1];

  pipe(p2c);
  pipe(c2p);

  if (fork() == 0){
    // child process
    close(p2c[1]); // close p2c write
    close(c2p[0]); // close c2p read

    read(p2c[0], buf, 1); // wait for a byte from parent
    printf("%d: received ping\n", getpid());
    write(c2p[1], buf, 1); // send back a byte to parent

    close(p2c[0]);
    close(c2p[1]);

    exit(0);
  } else {
    // parent process
    close(p2c[0]); // close p2c read
    close(c2p[1]); // close c2p write

    buf[0] = 'X';
    write(p2c[1], buf, 1); // send a byte to child

    read(c2p[0], buf, 1); // wait for a byte from child
    printf("%d: received pong\n", getpid());
    wait(0); 

    close(p2c[1]); 
    close(c2p[0]);
    exit(0);
  }
}