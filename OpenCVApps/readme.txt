OpenCV Installation in Linux:

Req Packages:
- GCC 4.4.x or later
- CMake 2.6 or higher
- Git
- GTK+2.x or higher, including headers (libgtk2.0-dev)
- pkg-config
- Python 2.6 or later and Numpy 1.5 or later with developer packages (python-dev, python-numpy)
- ffmpeg or libav development packages: libavcodec-dev, libavformat-dev, libswscale-dev
- [optional] libtbb2 libtbb-dev
- [optional] libdc1394 2.x
- [optional] libjpeg-dev, libpng-dev, libtiff-dev, libjasper-dev, libdc1394-22-dev

- [compiler] sudo apt-get install build-essential
- [required] sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
- [optional] sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

Tried it on Ubuntu 14.04 (9/20/2016):

1. Download OpenCV source code
2. cd ~/opencv
3. mkdir build
4. cd build
5. cmake -D CMAKE_BUILD_TYPE=RELEASE\DEBUG -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=<path-to-opencv_contrib>/modules ..
6. make -j8
7. sudo make install
at least in Ubuntu 14.04:
8. sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
9. sudo ldconfig

Using OpenCV with gcc and CMake:
see Tutorial1 directory

Using OpenCV with Eclipse CDT:
see Tutorial2 directory

Using OpenCV + CMake with Eclipse CDT:
see Tutorial3 directory

