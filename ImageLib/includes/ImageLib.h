#ifndef UTILS_H
#define UTILS_H

struct Image_U;
struct Image_F;

typedef unsigned char uchar;

template<class T>
inline void swap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

void fsort(uchar *arr, size_t size);

//Allocates memory for copy and copies data from image.
void copyImage(const Image_U& image, Image_U& copy);

//Copies string
void fstrcpy(char *dest, const char *src);

//returns string length
int fstrlen(const char *str);

//lexicographical comparison
int fstrcmp(const char *str1, const char *str2);

//concatenates 2 strings together
void fstrcat(char *dest, const char *src);

//Allocates memory for image and reads it from file
//Returns false if there was an error and true otherwise
bool readImage(Image_U &image, const char *filename);

//path - path to file from input
//appendix - It will be added to the end of filename
//output - output filename. Consists of input filename and appendix
void getOutputname(const char *path, const char *appendix, char *output);

//Frees main object
void free_memory(Image_U& image);
void free_memory(Image_F& image);

#endif