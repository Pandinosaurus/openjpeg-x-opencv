# OpenJPEG x OpenCV

A minimal example for integrating OpenJPEG with OpenCV as a replacement for Jasper.

Update(2020-03-27):  Thanks to the work of StefanBruens and alalek, official OpenCV support for JPEG2000 files is moving from Jasper to OpenJPEG (see PR https://github.com/opencv/opencv/pull/16494 ) !

## Why this file ?
This sample code is a minimal example for :

1) using OpenJPEG for decoding .jp2 files - official demonstration 
   files for OpenJPEG are great but they are far from being "minimal 
   working versions" of themselves, which complicates the adoption of 
   OpenJPEG for beginners ; plus the docs of OpenJPEG are still a 
   work in progress.

2) integrating OpenJPEG with OpenCV in the hope to, one day, 
   replace Jasper with OpenJPEG in OpenCV - see Issues on 
   OpenCV's Github.
   

## What does it do ?

In summary, it demonstrates how to read and decode a .jp2 file 
using OpenJPEG. It also demonstrates how to access OpenJPEG data 
using simple pointers arithmetics. Finally, it demonstrates how 
to painlessly copy OpenJPEG data in an OpenCV cv::Mat. The cv::Mat
is further written as a .jpg file on the disk using the OpenCV 
cv::imwrite method to complete the use case demonstration.

Please, be aware that the current version of OpenCV (3.4.1 at the
time) is still using Jasper which is not anymore able to handle 
.jp2 files properly. Therefore, if you want to write a .jp2 file 
from a  cv::Mat, you should better rely on the OpenJPEG encoders 
(not demonstrated in this file for the moment).

## Dependencies
- OpenCV >= 3.3.1 ( tried with a build from source on Windows with msvc2015 : https://github.com/opencv/opencv )
- OpenJPEG 2.3.0 ( tried with a build from source on Windows with msvc2015 : https://github.com/uclouvain/openjpeg )
- CMake >= 2.8  

## To Install
1) Change the CMakeList according to your install (the hardest part!) 
- Change absolute and relative paths for OpenCV, OpenJPEG, etc.
2) Go to the root directory with command line or clicks. <br> 
``` cd path/to/OpenJPEGxOpenCV/ ```
3) Create a build directory with command line or clicks.<br>
``` mkdir ./build ```
4) Generate project with CMake with command line or GUI (recommended). <br>
```bash cd ./build ``` <br>
``` cmake .. ```
5) Now you need to build your project with your favorite compiler. For this sample, we tried msvc2015 x86 on Windows 10 with the GUI of Visual Studio 2017 in release mode.
