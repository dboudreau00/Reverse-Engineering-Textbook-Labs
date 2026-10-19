#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    const char *secret = "open-sesame";
    if (argc > 1 && strcmp(argv[1], secret) == 0) {
        puts("Access granted");
        return 0;
    }
    puts("Access denied");
    return 1;
}
