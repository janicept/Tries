//Janice Kleinhenz
//COP3502C
//April 19, 2024
//Lab Assignment 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    int count;
    struct Trie *next[26];
};

struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node) {
        node->count = 0;
        for (int i = 0; i < 26; i++) {
            node->next[i] = NULL;
        }
    }
    return node;
}

void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->next[index]) {
            current->next[index] = createTrie();
        }
        current = current->next[index];
    }
    current->count++;
}

int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->next[index]) {
            return 0; // Word not found
        }
        current = current->next[index];
    }
    return current->count;
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie) {
        for (int i = 0; i < 26; i++) {
            deallocateTrie(pTrie->next[i]);
        }
        free(pTrie);
    }
    return NULL; // Return NULL after deallocation
}

int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return -1; // Return an error code
    }

    int wordCount = 0; // Initialize word count to 0
    char word[100]; // Assuming maximum word length is 100 characters

    // Read words from file until EOF or maximum number of words
    while (wordCount < 256 && fscanf(file, "%s", word) == 1) {
        pInWords[wordCount] = strdup(word); // Duplicate the word and store in array
        if (pInWords[wordCount] == NULL) {
            printf("Memory allocation failed\n");
            fclose(file); // Close the file before returning
            return -1; // Return an error code
        }
        wordCount++; // Increment word count
    }

    fclose(file); // Close the file
    return wordCount; // Return the number of words read
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);

    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    if (pTrie != NULL)
		printf("There is an error in this program\n");

    return 0;
}

