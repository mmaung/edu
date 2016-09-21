#include <iostream>
#include <sstream>
#include <libusb.h>
// #include <pthread.h>
#include <string.h>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#if 0
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

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

// assume the output is left and right camera images
const int GETDEFAULTOUTPUT_CMD_ID = 4;

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

void displayAndSaveCamImages(libusb_device_handle **dev_handle) {
  const int FRAME_WIDTH = 640;
  const int FRAME_HEIGHT = 480;
  
  unsigned char cmdHeader[512];
  unsigned char imageParam[FRAME_WIDTH * FRAME_HEIGHT * 2];
  int cmdId = GETDEFAULTOUTPUT_CMD_ID;
  int paramSize = 512;
  int key = 0;
  unsigned int saveCounter = 0;
  
  // namedWindow("Disparity", WINDOW_NORMAL);
  
  do {
    memcpy(&cmdHeader[0], &cmdId, sizeof(cmdId));
    memcpy(&cmdHeader[4], &paramSize, sizeof(paramSize));
    writeToUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_OUT), cmdHeader, 512);
    writeToUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_OUT), imageParam, 512);
    readFromUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_IN), cmdHeader, 512);
    readFromUsb(dev_handle, (TRANSFER_EP | LIBUSB_ENDPOINT_IN), imageParam, FRAME_WIDTH * FRAME_HEIGHT * 2);
  
    Mat leftCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, &imageParam[0]);
    Mat rightCam(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1, &imageParam[FRAME_WIDTH * FRAME_HEIGHT]);
    imshow("Left", leftCam);
    imshow("Right", rightCam);
    
    //-- And create the image in which we will save our disparities
    Mat imgDisparity16S = Mat(leftCam.rows, leftCam.cols, CV_16S);
    Mat imgDisparity8U = Mat(leftCam.rows, leftCam.cols, CV_8UC1);

    //-- 2. Call the constructor for StereoBM
    int ndisparities = 16 * 5;   /**< Range of disparity */
    int SADWindowSize = 21; /**< Size of the block window. Must be odd */

    Ptr<StereoBM> sbm = StereoBM::create(ndisparities, SADWindowSize);

    //-- 3. Calculate the disparity image
    sbm->compute(leftCam, rightCam, imgDisparity16S);

    //-- Check its extreme values
    double minVal; double maxVal;

    minMaxLoc(imgDisparity16S, &minVal, &maxVal);

    // printf("Min disp: %f Max value: %f \n", minVal, maxVal);

    //-- 4. Display it as a CV_8UC1 image
    // imgDisparity16S.convertTo(imgDisparity8U, CV_8UC1, 255 / (maxVal - minVal));
    imgDisparity16S.convertTo(imgDisparity8U, CV_8UC1);
    imshow("Disparity", imgDisparity8U);
  
    key = waitKey(100);
      
    if((key & 0xff) == 's') {
      stringstream ssLeft;
      ssLeft << "leftCam_" << saveCounter << ".png";
      stringstream ssRight;
      ssRight << "rightCam_" << saveCounter << ".png";
      imwrite(ssLeft.str(), leftCam);
      imwrite(ssRight.str(), rightCam);
      saveCounter++;
      cout << "saved image\n";
    }
    else {
      cout << "key is " << (key & 0xff) << "\n";
    }
  } while((key & 0xff) != 27);  // key isn't escape
}

int main()
{
  libusb_device_handle *dev_handle;
  libusb_context *context = NULL;
  // int err;

  #if 0
  // init usb
  libusb_init(&context);
  dev_handle = libusb_open_device_with_vid_pid(context, MOVIDIUS_DEV_VID, MOVIDIUS_DEV_PID);
  if(dev_handle == NULL) {
    std::cout << "Cannot open Movidius device\n";
  }
  else {
    err = libusb_claim_interface(dev_handle, DEV_INTERFACE);
    if (err < 0) {
      std::cout << "Cannot claim Movidius device\n";
    }
  }
  #endif
  initMoviUsbDev(&dev_handle, &context);
  
  displayAndSaveCamImages(&dev_handle);

  #if 0
  // close the usb
  err = libusb_release_interface(dev_handle, DEV_INTERFACE);
  if (err != 0) {
    std::cout << "Cannot release USB interface\n";
  }
  else {
    std::cout << "USB interface released\n";
    libusb_close(dev_handle);
    libusb_exit(context);
  }
  #endif
  exitMoviUsb(&dev_handle, &context);

  std::cout << CV_VERSION << std::endl;

  return 0;
}

