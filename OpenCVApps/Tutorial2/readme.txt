Using OpenCV with Eclipse (plugin CDT):

By creating a project directly:

1. start eclipse
2. File -> New -> C/C++ Project
3. Type in a name for project
4. Empty Project
5. default everything else
6. Finish
7. project should appear in the Project Navigator
8. add a source file
  8.1. right-click on <project> in the navigator. New -> Folder
  8.2. name the folder src, hit Finish
  8.3. right-click on src folder. New source file
  8.4. name the source file, hit Finish
9. set OpenCV headers and libraries paths
  9.1. Project->Properties
  9.2. C/C++ Build, Settings, Tool Settings Tab
  9.3. in GCC C++ Compiler, go to Includes, in Include paths (-l), add /usr/local/include/opencv
    9.3.1. if you don't know where opencv files are, run pkg-config --cflags opencv in a terminal
  9.4. in GCC C++ Linker
    9.4.1. in Library search path (-L), add /usr/local/lib
    9.4.2. in Libraries (-l), add opencv_core, opencv_imgproc, opencv_highgui, opencv_ml, opencv_ml, opencv_video, opencv_features2d
           opencv_calib3d, opencv_objdetect, opencv_contrib, opencv_legacy, opencv_flann
    9.4.3. if you don't know where opencv libraries are, run pkg-config --libs opencv in a terminal
10. Project -> Build all

To run the executable inside the Eclipse:

1. Run -> Run Configurations
2. under C/C++ Application, you should see the name of your executable + Debug (if not double click on C/C++ Application)
3. select the name
4. Arguments tab, write the path of the image file (path relative to the workspace/DisplayImage folder)
5. Apply
