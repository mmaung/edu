Using CMake + OpenCV with Eclipse (plugin CDT):
1. create a dir
2. inside <dir>, create source files
3. create CMakeLists.txt
4. mkdir build
5. cd build
6. cmake-gui ..
7. configure, and generate
8. make -j4
9. start eclipse, workspace not <dir> or <dir/build>
10. right-click on project explorer, select Import, C/C++ filter, choose Existing Code as a Makefile Project
11. name the project, pick <dir> as the existing code location
12. project properties, C/C++ build, Build directory: ${workspace_loc:/<dir>} to ${workspace_loc:/<dir>}/build
13. OK
14. Project -> Build

to run:
1. Run -> run configuration
2. double click on C/C++ Application, select <project> Default, rename it
3. pick C/C++ Application (e.g. build/helloWorld)

