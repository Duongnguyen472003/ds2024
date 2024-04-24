#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 10000


typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;


void map(const char *line, WordCount **wordCounts, int *numWords) {
    char *token;
    char copyLine[1000];
    strcpy(copyLine, line);
    token = strtok(copyLine, " ");
    while (token != NULL) {
        
        int i, j = 0;
        char word[MAX_WORD_LENGTH];
        for (i = 0; token[i]; i++) {
            if (isalpha(token[i])) {
                word[j++] = tolower(token[i]);
            }
        }
        word[j] = '\0';
        if (strlen(word) > 0) {
            
            int found = 0;
            for (i = 0; i < *numWords; i++) {
                if (strcmp(word, (*wordCounts)[i].word) == 0) {
                    found = 1;
                    (*wordCounts)[i].count++;
                    break;
                }
            }
           
            if (!found) {
                *wordCounts = (WordCount *)realloc(*wordCounts, (*numWords + 1) * sizeof(WordCount));
                strcpy((*wordCounts)[*numWords].word, word);
                (*wordCounts)[*numWords].count = 1;
                (*numWords)++;
            }
        }
        token = strtok(NULL, " ");
    }
}


void reduce(WordCount *wordCounts, int numWords) {
    for (int i = 0; i < numWords - 1; i++) {
        for (int j = i + 1; j < numWords; j++) {
            if (wordCounts[i].count < wordCounts[j].count) {
                WordCount temp = wordCounts[i];
                wordCounts[i] = wordCounts[j];
                wordCounts[j] = temp;
            }
        }
    }
}

int main() {
    FILE *fp;
    char line[1000];
    WordCount *wordCounts = NULL;
    int numWords = 0;

   
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    
    while (fgets(line, sizeof(line), fp) != NULL) {
        map(line, &wordCounts, &numWords);
    }

    
    reduce(wordCounts, numWords);

    
    for (int i = 0; i < numWords; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
    }

    
    fclose(fp);
    free(wordCounts);

    return 0;
}
