#ifndef _Image_h
#define _Image_h

#define WIDTH  768 
#define HEIGHT 256

#include <fstream>
#include <cmath>

#endif

using namespace std;
typedef unsigned char unchar;

class Image {
    ifstream* pInFile; 
    ofstream* pOutFile;
    unchar imageHeaderData[1078]; //.bmp header data with offset 1078.
public:
    unchar** imageData;
    unsigned char* data;
    Image(const char* fileName);
    void ReadBMP(const char* filename);
    ~Image();
};

Image::Image(const char* fileName){
    imageData    = new unchar* [HEIGHT]; // create new array size: height of image.

    for (int i = 0; i < HEIGHT; i++) {
        imageData[i]    = new unchar [WIDTH]; //create matrix.
    }

    //image I/O
    pInFile = new ifstream;
    pInFile->open(fileName, ios::in | ios::binary); // open fileName and read as binary.
    pInFile->seekg(0, ios::beg); //pos filter at beginning of image file.
    pInFile->read(reinterpret_cast<char*>(imageHeaderData),1078); //read bmp header data into array.

    for(int i=0; i<HEIGHT; i++) {
        pInFile->read(reinterpret_cast<char*>(imageData[i]),WIDTH);//read row into each array entry.
    }

    pInFile->close(); //close stream.
    // ReadBMP(fileName);

}

void Image::ReadBMP(const char* filename) {
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }
}

Image::~Image(){

    delete pInFile;
    delete pOutFile;

    for(int i=0; i<HEIGHT; i++){
        delete[] imageData[i];
    }

    delete[] imageData;
}