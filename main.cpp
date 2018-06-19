/*
 * File : main.cpp
 *
 * License : MIT
 * Date : 06/2018 (MM/YYYY)
 * Author : Rémi Ratajczak
 *
 * ------------------------------------------------------------------
 *                     Why this file?
 * ------------------------------------------------------------------
 * This sample code is a minimal example for :
 * 
 * 1) using OpenJPEG for decoding .jp2 files - official demonstration 
 * files for OpenJPEG are great but they are far from being "minimal 
 * working versions" of themselves, which complicates the adoption of 
 * OpenJPEG for beginners ; plus the docs of OpenJPEG are still a 
 * work in progress.
 * 
 * 2) integrating OpenJPEG with OpenCV in the hope to, one day, 
 * replace Jasper with OpenJPEG in OpenCV - see Issues on 
 * OpenCV's Github.
 *
 * ------------------------------------------------------------------
 *                     What does it do?
 * ------------------------------------------------------------------
 * In summary, it demonstrates how to read and decode a .jp2 file 
 * using OpenJPEG. It also demonstrates how to access OpenJPEG data 
 * using simple pointers arithmetics. Finally, it demonstrates how 
 * to painlessly copy OpenJPEG data in an OpenCV cv::Mat. The cv::Mat
 * is further written as a .jpg file on the disk using the OpenCV 
 * cv::imwrite method to complete the use case demonstration.
 *
 * Please, be aware that the current version of OpenCV (3.4.1 at the
 * time) is still using Jasper which is not anymore able to handle 
 * .jp2 files properly. Therefore, if you want to write a .jp2 file 
 * from a  cv::Mat, you should better rely on the OpenJPEG encoders 
 * (not demonstrated in this file for the moment).
 *
 * ------------------------------------------------------------------
 *                     Dependencies
 * ------------------------------------------------------------------
 * OpenCV >= 3.3.1
 * OpenJPEG 2.3.0
 * CMake >= 2.8
 * 
 *
 */

////////////////////////////////////////////////////////////////////
// File includes:
#include <opencv2/imgproc.hpp> //OpenCV
#include <opencv2/highgui.hpp> //OpenCV
#include <opencv2/imgcodecs.hpp> //OpenCV
#include <opencv2/core/utility.hpp> //OpenCV
#include <iostream> //stl
#include "openjp2\openjpeg.h" //OpenJPEG
#include "openjp2\opj_common.h" //OpenJPEG
#include "opj_apps_config.h" //OpenJPEG
#include "opj_config.h" //OpenJPEG

////////////////////////////////////////////////////////////////////
// Useful namespaces
using namespace cv; 
using namespace std;

////////////////////////////////////////////////////////////////////
// Show debug messages or not
bool DEBUG = true;


////////////////////////////////////////////////////////////////////
// Utility for error messages
int error_message(std::string message)
{
    std::cerr << message << std::endl;
    return false;
}

////////////////////////////////////////////////////////////////////
// Debug messages for an opj_image object
void debug_messages_opj_img(opj_image* opj_img)
{
    if (opj_img != NULL && opj_img != nullptr) 
    {
        std::cout << "width : " << opj_img->x0 << " to " << opj_img->x1 << std::endl;
        std::cout << "height : " << opj_img->y0 << " to " << opj_img->y1 << std::endl;
        std::cout << "numcomps (channels) : " << opj_img->numcomps << std::endl;
        std::cout << "colorspace : " << opj_img->color_space << std::endl;
    }
}


////////////////////////////////////////////////////////////////////
// Initialize our opj_parameters for a decoder (opj_dparameters)
// Parameters are commidity classes / structs to store data in memory
void set_default_parameters(opj_dparameters* parameters)
{
    if (parameters) {
        memset(parameters, 0, sizeof(opj_dparameters));

        /* default decoding parameters (command line specific) */
        parameters->decod_format = -1;
        parameters->cod_format = -1;

        /* default decoding parameters (core) */
        opj_set_default_decoder_parameters(parameters);
    }
}

////////////////////////////////////////////////////////////////////
// Stream and decode an .jp2 file, store it in a cv::Mat passed as 
// reference
bool getJP2Image(const char* pathToImgWithExtension, cv::Mat & opcv_output)
{
    // [x] Check with only opencv 331 compiled in 32 bits (x86) with msvc15 using Visual Studio Community 2017 on Windows 10 and linked through CMake- passed
    // [x] Check with opencv 331 and openjpeg 230 both compiled in 32 bits (x86) with msvc12 using Visual Studio Community 2017 on Windows 10 and both linked through CMake - passed
    //      - Needed to modify the macro in the target_link_libraries instruction in CMakeFile from ${OPENJPEG_LIBRARIES} to path on disk 
    //        (akka : "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/*.lib") to compile
    //      - Needed to modify the macro in the include_directories instruction in CMakeFiles from ${OPENJPEG_BINARY_DIR} to path on disk
    //        (akka : "D:/lib/openjpeg/build86/src/lib/" ; also included "D:/lib/openjpeg/build86/src/bin/") - did the same with ${OPENJPEG_SOURCE_DIR}

    /* Sample OpenJpeg imread image */
    opj_dparameters* mparameters = new opj_dparameters();
    std::strcpy(mparameters->infile, pathToImgWithExtension);

    /* Open a stream on a .jp2 image */
    opj_stream_t* mopj_stream_fname = NULL; /* Stream */
    mopj_stream_fname = opj_stream_create_default_file_stream(mparameters->infile, true);
    if (!mopj_stream_fname) {
        fprintf(stderr, "ERROR -> failed to create the stream from the file %s\n", mparameters->infile);
        return false;
    }

    /* Create a decompressor codec */
    opj_codec_t* mopj_codec = NULL; /* Handle to a decompressor */
    mopj_codec = opj_create_decompress(OPJ_CODEC_JP2); /* select JP2 codec for .jp2 files */
    opj_setup_decoder(mopj_codec, mparameters);  /* init the decoder */
    opj_codec_set_threads(mopj_codec, 4); /* tell the decoder to work on 4 threads */

    /* Decode an image with .jp2 format */
    opj_image_t* mopj_img = NULL; /* output image */
    if (!opj_read_header(mopj_stream_fname, mopj_codec, &mopj_img)) /* you need to read the header before decoding */
        return error_message("Unable to read_header in main.");
    if (!opj_decode(mopj_codec, mopj_stream_fname, mopj_img)) /* decoding */
        return error_message("Unable to decode in main.");
    if (DEBUG) debug_messages_opj_img(mopj_img);

    /* Copy decoded data in cv::Mat using pointers arithmetics  - note that we are copying the data to avoid segfaults */
    /* NB : as this is a minimal example, we assume that the .jp2 file represents a grayscale image - a bit more work would be involved to handle multichannels images */
    int width = mopj_img->x1;
    int height = mopj_img->y1;
    int channels = mopj_img->numcomps;
    opcv_output = cv::Mat(cv::Size(width, height), CV_8UC1); /* later, we will need to cast OPJ_INT32 data to uchar */
    for(int channel = 0; channel < channels; channel++) /* iterate through the image pixels stored in the components data of an opj_image pointer */
        for (int col = 0; col < width; col++)
            for (int row = 0; row < height; row++)
                opcv_output.at<uchar>(cv::Point(col, row)) = (uchar)*(mopj_img->comps->data++); /* we assume file is continuous on memory ; be aware of the cast to uchar */

    /* Clean memory */
    if (mparameters != NULL &&
        mparameters != nullptr) delete mparameters;
    if (mopj_stream_fname != NULL &&
        mopj_stream_fname != nullptr) delete mopj_stream_fname;
    if (mopj_codec != NULL &&
        mopj_codec != nullptr) delete mopj_codec;
    if (mopj_img != NULL &&
        mopj_img != nullptr) delete mopj_img;

    return true;
}

////////////////////////////////////////////////////////////////////
// Main
int main(void)
{
    cv::Mat result; /* create our cv::Mat */
    if ( !getJP2Image("../data/trial.jp2", result) ) /* load .jp2 file in it */
        return -1; 
    if ( !result.empty() ) 
        cv::imwrite("../data/res.jpg", result); /* save it as a .jpg file */
    return 0;
}
