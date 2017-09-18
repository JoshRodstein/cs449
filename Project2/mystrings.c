/*
MyStrings.c
Joshua Rodstein
CS449 / Project 2 / Part 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char* argv[]) {
    int i, j, char_count, pos = 0;
    char cur_char;
    FILE* fp;
    char *buffer;

    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("Error: File not found\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    char_count = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(sizeof(char)*char_count);
    char_count = 0;

     while(feof(fp) == 0) {
        fread(&cur_char, sizeof(char), 1, fp);
        if((cur_char >= 32) && (cur_char <= 126)){
            buffer[pos] = cur_char;
            char_count++;
            pos++;
        } else {
            if(char_count >= 4) {
	      for(j = (pos - char_count); j < pos; j++) {
                    printf("%c", buffer[j]);
                }
                printf("\n");
                char_count = 0;
                pos++;
            } else {
                char_count = 0;
                pos++;
            }
        }
    }
    free(buffer);
    fclose(fp);
    
    return 0;
}

