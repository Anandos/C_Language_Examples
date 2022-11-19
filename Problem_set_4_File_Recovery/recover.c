#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint8_t
#include <cs50.h> // bool data type

int main(int argc, char *argv[])
{
    if (argc != 2) // check for valid cmd line arguments
    {
        printf("Takes one command line argument\n");
        return 1;
    }

    FILE *rawptr = fopen(argv[1], "r"); // create raw image data pointer

    uint8_t block_buffer[512]; // 512 block data buffer
    int count = 0; // jpeg count
    bool in_image = false; // before first image
    char *jpeg_name = malloc(8 * sizeof(char));
    sprintf(jpeg_name, "%03i.jpg", count); // create file name for jpeg
    FILE *jpegptr = NULL;  // declare jpeg file pointer before loops

    while (fread(block_buffer, 1, 512, rawptr)) // reading byte in 512 chunks until false
    {
        if (block_buffer[0] == 0xff && block_buffer[1] == 0xd8 && block_buffer[2] == 0xff) // loop for looking for jpeg
        {
            if (block_buffer[3] >= 0xe0 && block_buffer[3] <= 0xef)
            {
                if (in_image) // close and save current image data to new jpeg
                {
                    fclose(jpegptr);
                    // printf("Closing %s file\n", jpeg_name);
                    count ++;
                }
                else
                {
                    in_image = true;
                }
                // create new jpeg and write this block to it
                sprintf(jpeg_name, "%03i.jpg", count); // create file with name for jpeg
                jpegptr = fopen(jpeg_name, "w"); // create new file and pointer for new jpeg data
                fwrite(&block_buffer, 1, 512, jpegptr); // write jpeg data to file
                // printf("Writing %s image file\n", jpeg_name);
            }
        }
        else if (in_image) //if image has been found already, write next block to it
        {
            fwrite(&block_buffer, 1, 512, jpegptr);
            // printf("Continue writing jpeg %s\n", jpeg_name);
        }
    }
    free(jpeg_name);
    fclose(jpegptr);
    fclose(rawptr); // close raw file at end
    // printf("Closing files\n");
    return 0;
}