# OpenJPEG x OpenCV

A minimal example for integrating OpenJPEG with OpenCV as a replacement for Jasper.


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
- OpenCV >= 3.3.1
- OpenJPEG 2.3.0
- CMake >= 2.8