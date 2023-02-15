#include<stdio.h>
#include<cs50.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

int main (void)
{
    string Text = get_string("Enter Your Text Here: ");
    int Letters = 0;
    int Words = 0;
    int Sentence = 0;

    for (int l = 0; l < strlen(Text) ;l++)
        {
            if (isalpha(Text[l]))
            {
                Letters++;
            }
            else if (isspace (Text[l]))
            {
                Words++;
            }
            else if (Text[l]=='.'|| Text[l] == '?'|| Text[l] == '!')
            {
                Sentence++;
            }
        }

    float L = (Letters*100)/(Words+1);
    float S = (Sentence*100)/(Words+1);
    float Grade = 0.0588 * L - 0.296 * S - 15.8;
    //printf("Your Grade is: %f \n",Grade);

    if ((int) round(Grade) <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if ( round(Grade) > 1 && round(Grade) < 7.5)
    {
        printf("Grade %i\n", (int) round(Grade));
    }
    else if ( round(Grade) > 7.6 &&  round(Grade) < 16)
    {
        printf("Grade %i\n",(int) round(Grade));
    }
    else if ((int) round(Grade)>=16)
    {
        printf("Grade 16+\n");
    }
}
