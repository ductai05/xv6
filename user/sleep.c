#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: sleep <number of ticks>\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);
    if (sleep(ticks) < 0) {
        printf("sleep: sleep failed\n");
        exit(1);
    }

    exit(0);
}