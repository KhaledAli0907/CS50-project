#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>


int main(int argc, string argv[])
{
    // make the sure the user entered the right command
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./carsar key\n");
            return 1;
        }
    }
    // store the key as an integer
    int key = atoi(argv[1]);
    // ask the user for text
    string Text = get_string("Enter Text Here: ");
    // return the cyphered text
    printf("ciphertext: ");
    // encryption loop
    for (int i = 0; i < strlen(Text); i++)
    {
        // check if its alphabetical
        if (isalpha(Text[i]))
        {
            // check if it's UpperCase
            if (isupper(Text[i]))
            {
                int cupper = (((Text[i] - 65) + key) % 26) + 65;
                printf("%c", cupper);
            }
            // check if it's lowerCase
            else if (islower(Text[i]))
            {
                int clower = (((Text[i] - 97) + key) % 26) + 97;
                printf("%c", clower);
            }
        }
        // print any symbol as it is
        else
        {
            printf("%c", Text[i]);
        }
    }
    printf("\n");
}