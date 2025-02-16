#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        return -1;
    }

    char *msg = argv[1];
    if(msg[0] == NULL) {
        return -1;
    }

    int len = sizeof(msg) / sizeof(msg[0]);
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

        if(strcmp(tolower(msg[start]), tolower(msg[end])) != 0) {
            puts("no");
            return -1;
        }
        start++;
        end--;
    }

    puts("yes");
    return 0;
}