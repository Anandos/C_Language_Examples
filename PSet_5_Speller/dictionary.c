// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <string.h> //
#include <stdio.h> //
#include <stdlib.h> //
#include <cs50.h> //

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; //45 + 1
    struct node *next;
}
node;

const unsigned int HASH_TOTAL = 676; //26 * 26 = 676 hash buckets
int word_count = 0; //count dictionary words
node *hashtable[HASH_TOTAL]; //Hash table

bool check(const char *word) //Returns true if word is in dictionary, else false
{
    char lower_word[45];
    int i = 0;
    for (i = 0; i < strlen(word); i++) //convert word to lower case
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[i] = '\0';

    int word_bucket = hash(lower_word);
    node *n = hashtable[word_bucket];

    while (n != NULL)
    {
        if (strcmp(n -> word, lower_word) == 0)
        {
            // printf("true match: %s with %s\n", n -> word, lower_word);
            return true;
        }
        // printf("skipping, %s with %s\n", n -> word, word);
        n = n -> next;
    }

    // printf("no match in bucket: %s %i\n", lower_word, word_bucket);
    return false; //return false if word is not found in bucket's linked list
}

unsigned int hash(const char *word) //Hashes word to a number
{
    int number;
    char x = word[0];
    number = (((int)x - 97) * 26); //hash first letter of word
    if (isalpha(word[1]))
    {
        char y = word[1];
        number += ((int)y - 97); //hash second letter of word
    }
    // printf("hashed word; %s %i\n", word, number);
    return number;
}

bool load(const char *dictionary) //Loads dictionary into memory, returning true if successful, else false
{
    FILE *dictptr = fopen(dictionary, "r"); //open dictionary file
    if (dictptr == NULL) //check dictionary file pointer
    {
        fclose(dictptr);
        return false;
    }

    char current_word[LENGTH + 1];
    int hashnum = 0;
    while (fscanf(dictptr, "%s", current_word) != EOF) //fscanf each line of dictionary file,
    {
        word_count++;
        hashnum = hash(current_word); //get hash number of word

        node *n = malloc(sizeof(node)); //node for current word
        if (n == NULL)
        {
            return false; //error check
        }

        strcpy(n -> word, current_word); //copy dictionary word into n node
        n -> next = hashtable[hashnum]; //n node points to same as hash bucket pointer

        hashtable[hashnum] = n; //hash bucket points to n node

        // printf("copied: %s %i\n", n -> word, hashnum);
    }

    node *m = malloc(sizeof(node)); //add 'the' to dictionary
    if (m == NULL)
    {
        return false;
    }
    hashnum = hash("the");
    strcpy(m -> word, "the");
    m -> next = hashtable[hashnum];
    hashtable[hashnum] = m;

    fclose(dictptr);

    for (int i = 0; i < HASH_TOTAL; i++) //for each bucket in hash table
    {
        node *x = hashtable[i];
        while (x != NULL)
        {
            // printf("success: %s\n", x -> word);
            x = x -> next;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void) //get rid of all the node pointers in the tree
{
    for (int i = 0; i < HASH_TOTAL; i++) //for each bucket in hash table
    {
        node *x = hashtable[i];
        node *y = x;
        if (hashtable[i] == NULL) //if bucket is empty
        {
            continue;
        }
        while (x -> next != NULL) //free each node in linked list for bucket
        {
            y = x;
            free(y);
            x = x -> next; //temp points to next, free current, hashtable points to temp
        }
        //get to end of linked list
        free(x);
        hashtable[i] = NULL;
    }

    return true;
}
