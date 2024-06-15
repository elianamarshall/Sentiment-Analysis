#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct words {
    char *word;
    float score; //polarity score
    float SD; //standard deviation
    int SIS_array[10]; //sentiment intensity scores array
};

void readLexicon(const char *filename, struct words **lexicon, int *numWords);
void freeLexicon(struct words *lexicon, int numWords);
float sentimentScore(struct words *lexicon, int numWords, const char *word);
float emoticonScore(struct words *lexicon, int numWords, const char *emoticon);
void sentimentAnalysis(const char *filename, struct words *lexicon, int numWords);

#endif