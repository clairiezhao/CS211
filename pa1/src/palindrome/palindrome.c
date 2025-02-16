#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        return -1;
    }

    char *msg = argv[1];

    int len = 0;
    while(msg[len] != '\0') {
        len++;
    }

    int start = 0, end = len-1;

    while(start < end) {

        if(!isalpha(msg[start])) {
            start++;
            continue;
        }
        if(!isalpha(msg[end])) {
            end--;
            continue;
        }

        if(tolower(msg[start]) != tolower(msg[end])) {
            puts("no");
            return -1;
        }
        start++;
        end--;
    }

    puts("yes");
    return 0;
}