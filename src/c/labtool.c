// labtool.c: a harmless tool that prints the name it was started under.
#include <stdio.h>

int main(int argc, char **argv) {
    printf("labtool running as: %s\n", argv[0]);
    return 0;
}
