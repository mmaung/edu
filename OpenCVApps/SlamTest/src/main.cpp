#include <cstddef>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image1;
  Mat image2;
  image1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  image2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);

  if(argc != 3 || !image1.data || !image2.data) {
    printf("No image data \n");
    return -1;
  }

#if 0
  namedWindow("Image 1", WINDOW_AUTOSIZE);
  imshow("Image 1", image1);

  namedWindow("Image 2", WINDOW_AUTOSIZE);
  imshow("Image 2", image2);
#endif

  vector<KeyPoint> results;
  FAST(image1, results, 80, true, FastFeatureDetector::TYPE_9_16);

  // printf("keypoint count: %d\n", (int)results.size());
  Mat keyPtImage1;
  drawKeypoints(image1, results, keyPtImage1);

  stringstream keyPtCountText;
  keyPtCountText << "#Keypoints: " << results.size();
  const int fontFace = cv::FONT_HERSHEY_SIMPLEX;
  const double fontScale = 1;
  const int thickness = 2;
  const Size keyPtTextSize = getTextSize(keyPtCountText.str(), fontFace, fontScale, thickness, nullptr);
  putText(keyPtImage1, keyPtCountText.str(), Point(0, keyPtTextSize.height),
          fontFace, fontScale, Scalar(0, 0, 255), thickness);

  namedWindow("Image 1", WINDOW_AUTOSIZE);
  imshow("Image 1", keyPtImage1);

  cv::Mat result;
  auto extractor = xfeatures2d::FREAK::create();
  extractor->compute(image1, results, result);

  waitKey(0);

  return 0;
}
