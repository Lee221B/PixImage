#include "PixImage.h"

int main() {
    char fileName[100] = "1.jpg";
    PixImage* pix1 = new PixImage(fileName);
    Mat imageSobel = pix1->Sobel();
    Mat imageBlur = pix1->Bluring(7);
    imwrite("1Sobel.jpg", imageSobel);
    imwrite("1Blur.jpg", imageBlur);
    waitKey(0);
    return 0;
}