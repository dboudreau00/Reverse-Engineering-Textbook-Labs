// killswitch.c: the WannaCry kill-switch pattern, with no payload.
#include <stdio.h>
#include <windows.h>
#include <wininet.h>

int main(void) {
    HINTERNET net = InternetOpenA("Mozilla/4.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET url = InternetOpenUrlA(net, "http://killswitch.example.test/", NULL, 0,
                                     INTERNET_FLAG_RELOAD, 0);
    if (url != NULL) {                      // the domain answered: stand down
        InternetCloseHandle(url);
        InternetCloseHandle(net);
        puts("Kill switch answered: exiting.");
        return 0;
    }
    InternetCloseHandle(net);
    puts("Kill switch unreachable: a real sample would continue here. This one stops.");
    return 1;
}
