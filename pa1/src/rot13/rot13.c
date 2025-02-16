#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        return -1;
    }

    char *msg = argv[1];
    int i = 0, char_code = 0;

    while(msg[i] != '/0') {
        
        if(!isalpha(msg[i])) {
            char_code = (int)msg[i];
            //uppercase
            if(char_code <= 90) {
                char_code += 13;
                if(char_code > 90) {
                    char_code -= 26;
                }
            }
            //lowercase
            else if(char_code <= 122) {
                char_code += 13;
                if(char_code > 122) {
                    char_code -= 26;
                }
            }
            printf("%c", (char)char_code);
        }
        else {
            printf("%c", msg[i]);
        }
    }

}