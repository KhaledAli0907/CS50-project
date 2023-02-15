// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

//define BucketSize = (26*to the power of 3) + 1
#define BucketSize 17577
//decalre variable to keep the words count
int WordsCount= 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = BucketSize;

// []->[]
// []

// Hash table
node *table[N];

//function to insert nodes at head
node *InsertAtHead(node *head, node*NewNode)
{
    NewNode->next = head;
    head = NewNode;
    return NewNode;
}
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //get hash index
    int HashIndex = hash(word);
    //cursor to go through all the words
    node *cursor = table[HashIndex];
    //check if it is not null
    while (cursor != NULL)
    {
        //traverse linked list searching for the word
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        //setting the cursor ptr to the next ptr
        cursor= cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //declaring varablies
    char word[LENGTH + 1];
    //Open File
    FILE *f = fopen(dictionary, "r");
    //make sure its't null
    if (f == NULL)
    {
        printf("Can't open file");
        return false;
    }
    //read words into file
    while (fscanf(f, "%s", word) != EOF)
    {
        //create new node for each word
        node *WordNode = malloc(sizeof(node));
        //check there is enough memory
        if (WordNode == NULL)
        {
            printf("No enough memory");
            return false;
        }
        //copy word into node
        strcpy(WordNode->word, word);
        WordNode->next = NULL;

        //Hashing the word
        int HashOrder = hash(word);
        //insert at hash order if it's empty
        if (table[HashOrder] == NULL)
        {
            table[HashOrder] = WordNode;
        }
        //else if it isn't
        else
        {
            InsertAtHead(table[HashOrder], WordNode);
        }
        WordsCount++;
    }
    //close the opening file to prevent memory leaks
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return WordsCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    //go through the nodes
    for (int i = 0; i < BucketSize; i++)
    {
        //setting the ptrs
        node *cursor = table[i];
        node *tmp = cursor;

        while (cursor != NULL)
        {
            //moving the cursor to the next node
            cursor = cursor->next;
            //deleting the first one
            free(tmp);
            //setting the temp ptr to cursor ptr
            tmp = cursor;
        }
    }
    return true;
}
