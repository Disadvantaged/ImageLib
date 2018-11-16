#ifndef IMAGELIB_H
#define IMAGELIB_H

#include <iostream>
#include <fstream>
#include "utils.h"
#include <random>
#include <ctime>
#include <cmath>

const float  PI_F = 3.14159265358979f;
constexpr short MAXLEN = 1024;

typedef unsigned char uchar;
struct Point_U {
    size_t x;
    size_t y;
};

struct Image_U {
    uchar **    data;
    Point_U     size;
    int         maxVal;
};

struct Image_F {
    float ** data;
    Point_U size;
};

struct Pixel_I {
    Point_U coord;
    int color;
};

struct Pixel_F {
    Point_U coord;
    float color;
};

struct Line_I {
    Point_U p1;
    Point_U p2;
    int color;
};

struct Circle_I {
    Point_U center;
    size_t radius;
    int color;
};

static void strToInt(const char *str, Image_U& image, int& i, int& k);

//Reads data from file and stores it
bool getData(std::ifstream& input, Image_U& data);

//Reads file header and stores information.
bool getInfo(std::ifstream& input, Image_U& size);

//Read image from .pgm (ASCII format) file
//if image.data is NULL this function returns sizes of image to allocate memory by user
//otherwise it fills image.data by values read from file
//return true if file have appropriate format otherwise false
bool GetPgm(Image_U & image, std::ifstream& input);

//Write image to .pgm (ASCII format) file
//return true if image was written, false otherwise
bool OutPgm(const Image_U & image, const char *path, const char * filename);

//Get pixel value by it's coordinates
//return false if pixel coordinates are out of the image
bool GetPixel(const Image_U & image, Pixel_I & pixel);

//Set pixel value by it's coordinates. Color should be in range [0,255]
//return false if pixel coordinates are out of the image
bool SetPixel(Image_U & image, const Pixel_I & pixel);

//Draw line by two points
void DrawLine(Image_U & image, const Line_I & line);

//Draw rectangle by two points
void DrawRectangle(Image_U & image, const Line_I & line);

//Draw line by coordinate center and radius
void DrawCircle(Image_U & image, const Circle_I & circle);

//Finds maximum value in image and updates it respectively
void updateMaxValue(Image_U& image);

//Adds noise to image
void giveEmSomePepper(Image_U & image, float percent = 5);

//Generate gaussian kernel of size = gauss.size.x X gauss.size.y
//gauss.data should be allocated by user
//if gauss.size.x != gauss.size.y returns false
bool GetGaussianKernel(Image_F & gauss, float sigma = 1.0f);

//Blur image by convolving with gaussian kernel  
//return false in case of any error
bool GaussianBlur(Image_U& image, const Image_F & gauss);

//Gets median for i, j-element by translating all the elements
//in given radius from this element
//to array and finding median of this array.
uchar getMedian(Image_U& image, size_t i, size_t j, int radius, uchar *arr);

//Filter image noise by median filter of size = (2*radius + 1) * (2*radius + 1) 
//return false in case of any error
bool MedianBlur(Image_U & image, size_t radius = 1);

//Calculate median pixel value of the whole image 
//return false in case of any error
bool Median(const Image_U & image, float & median);

//Calculate average pixel value of the whole image 
//return false in case of any error
bool Average(const Image_U & image, float & average);

//Calculate the most popular pixel value of the whole image 
//return false in case of any error
bool Mode(const Image_U & image, float & mode);

#endif