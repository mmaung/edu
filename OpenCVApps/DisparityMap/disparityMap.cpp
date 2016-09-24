#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/contrib/contrib.hpp"
// #include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <libusb.h>
#include <iostream>

using namespace cv;
using namespace std;

const int DEV_INTERFACE = 0;
const int MOVIDIUS_DEV_VID = 0x040E;
const int MOVIDIUS_DEV_PID = 0xF63B;

const int EP1 = 1;
const int EP2 = 2;
const int EP3 = 3;

const int TIMEOUT_MS = 0;;
const int TRANSFER_EP = EP1;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// assume the output is left and right camera images
const int GETDEFAULTOUTPUT_CMD_ID = 5;

void initMoviUsbDev(libusb_device_handle **dev_handle, libusb_context **context) {
  int err;

  err = libusb_init(context);
  if(err < 0) {
    std::cout << "libusb init failure\n";
  }

  *dev_handle = libusb_open_device_with_vid_pid(*context, MOVIDIUS_DEV_VID, MOVIDIUS_DEV_PID);
  if(*dev_handle == NULL) {
    std::cout << "Cannot open Movidius device\n";
  }
  else {
    err = libusb_claim_interface(*dev_handle, DEV_INTERFACE);
    if (err < 0) {
      std::cout << "Cannot claim Movidius device\n";
    }
  }
  
  // std::cout << "init movidius usb device successful\n";
}

void exitMoviUsb(libusb_device_handle **dev_handle, libusb_context **context) {
  int err;

  err = libusb_release_interface(*dev_handle, DEV_INTERFACE);
  if (err != 0) {
    std::cout << "Cannot release USB interface\n";
  }
  else {
    libusb_close(*dev_handle);
    libusb_exit(*context);
  }
  
  // std::cout << "exit movidius usb device\n";
}

void writeToUsb(libusb_device_handle **dev_handle, unsigned char endPt, unsigned char *buf, int bufSize) {
  int status;
  int actual;

  status = libusb_bulk_transfer(*dev_handle, endPt, buf, bufSize, &actual, 0);

  if (status == LIBUSB_ERROR_TIMEOUT) {
    cout << "timeout writing to usb endpoint\n";
  }
  else if (status) {
    cout << "usb write status: " << status << "\n";
  }
  if (actual > 0) {
    // cout << "wrote " << actual << " bytes to usb endpoint\n";
  }
}

void readFromUsb(libusb_device_handle **dev_handle, unsigned char endPt, unsigned char *buf, int bufSize) {
  int status;
  int actual;

  status = libusb_bulk_transfer(*dev_handle, endPt, buf, bufSize, &actual, 0);

  if (status == LIBUSB_ERROR_TIMEOUT) {
    cout << "timeout reading from usb endpoint\n";
  }
  else if (status) {
    cout << "usb read status: " << status << "\n";
  }

  if (actual > 0) {
    // cout << "read " << actual << " bytes from usb endpoint\n";
  }
}

void getCamImages(libusb_device_handle **dev_handle, Mat &leftCam, Mat &rightCam, Mat &disparity) {
  unsigned char cmdHeader[512];
  unsigned char imageParam[FRAME_WIDTH * FRAME_HEIGHT * 3];
  int cmdId = GETDEFAULTOUTPUT_CMD_ID;
  int paramSize = 512;
  int key = 0;
  unsigned int saveCounter = 0;

  // namedWindow("Disparity", WINDOW_NORMAL);

  memcpy(&cmdHeader[0], &cmdId, sizeof(cmdId));
  memcpy(&cmdHeader[4], &paramSize, sizeof(paramSize));
  writeToUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_OUT), cmdHeader, 512);
  writeToUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_OUT), imageParam, 512);
  readFromUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_IN), cmdHeader, 512);
  readFromUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_IN), imageParam, FRAME_WIDTH * FRAME_HEIGHT * 3);

  // Mat leftCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, &imageParam[0]);
  // Mat rightCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, &imageParam[FRAME_WIDTH * FRAME_HEIGHT]);
  memcpy(leftCam.ptr(), &imageParam[0], FRAME_WIDTH * FRAME_HEIGHT);
  memcpy(rightCam.ptr(), &imageParam[FRAME_WIDTH * FRAME_HEIGHT], FRAME_WIDTH * FRAME_HEIGHT);
  memcpy(disparity.ptr(), &imageParam[FRAME_WIDTH * FRAME_HEIGHT * 2], FRAME_WIDTH * FRAME_HEIGHT);
}

int main(int argc, char* argv[])
{
  int key;
  Mat computedDisparity;
  Mat computedDisparity8;
  Mat leftCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1);
  Mat rightCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1);
  Mat givenDisparity8(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1);
  libusb_device_handle *dev_handle;
  libusb_context *context = NULL;
  initMoviUsbDev(&dev_handle, &context);

#if 0
#if 0
  StereoBM sbm;
  sbm.state->SADWindowSize = 9;
  sbm.state->numberOfDisparities = 112;
  sbm.state->preFilterSize = 5;
  sbm.state->preFilterCap = 61;
  sbm.state->minDisparity = -39;
  sbm.state->textureThreshold = 507;
  sbm.state->uniquenessRatio = 0;
  sbm.state->speckleWindowSize = 0;
  sbm.state->speckleRange = 8;
  sbm.state->disp12MaxDiff = 1;
#else
  StereoSGBM sbm;
  sbm.SADWindowSize = 3;
  sbm.numberOfDisparities = 144;
  sbm.preFilterCap = 63;
  sbm.minDisparity = -39;
  sbm.uniquenessRatio = 10;
  sbm.speckleWindowSize = 100;
  sbm.speckleRange = 32;
  sbm.disp12MaxDiff = 1;
  sbm.fullDP = false;
  sbm.P1 = 216;
  sbm.P2 = 864;
#endif
#endif
  Ptr<StereoSGBM> sbm = StereoSGBM::create(-39, 144, 3, 216, 864, 1, 0, 10, 100, 32);

  do {
    getCamImages(&dev_handle, leftCam, rightCam, givenDisparity8);
    sbm->compute(leftCam, rightCam, computedDisparity);
    normalize(computedDisparity, computedDisparity8, 0, 255, CV_MINMAX, CV_8U);
    
    imshow("left", leftCam);
    imshow("right", rightCam);
    imshow("given disparity", givenDisparity8);
    imshow("computed disparity", computedDisparity8);

    key = waitKey(100);
  } while((key & 0xff) != 27);  // key isn't escape

    return(0);
}
