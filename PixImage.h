//
// Created by 李鑫烨 on 2017/12/22.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cassert>
#include <cmath>

using namespace std;
using namespace cv;

#define SCALE 4

#ifndef PIXIMAGE_PIXIMAGE_H
#define PIXIMAGE_PIXIMAGE_H

class PixImage {
private:
    Mat image;
public:
    PixImage(char* fileName);
    Mat Bluring(int n);
    Mat Sobel();
};

PixImage::PixImage(char* fileName) {
    image = imread(fileName);
}

Mat PixImage::Bluring(int n) {
    assert(n != 0);
    assert(n % 2 == 1);
    assert(n <= image.cols && n <= image.rows);
    Mat imageNew;
    int radius = (n - 1)/2;
    image.copyTo(imageNew);
    for(int i = 0; i < imageNew.rows; i++) {
        for(int j = 0; j < imageNew.cols * imageNew.channels(); j++) {
            int sum = 0, num = 0;
            for(int r = i - radius; r <= i + radius; r++) {
                if(r < 0 || r >= image.rows) continue;
                for(int c = j - radius * image.channels();
                    c <= j + radius * image.channels(); c += image.channels()) {
                    if(c < 0 || c >= image.cols * image.channels()) continue;
                    sum += image.ptr<unsigned char>(r)[c];
                    num++;
                }
            }
            imageNew.ptr<unsigned char>(i)[j] = (unsigned char)(sum/num);
        }
    }
    return imageNew;
}

Mat PixImage::Sobel() {
    int b = 0, g = 0, r = 0;
    int channels = image.channels();
    //imshow("[img]", image);
    Mat imageGrey;
    image.copyTo(imageGrey);
    for(int i = 0; i < image.rows; i++) {
        double grey = 0;
        for(int j = 0; j < image.cols ; j++) {
            unsigned char blue = image.ptr<unsigned char>(i)[j * channels];
            unsigned char green = image.ptr<unsigned char>(i)[j * channels + 1];
            unsigned char red = image.ptr<unsigned char>(i)[j * channels + 2];
            b += blue;
            g += green;
            r += red;
            grey = 0.11 * blue + 0.59 * green + 0.30 * red;
            imageGrey.ptr<unsigned char>(i)[j * channels] = (unsigned char)floor(grey);
            imageGrey.ptr<unsigned char>(i)[j * channels + 1] = (unsigned char)floor(grey);
            imageGrey.ptr<unsigned char>(i)[j * channels + 2] = (unsigned char)floor(grey);
        }
    }
    //imshow("[imgGrey]", imageGrey);

    unsigned char mean = (unsigned char)floor(((b * 0.11 + g * 0.59 + r * 0x30)/(image.rows * image.cols)));
    unsigned char thresh = (unsigned char)sqrt(SCALE * mean);

    Mat imageNew;
    image.copyTo(imageNew);

    for(int i = 0; i < imageGrey.rows; i++) {
        for(int j = 0; j < imageGrey.cols; j++) {
            if(i == 0 || i == imageGrey.rows - 1 || j == 0 || j == imageGrey.cols - 1) {
                imageNew.ptr<unsigned char>(i)[j * channels] = 0;
                imageNew.ptr<unsigned char>(i)[j * channels + 1] = 0;
                imageNew.ptr<unsigned char>(i)[j * channels + 2] = 0;
            }
            else {
                int Gx = imageGrey.ptr<unsigned char>(i + 1)[channels * (j - 1)] +
                         2 * imageGrey.ptr<unsigned char>(i + 1)[channels * j] +
                         imageGrey.ptr<unsigned char>(i + 1)[channels * (j + 1)] -
                         imageGrey.ptr<unsigned char>(i - 1)[channels * (j - 1)] -
                         2 * imageGrey.ptr<unsigned char>(i - 1)[channels * j] -
                         imageGrey.ptr<unsigned char>(i - 1)[channels * (j + 1)];
                int Gy = imageGrey.ptr<unsigned char>(i - 1)[channels * (j - 1)] +
                         2 * imageGrey.ptr<unsigned char>(i)[channels * (j - 1)] +
                         imageGrey.ptr<unsigned char>(i + 1)[channels * (j - 1)] -
                         imageGrey.ptr<unsigned char>(i - 1)[channels * (j + 1)] -
                         2 * imageGrey.ptr<unsigned char>(i)[channels * (j + 1)] -
                         imageGrey.ptr<unsigned char>(i + 1)[channels * (j + 1)];
                unsigned char G = sqrt(Gx * Gx + Gy * Gy);
                if(G >= thresh) {
                    imageNew.ptr<unsigned char>(i)[j * channels] = 255;
                    imageNew.ptr<unsigned char>(i)[j * channels + 1] = 255;
                    imageNew.ptr<unsigned char>(i)[j * channels + 2] = 255;
                }
                else {
                    imageNew.ptr<unsigned char>(i)[j * channels] = 0;
                    imageNew.ptr<unsigned char>(i)[j * channels + 1] = 0;
                    imageNew.ptr<unsigned char>(i)[j * channels + 2] = 0;
                }
            }
        }
    }
    //imshow("[edge]", imageNew);
    return imageNew;
}

#endif //PIXIMAGE_PIXIMAGE_H
