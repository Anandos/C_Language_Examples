#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2) //check there ia a key argument
    {
        printf("no key entered\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++) //check key has no other characters
    {
        if (isdigit(argv[1][i]) == 0)
        {
            printf("Key should only contain digits\n");
            return 1;
        }
    }

    int key = atoi(argv[1]); //argv key needs conversion from string to int
    string plaintext = get_string("plaintext: ");
    int length = strlen(plaintext);
    char ciphertext[length + 1];
    ciphertext[length] = '\0'; //close off ciphertext
    char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                       'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                      };

    int plainchar = 0;
    for (int i = 0; i < length; i++) //iterate through plaintext
    {
        if (isalpha(plaintext[i]) == 0) //if plaintext character is no alphabetic, skip
        {
            ciphertext[i] = plaintext[i];
        }
        else if (isupper(plaintext[i]))
        {
            plainchar = ((plaintext[i] - 65) + key) % 26;
            ciphertext[i] = alphabet[plainchar];
        }
        else
        {
            plainchar = ((plaintext[i] - 97) + key) % 26;
            ciphertext[i] = tolower(alphabet[plainchar]);
        }


        // printf("%i\n", plainchar);
    }

    printf("ciphertext: %s\n", ciphertext);

}