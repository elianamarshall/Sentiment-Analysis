#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char *argv[]) {
    if(argc != 3) { //if there are an incorrect number of arguments
        fprintf(stderr, "Usage: %s <vader_lexicon.txt> <validation.txt>\n", argv[0]);
        return 1;
    }
    struct words *lexicon = NULL; //pointer to store the data from the lexicon
    int numWords = 0; //variable to store the number of words in the lexicon

    readLexicon(argv[1], &lexicon, &numWords);
    sentimentAnalysis(argv[2], lexicon, numWords);
    freeLexicon(lexicon, numWords);

    return 0;
}