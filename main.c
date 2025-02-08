/*
 COP 3502C Final Project
 This program is written by: Jake Inman
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Trie {
    int frequency; // frequency word is used
    int prefixOf; // number of words it is a prefix of
    int maxChild; // child node (a-z) w/ highest prefixOf value
    struct Trie* next[26]; // a-z nodes
} Trie;

Trie* init()
{
    // allocate memory for tree
    Trie* trie = malloc(sizeof(Trie));
    // set other variables
    trie->frequency = 0;
    trie->prefixOf = 0;
    trie->maxChild = 0;
    // init all child nodes to NULL
    for (int i = 0; i < 26; i++)
        trie->next[i] = NULL;

    return trie;
}

void insert(Trie* dic, char word[], int freq, int k)
{
    if(word[k] == '\0')
    {
        dic->frequency += freq; // increment frequency of usage
        dic->prefixOf += freq; // last letter is a prefix of itself
        return;
    }
    ///// runs only if not last letter in word /////

    // increments prefixOf for each node
    dic->prefixOf += freq; // each node is basically a prefix of inserted word/words

    int nextIndex = word[k] - 'a';
    if(dic->next[nextIndex] == NULL)
        dic->next[nextIndex] = init();

    insert(dic->next[nextIndex], word, freq, k+1); // recursion to next letter

    /* after recursion (going back and updating maxChild value) */

    int maxPrefix = 0; // value to store max prefixOf value

    for(int i=0; i<26; i++) // iterate through all children
    {
        // find max prefixOf value between all child nodes
        if(dic->next[i] && dic->next[i]->prefixOf > maxPrefix)
            maxPrefix = dic->next[i]->prefixOf;
    }
    dic->maxChild = maxPrefix; // set new maxChild value
}

void printAll(Trie* tree, char cur[])
{

    // Stop!
    if (tree == NULL) return;

    // Print this node, if it's a word.
    if (tree->frequency)
        printf("%s %d\n", cur, tree->frequency);

    // store length for index
    int len = strlen(cur);
    //printf("%s %d\n", cur, tree->maxChild);

    // Recursively print all words in each subtree,
    // in alpha order.
    int i;
    for (i=0; i<26; i++)
    {
        cur[len] = (char)('a'+i);
        cur[len+1] = '\0';
        printAll(tree->next[i], cur);
    }
}

void searchPrefix(Trie* dic, char prefix[])
{
    char letters[26]; // array of next suggested letters
    int p = 0; // tracker for letters arr
    int k = 0; // tracker for prefix arr

    int len = strlen(prefix); // stores len of prefix word

    int nextIndex = 0; // value to store next letter index
    for(int i=0; i<len; i++)
    {
        // this block will travel through the trie to the end of the prefix word
        // as long as the nodes it travels through exist, it is a valid prefix
        nextIndex = prefix[k++] - 'a';
        if(dic->next[nextIndex] && dic->next[nextIndex]->maxChild > 0)
        {
            dic = dic->next[nextIndex];
            continue;
        }
        printf("unrecognized prefix\n");
        return;
    }

    int max = dic->maxChild; // value to store maxChild value to compare to
    for(int i=0; i<26; i++)
    { // iterate through all children
        if(dic->next[i] && dic->next[i]->prefixOf >= max) // if letter is in use & = maxChild value
            letters[p++] = (char)('a'+i); // add letter to string (used the same thing as in the printAll func)
    }
    letters[p] = '\0'; // null terminator at end of string

    printf("%s\n", letters); // prints suggested letters
}

void freeTrie(Trie* dic)
{
    if(!dic) return; // base case if node is NULL return

    for(int i=0; i<26; i++)
    { // iterate through each child node
        if(dic->next[i]) // if it exists, pass it into recursive call
            freeTrie(dic->next[i]);
    }

    free(dic); // free the node
}

void query(Trie* Dictionary, int num)
{
    for(int i=0; i<num; i++)
    {
        // command variables
        int command; char word[2000000]; int count;

        scanf("%d", &command); // input for command

        switch(command)
        {
            case 1: // insert word
                scanf("%s %d", word, &count);
                insert(Dictionary, word, count, 0);

                break;

            case 2: // search prefix
                scanf("%s", word);
                searchPrefix(Dictionary, word);
                break;

            case 3: // print command (for testing)
                printf("Dictionary:\n");
                word[0] = '\0';
                printAll(Dictionary, word);

            default:
                break;
        }
    }
}

int main(void)
{
    Trie *Dictionary = init(); // init root of trie

    /* Code stores words from the input into a trie with extra variables to help with certain processes
     * Users can search for a certain prefix, which the code will evaluate in the trie
     * then find whatever letter/s are the most likely to come next after that prefix */

    /* USAGE */
    // input num commands want to do
    // 1 for insert word ( 1 word 80 <- its frequency )
    // 2 for search prefix ( 2 prefix )

    int numCommands; // input for num of commands
    scanf("%d", &numCommands);

    query(Dictionary, numCommands); // calls query function

    freeTrie(Dictionary);

    return 0;
}
