#include <iostream>
#include <cstdlib>
#include <ctime>
#include <synchapi.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define CONSOLE_WIDTH 300
// #define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT (CONSOLE_WIDTH / (16.0 / 9.0)) / 2.0
#include "stb_image.hpp"
#include "stb_image_write.hpp"
#include "stb_image_resize.hpp"
typedef unsigned char uint8_t;
const std::string density = "@#W$9876543210?!abc;:+=-,._ ";

// convert an image width and height to a console size
void convertToConsoleSize(int &width, int &height, uint8_t *&rgb_image)
{
    // get the ratio of the image to the console
    float ratio = (float)width / (float)height;
    // get the ratio of the console to the image
    float ratio2 = (float)height / (float)width;
    // if the image is wider than the console
    if (ratio > 1)
    {
        // resize the image to the console width
        stbir_resize_uint8(rgb_image, width, height, 0, rgb_image, CONSOLE_WIDTH, (CONSOLE_WIDTH * ratio2) / 2, 0, 3);
        // set the width to the console width
        width = CONSOLE_WIDTH;
        // set the height to the console height
        height = (int)(CONSOLE_WIDTH * ratio2) / 2;
    }
    // if the image is taller than the console
    else if (ratio < 1)
    {
        // resize the image to the console height
        stbir_resize_uint8(rgb_image, width, height, 0, rgb_image, (CONSOLE_HEIGHT * ratio), CONSOLE_HEIGHT / 2, 0, 3);
        // set the width to the console width
        width = (int)(CONSOLE_HEIGHT * ratio);
        // set the height to the console height
        height = CONSOLE_HEIGHT / 2;
    }
    // if the image is the same size as the console
    else
    {
        stbir_resize_uint8(rgb_image, width, height, 0, rgb_image, CONSOLE_WIDTH, CONSOLE_HEIGHT, 0, 3);
        // set the width to the console width
        width = CONSOLE_WIDTH;
        // set the height to the console height
        height = CONSOLE_HEIGHT;
    }
}

// convert an image to a ascii image using the density string
void convertToAscii(int width, int height, uint8_t *&rgb_image)
{
    // loop through the image
    for (int i = 0; i < width * height * 3; i += 3)
    {
        // get the average of the rgb values
        uint8_t average = (rgb_image[i] + rgb_image[i + 1] + rgb_image[i + 2]) / 3;
        // get the index of the density string
        int index = (int)(density.size() - (average / 255.0 * (density.size() - 1)));
        // set the rgb values to the density string character
        rgb_image[i] = density[index];
        rgb_image[i + 1] = density[index];
        rgb_image[i + 2] = density[index];
    }
}

// display an ascii image in the console
void displayAscii(int width, int height, uint8_t *&rgb_image)
{
    // loop through the image
    for (int i = 0; i < width * height * 3; i += 3)
    {
        // print the character
        std::cout << (char)rgb_image[i];
        // if the end of the line is reached
        if ((i / 3) % width == width - 1)
        {
            // print a new line
            std::cout << std::endl;
        }
    }
}

// convert back an uint8_t array to an image file
void convertToImageFile(int width, int height, uint8_t *rgb_image, std::string filename)
{
    // create the image file
    FILE *file = fopen(filename.c_str(), "wb");
    // write the image file
    stbi_write_png(filename.c_str(), width, height, 3, rgb_image, width * 3);
    // close the image file
    fclose(file);
}

int main()
{
    int width, height, bpp;

    uint8_t *rgb_image = stbi_load("img/cat.jpg", &width, &height, &bpp, 3);

    // convert the image to the console size
    convertToConsoleSize(width, height, rgb_image);

    // create a new image file
    convertToImageFile(width, height, rgb_image, "img/converted.png");

    // convert the image to ascii
    convertToAscii(width, height, rgb_image);

    // display the ascii image in the console
    displayAscii(width, height, rgb_image);

    // Display a square of "#" of size 10x10 in the command line
    // system("cls");
    // for (int i = 0; i < 76; i++)
    // {
    //     for (int j = 0; j < 350; j++)
    //     {
    //         std::cout << "#";
    //     }
    //     std::cout << std::endl;
    // }
    // Sleep(3000);

    // system("cls");
    // for (int i = 0; i < 10; i++)
    // {
    //     float startTicks = time(NULL);
    //     for (int o = 0; o < 5; o++)
    //     {
    //         for (int k = 0; k < i; k++)
    //         {
    //             std::cout << " ";
    //         }
    //         for (int j = 0; j < 5; j++)
    //         {
    //             std::cout << "#";
    //         }
    //         std::cout << std::endl;
    //     }
    //     float frameTicks = time(NULL) - startTicks;
    //     // 1 second between each frame
    //     Sleep(300 - frameTicks);
    //     // clear console
    //     system("cls");
    // }

    stbi_image_free(rgb_image);

    return 0;
}