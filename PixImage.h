//
// Created by 李鑫烨 on 2017/12/22.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cassert>
#include <cmath>

using namespace std;
using namespace cv;

#define SCALE 400

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

/* 以文件名为参数的构造函数，
 * 打开fileName文件，
 * 建立image对象
 */
PixImage::PixImage(char* fileName) {
    image = imread(fileName);
}

Mat PixImage::Bluring(int n) {
    assert(n != 0);
    assert(n % 2 == 1);
    assert(n <= image.cols && n <= image.rows);
    Mat imageNew;

    //计算模糊半径
    int radius = (n - 1)/2;
    image.copyTo(imageNew);
    for(int i = 0; i < imageNew.rows; i++) {
        for(int j = 0; j < imageNew.cols * imageNew.channels(); j++) {

            //求得周围radius半径内的像素同通道值得平均值
            //若在图像边缘则只取部分
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

    //先将图像灰度化，存储于imageGrey中
    Mat imageGrey;
    image.copyTo(imageGrey);
    for(int i = 0; i < image.rows; i++) {
        double grey = 0;
        for(int j = 0; j < image.cols ; j++) {
            // blue, green, red为单个像素中B G R各通道的值
            // b g r为整个图像中B G R各通道的值之和
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

    //求得整个图像的平均灰度值mean，
    //通过mean与参数SCALE求得阈值thresh，
    //SCALE可通过图片边缘化效果自行调节
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
                //通过Sobel算子计算像素的纵向与横向系数
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
                //判断综合系数是否超过阈值，即是否为边缘,
                //若超过阈值，置为白色，反之而置为黑色。
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
    return imageNew;
}

#endif //PIXIMAGE_PIXIMAGE_H
