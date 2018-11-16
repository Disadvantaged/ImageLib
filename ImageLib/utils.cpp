#include "utils.h"
#include "ImageLib.h"

void addElement(uchar *heap, int &len, uchar num) {
    int i = ++len;
    heap[i] = num;
    while (i > 1 && heap[i] > heap[i / 2]) {
        swap(heap[i], heap[i / 2]);
        i /= 2;
    }
}

bool cmp(uchar a, uchar b) {
    return a > b;
}

void heapify(uchar *heap, int len, int pos, bool(*cmp)(uchar, uchar)) {
    int left = 2 * pos;
    int right = 2 * pos + 1;
    int largest = pos;
    if (left <= len && !cmp(heap[pos], heap[left])) {
        largest = left;
    }
    if (right <= len && !cmp(heap[largest], heap[right])) {
        largest = right;
    }
    if (largest != pos) {
        swap(heap[largest], heap[pos]);
        heapify(heap, len, largest, cmp);
    }
}

void buildHeap(uchar *arr, int n) {
    int heapLen = 0;
    while (heapLen != n) {
        addElement(arr, heapLen, arr[heapLen + 1]);
    }
}

void fsort(uchar *arr, size_t n) {
    buildHeap(arr, n);
    for (int i = n; i > 1; i--) {
        swap(arr[1], arr[i]);
        heapify(arr, i - 1, 1, cmp);
    }
}

int fstrlen(const char * str) {
    int i = 0;
    while (str[i++]) { }
    return i - 1;
}

int fstrcmp(const char * str1, const char * str2) {
    int i = 0;
    while (str1[i] && str2[i] && str1[i] == str2[i]) {
        i++;
    }
    return str1[i] - str2[i];
}

void fstrcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}
void fstrcat(char * dest, const char * src) {
    int len = fstrlen(dest);
    int i = 0;
    while (src[i]) {
        dest[i + len] = src[i];
        i++;
    }
    dest[i + len] = '\0';
}

void copyImage(const Image_U& image, Image_U& copy) {
    copy.size = image.size;
    copy.maxVal = image.maxVal;
    copy.data = new uchar*[image.size.y];
    for (size_t i = 0; i < copy.size.y; i++) {
        copy.data[i] = new uchar[copy.size.x];
        memcpy(copy.data[i], image.data[i], sizeof(uchar) * image.size.x);
    }
}

bool readImage(Image_U& image, const char *filename) {
    std::ifstream    input(filename);
    if (!GetPgm(image, input)) {
        return false;
    }
    image.data = new uchar*[image.size.y];
    for (size_t i = 0; i < image.size.y; i++) {
        image.data[i] = new uchar[image.size.x];
    }
    if (!GetPgm(image, input)) {
        return false;
    }
    return true;
}

void getOutputname(const char * path, const char *appendix, char * outputName) {
    int pathLen = fstrlen(path);
    int i = pathLen - 1;
    int j = 0;
    if (path[i] == '\\') {
        i -= 2;
    }
    while (path[i] && path[i] != '\\') {
        i--;
    }
    i++;
    while (path[i]) {
        if (path[i] == '.') {
            outputName[j] = 0;
            fstrcat(outputName, appendix);
            j += fstrlen(appendix);
        }
        outputName[j] = path[i];
        i++;
        j++;
    }
    outputName[j] = 0;
}

void free_memory(Image_U& image) {
    for (size_t i = 0; i < image.size.y; i++) {
        delete[] image.data[i];
    }
    delete[] image.data;
}

void free_memory(Image_F& image) {
    for (size_t i = 0; i < image.size.y; i++) {
        delete[] image.data[i];
    }
    delete[] image.data;
}