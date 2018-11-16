#include "ImageLib.h"

static void strToInt(const char *str, Image_U& image, int& i, int& k) {
    int pixel = -1;
    int j = 0;
    while (str[j]) {
        switch (str[j]) { // because it is faster
        case ' ':
            image.data[i][k++] = pixel;
            pixel = 0;
            if (k == image.size.x) {
                i++;
                k = 0;
            }
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            pixel = (pixel == -1 ? 0 : pixel * 10) + str[j] - '0';
            break;//if pixel equal to -1 then there was no symbols before
        }
        j++;
    }
    if (pixel != -1) {
        image.data[i][k++] = pixel;
        if (k == image.size.x) {
            i++;
            k = 0;
        }
    }
}

bool getData(std::ifstream & input, Image_U& image) {
    int i = 0, k = 0;
    while (!input.eof()) {
        char *str = new char[MAXLEN];
        input.getline(str, MAXLEN);
        strToInt(str, image, i, k);
        delete[] str;
    }
    return true;
}

bool getInfo(std::ifstream & input, Image_U& image) {
    char str[MAXLEN];

    input.getline(str, MAXLEN); //pgm magic number
    if (fstrcmp(str, "P2"))
        return false;
    input.getline(str, MAXLEN); //comment
    input >> image.size.x >> image.size.y;
    input >> image.maxVal;
    return true;
}

bool GetPgm(Image_U & image, std::ifstream& input) {

    if (input.fail()) {
        std::cerr << "Couldn't open the file\n";
        return false;
    }
    if (image.data == nullptr && !getInfo(input, image)) {
        std::cerr << "Wrong file header\n";
        return false;
    }
    if (image.data != nullptr) {
        if (!getData(input, image)) {
            return false;
        }
    }
    return true;
}

bool OutPgm(const Image_U & image, const char *path, const char * filename) {
    char fullpath[MAXLEN];

    fullpath[0] = 0;
    fstrcat(fullpath, path);
    if (path[fstrlen(path) - 1] != '\\') {
        fstrcat(fullpath, "\\");
    }
    fstrcat(fullpath, filename);

    std::ofstream output{ fullpath };
    if (output.fail()) {
        return false;
    }
    output << "P2\n# " << fullpath << '\n';
    output << image.size.x << ' ' << image.size.y << '\n' << image.maxVal << '\n';
    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            output << (int)image.data[i][j] << ' ';
        }
        output << '\n';
    }
    output.close();
    return true;
}

bool GetPixel(const Image_U & image, Pixel_I & pixel) {
    if (pixel.coord.x < 0 || pixel.coord.y < 0 ||
        pixel.coord.y >= image.size.y || pixel.coord.y >= image.size.y) {
        return false;
    }
    pixel.color = image.data[pixel.coord.y][pixel.coord.x];
    return true;
}

bool SetPixel(Image_U & image, const Pixel_I & pixel) {
    if (pixel.coord.x < 0 || pixel.coord.y < 0 ||
        pixel.coord.y >= image.size.y || pixel.coord.y >= image.size.y) {
        return false;
    }
    image.data[pixel.coord.y][pixel.coord.x] = pixel.color;
    return true;
}

void DrawLine(Image_U & image, const Line_I & line) {
    Pixel_I p1 = { line.p1, line.color };
    Pixel_I p2 = { line.p2, line.color };
    int a = p2.coord.y - p1.coord.y;
    int b = p1.coord.x - p2.coord.x;
    int c = -(int)(p1.coord.x * a + p1.coord.y * b);
    while (p1.coord.x <= p2.coord.x && p1.coord.y <= p2.coord.y) {
        SetPixel(image, p1);
        float f1, f2;
        f1 = a * (p1.coord.x + 1) + b * p1.coord.y + c;
        f2 = a * p1.coord.x + b * (p1.coord.y + 1) + c;
        if (abs(f1) < abs(f2)) {
            p1.coord.x++;
        } else {
            p1.coord.y++;
        }
    }
}

void DrawRectangle(Image_U & image, const Line_I & line) {
    Point_U p1 = line.p1,
        p2 = { line.p2.x, line.p1.y },
        p3 = { line.p1.x, line.p2.y },
        p4 = line.p2;
    DrawLine(image, { p1, p2, line.color });
    DrawLine(image, { p2, p4, line.color });
    DrawLine(image, { p1, p3, line.color });
    DrawLine(image, { p3, p4, line.color });
}

size_t radiusError(const Point_U& pos, size_t radius) {
    return abs((long)(pos.x * pos.x + pos.y * pos.y - radius * radius));
}

void draw8Dots(Image_U& image, const Point_U& pos, const Point_U& c, const int color) {
    SetPixel(image, { c.x + pos.x, c.y + pos.y, color });
    SetPixel(image, { c.x - pos.x, c.y + pos.y, color });
    SetPixel(image, { c.x - pos.x, c.y - pos.y, color });
    SetPixel(image, { c.x + pos.x, c.y - pos.y, color });
    SetPixel(image, { c.x + pos.y, c.y + pos.x, color });
    SetPixel(image, { c.x - pos.y, c.y + pos.x, color });
    SetPixel(image, { c.x - pos.y, c.y - pos.x, color });
    SetPixel(image, { c.x + pos.y, c.y - pos.x, color });
}

void DrawCircle(Image_U & image, const Circle_I & circle) {
    Point_U start = { circle.radius, 0 };
    while (start.x >= start.y) {
        draw8Dots(image, start, circle.center, circle.color);
        if (radiusError({ start.x, start.y + 1 }, circle.radius)
        > radiusError({ start.x - 1, start.y + 1 }, circle.radius)) {
            start.x--;
        }
        start.y++;
    }
}

void updateMaxValue(Image_U & image) {
    uchar max = image.data[0][0];
    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            max = max > image.data[i][j] ? max : image.data[i][j];
        }
    }
    image.maxVal = max;
}

void giveEmSomePepper(Image_U & image, float percent) {
    int probability = (int)(100 - percent) * 10;
    std::mt19937 gen((unsigned int)time(0));
    std::uniform_int_distribution<size_t> dist{ 0, 1000 };

    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            image.data[i][j] = dist(gen) > probability
                ? (dist(gen) > 500 ? image.maxVal : 0)
                : image.data[i][j];
        }
    }

}

bool GetGaussianKernel(Image_F & gauss, float sigma) {
    if (gauss.size.x != gauss.size.y || !gauss.data) {
        return false;
    }
    int r = gauss.size.x / 2;
    float sum = 0.0f;
    float s = 1 / (2 * sigma * sigma);//This value is used in gaussian distribution
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            float dist = (float)sqrt(i * i + j * j);
            gauss.data[i + r][j + r] = s / PI_F * exp(-dist * s);
            sum += gauss.data[i + r][j + r];
        }
    }
    //normalization
    for (size_t i = 0; i < gauss.size.y; i++) {
        for (size_t j = 0; j < gauss.size.x; j++) {
            gauss.data[i][j] /= sum;
        }
    }
    return true;
}

bool GaussianBlur(Image_U& image, const Image_F & gauss) {
    Image_U copy;
    copyImage(image, copy);
    int half = gauss.size.x / 2;
    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            float val = 0;
            for (int y = -half; y <= half; y++) {
                for (int x = -half; x <= half; x++) {
                    if (i + y < 0 || j + x < 0
                        || i + y >= image.size.y || j + x >= image.size.x) {
                        val += copy.data[i][j] * gauss.data[y + half][x + half];
                    } else {
                        val += copy.data[i + y][j + x] * gauss.data[y + half][x + half];
                    }
                }
            }
            image.data[i][j] = (uchar)val;
        }
    }
    free_memory(copy);
    return true;
}

uchar getMedian(Image_U& image, size_t i, size_t j, int radius, uchar *arr) {
    size_t len = 0;
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (i + y >= 0 && j + x >= 0
                && i + y < image.size.y && j + x < image.size.x){
                arr[len++] = image.data[i + y][j + x];
            }
        }
    }
    fsort(arr, len);
    if (len % 2) {
        return (arr[len / 2] + arr[len / 2 - 1]) / 2;
    } else {
        return arr[len / 2];
    }
    
}

bool MedianBlur(Image_U & image, size_t radius) {
    Image_U copy;
    copyImage(image, copy);
    //arr is allocated once and used in all steps of bluring
    size_t size = (2 * radius + 1) * (2 * radius + 1);
    uchar *arr = new uchar[size + 1];
    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            image.data[i][j] = getMedian(copy, i, j, radius, arr);
        }
    }
    delete[] arr;
    free_memory(copy);
    return true;
}

bool Median(const Image_U & image, float & median) {
    int size = image.size.x * image.size.y;
    uchar   *arr = new uchar[size + 1];
    for (int i = 0; i < size; i += image.size.x) {
        memcpy(&arr[i], image.data[i / image.size.x], sizeof(uchar) * image.size.x);
    }
    fsort(arr, size);
    if (size % 2) {
        median = (arr[size / 2] + arr[size / 2 - 1]) / 2.0f;
    } else {
        median = arr[size / 2];
    }
    delete[] arr;
    return true;
}

bool Average(const Image_U & image, float & average) {
    float sum = 0.0f;
    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            sum += image.data[i][j];
        }
    }
    average = sum / (image.size.x * image.size.y);
    return true;
}

bool Mode(const Image_U & image, float & mode) {
    int     counter[256] = { 0 }; //max uchar value

    for (size_t i = 0; i < image.size.y; i++) {
        for (size_t j = 0; j < image.size.x; j++) {
            counter[image.data[i][j]]++;
        }
    }
    int max = counter[0];
    for (int i = 0; i < 256; i++) {
        max = max > counter[i] ? max : counter[i];
    }
    for (int i = 0; i < 256; i++) {
        if (counter[i] == max) {
            mode = i;
            return true;
        }
    }
    return true;
}