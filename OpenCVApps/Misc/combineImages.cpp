#include <stdio.h>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage: combineImages <last-image-number>\n");
    return -1;
  }

  for(int i = 0; i <= atoi(argv[1]); i++) {
    stringstream ssLeftImg;
    ssLeftImg << "../LeftImages/" << i << ".png";
    stringstream ssRightImg;
    ssRightImg << "../RightImages/" << i << ".png";
    Mat im1 = imread(ssLeftImg.str());
    Mat im2 = imread(ssRightImg.str());
    Size sz1 = im1.size();
    Size sz2 = im2.size();
    Mat im3(sz1.height, sz1.width+sz2.width, CV_8UC3);
    Mat left(im3, Rect(0, 0, sz1.width, sz1.height));
    im1.copyTo(left);
    Mat right(im3, Rect(sz1.width, 0, sz2.width, sz2.height));
    im2.copyTo(right);
    stringstream ssCombinedFile;
    ssCombinedFile << "../CombinedImages/cam" << i << ".png";
    imwrite(ssCombinedFile.str(), im3);
  }

  return 0;
}
