#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

void readLexicon(const char *filename, struct words **lexicon, int *numWords) { //reads words, polarity scores, standard deviations, and sentiment intensity score arrays from vader_lexicon.txt
    FILE *file = fopen(filename, "r"); //opens the file to read it

    char line[1024]; //temporary variable to store each line read from vader_lexicon.txt
    *lexicon = NULL; //pointer to a pointer to a struct representing the lexicon
    *numWords = 0; //tracks the number of words in the lexicon

    while (fgets(line, sizeof(line), file)) { //reads each line from the file
        struct words w; //creates a new instance of struct words

        char *substring = strtok(line, " \t\n"); //splits each sentence into individual words

        w.word = strdup(substring); //copies the word to the struct words instance
        substring = strtok(NULL, " \t\n"); //gets the score of the word
        w.score = substring ? atof(substring) : 0.0; //converts the score a float or sets it to 0.0 if the substring is NULL

        (*numWords)++; //increments the number of words
        *lexicon = realloc(*lexicon, (*numWords) * sizeof(struct words)); //reallocates memory for lexicon
        (*lexicon)[*numWords - 1] = w; //adds the struct words instance to lexicon
    }
    fclose(file); //closes the file
}

void freeLexicon(struct words *lexicon, int numWords) { //frees the memory allocated for the lexicon
    for(int i = 0; i < numWords; i++) { //iterates through each word in the lexicon
        free(lexicon[i].word); //frees the memory allocated for each word
    }
    free(lexicon); //frees the memory allocated for the lexicon
}

float sentimentScore(struct words *lexicon, int numWords, const char *word) { //gets the sentiment score of a word
    for(int i = 0; i < numWords; i++) { //iterates through each word in the lexicon
        if(strcmp(lexicon[i].word, word) == 0) { //checks if the word is in the lexicon
            return lexicon[i].score; //returns the sentiment score of the word
        }
    }
    return 0; //if the word is not in the lexicon, return 0 as the sentiment score
}

float emoticonScore(struct words *lexicon, int numWords, const char *emoticon) {
    for (int i = 0; i < numWords; i++) { //iterates through each word in the lexicon
        if (strcmp(lexicon[i].word, emoticon) == 0) { //checks if the emoticon is in the lexicon
            return lexicon[i].score; //returns the sentiment score of the emoticon
        }
    }
    return 0; //if the emoticon is not in the lexicon, return 0 as the sentiment score
}

void sentimentAnalysis(const char *filename, struct words *lexicon, int numWords) { 
    FILE *file = fopen(filename, "r"); //opens the file to read it

    char line[1024]; //temporary variable to store each line read from vader_lexicon.txt

    printf("    string sample                                            score\n");
    printf("--------------------------------------------------------------------------------------------\n");
    
    while(fgets(line, sizeof(line), file)) { //loops through each line of the file
        char sentence[1024] = ""; //temporary variable to store a sentence
        float sum = 0.0; //variable to store the total sentiment score of each word in a sentence
        int substringCount = 0; //counter for the number of words in each sentence
        char *split = " \n"; //splits the string after every space
        char *substring = strtok(line, split); //splits each sentence into individual words

        while(substring) { //loops through each substring
            char oSubstr[1024]; //variable to store the original substring
            strcpy(oSubstr, substring); //stores the original substring

            // Check if the substring is an emoticon
            float emoScore = emoticonScore(lexicon, numWords, oSubstr); //calculates the sentiment score of the current substring
            if (emoScore != 0.0) { //if the emoticon is not in the lexicon or if the substring is a word instead of an emoticon
                strcat(sentence, oSubstr); //concatenates the original substring to the sentence
            } else {
                char *punctuation = strpbrk(oSubstr, ",.!?;:"); //finds the first occurence of any punctuation in the original substring

                if (punctuation) { //if there is punctuation
                    *punctuation = '\0'; // replaces the punctuation with a null terminator
                    strcat(sentence, oSubstr); //concatenates the original substring to the sentence

                    strcat(sentence, punctuation); //concatenates the punctuation to the sentence
                } else {
                    strcat(sentence, oSubstr); //concatenates the original substring to the sentence
                }
            }
            strcat(sentence, " "); //adds a space after concatenating each substring

            char lSubstr[1024]; //variable to store the lowercase version of the substring
            strcpy(lSubstr, oSubstr); //copies the original substring to the lowercase variable
            for (int i = 0; lSubstr[i]; i++) { //iterates through each character of the lowercase substring
                lSubstr[i] = tolower(lSubstr[i]); //converts each character in the substring to lowercase
            }
            float score = sentimentScore(lexicon, numWords, lSubstr); //calculate the sentiment score of the lowercase substring
            sum += score; //adds the sentiment score of the substring to the total sentiment score
            substringCount++; //increments the counter for the number of substrings

            substring = strtok(NULL, split); //moves to the next substring in the sentence
        }

        if (substringCount > 0) { //if there are substrings in the sentence
            float average = sum / substringCount; //calculates the average sentiment score
            printf("%s   %.2f\n", sentence, average); //prints the sentence and its sentiment score
        }
    }
    fclose(file); //closes the file
}