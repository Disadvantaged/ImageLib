#include "ImageLib.h"
#include "utils.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Not enough arguments\n";
        return -1;
    } else {
        Image_U image;
        char outputFilename[MAXLEN];

        image.data = nullptr;
        if (!readImage(image, argv[1])) {
            return -1;
        }
        Image_U copy;
        copyImage(image, copy);

        float percent;
        cout << "percent of black and white dots: ";
        cin >> percent;
        
        //Creates kernel for Gaussian blur
        Image_F kernel;
        float sigma;
        cout << "Gaussian kernel size: ";
        cin >> kernel.size.x;
        cout << "sigma: ";
        cin >> sigma;
        kernel.size.y = kernel.size.x;
        kernel.data = new float*[kernel.size.y];
        for (size_t i = 0; i < kernel.size.y; i++) {
            kernel.data[i] = new float[kernel.size.x];
        }
        if (!GetGaussianKernel(kernel, sigma)) {
            cerr << "Error creating gaussian kernel\n";
            return -1;
        }
        
        //Applies SaltAndPepper
        giveEmSomePepper(copy, percent);
        getOutputname(argv[1], "Salted", outputFilename);
        OutPgm(copy, argv[2], outputFilename);
        
        //Output filtered image by Gaussian blur
        GaussianBlur(copy, kernel);
        getOutputname(argv[1], "Gaussed", outputFilename);
        OutPgm(copy, argv[2], outputFilename);

        //Creates another copy of image and applies SaltAndPepper
        size_t rad;
        cout << "Median blur radius: ";
        cin >> rad;
        for (size_t i = 0; i < copy.size.y; i++) {
            memcpy(copy.data[i], image.data[i], sizeof(uchar) * image.size.x);
        }
        giveEmSomePepper(copy, percent);

        //Output filtered image
        MedianBlur(copy, rad);
        getOutputname(argv[1], "Median", outputFilename);
        OutPgm(copy, argv[2], outputFilename);


        //Output ordinary image
        getOutputname(argv[1], "Out", outputFilename);
        OutPgm(image, argv[2], outputFilename);

        //Black image
        Image_U blackImage;
        blackImage.size = { 512, 256 };
        blackImage.maxVal = 255;
        blackImage.data = new uchar*[blackImage.size.y];
        for (int i = 0; i < blackImage.size.y; i++) {
            blackImage.data[i] = new uchar[blackImage.size.x];
            memset(blackImage.data[i], 0, sizeof(uchar) * blackImage.size.x);
        }
        fstrcpy(outputFilename, "Black.pgm");
        Point_U p1 = { 10, 10 }, p2 = { 200, 200 };
        Line_I d1 = { p1, p2, 255 };
        Circle_I circle = { {100, 100}, 90, 255 };
        DrawCircle(blackImage, circle);
        DrawLine(blackImage, d1);
        DrawRectangle(blackImage, d1);
        OutPgm(blackImage, argv[2], outputFilename);
        float avg, median, mode;
        Average(blackImage, avg);
        Median(blackImage, median);
        Mode(blackImage, mode);
        cout << avg << ' ' << median << ' ' << mode << '\n';


        system("pause");
        free_memory(image);
        free_memory(copy);
//        free_memory(kernel);
    }
}