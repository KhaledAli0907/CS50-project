#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    // get user input while making sure it's between 1 and 8
    do
    {
        height = get_int("Enter Height:");
    }
    while (height < 1 || height > 8);
    // loop through raws
    for (int row = 0; row < height; row++)
    {
        // loop and print spaces
        for (int space = height - 2; space >= row; space--)
        {
            printf(" ");
        }
        // loop and print hashes
        for (int hash = 0; hash <= row; hash++)
        {
            printf("#");
        }
        // print gap
        printf("  ");
        // loop to continue print hashes
        for (int hash = 0; hash <= row; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}
