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
    Image(const char* fileName);
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
}

Image::~Image(){

    delete pInFile;
    delete pOutFile;

    for(int i=0; i<HEIGHT; i++){
        delete[] imageData[i];
    }

    delete[] imageData;
}