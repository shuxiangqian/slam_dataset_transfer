//
// Created by shuxiangqian on 2019-04-01.
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "DataRecorder.h"
#include <cstdio>
#include <vector>

using namespace std;
using namespace cv;

std::string source_dir = "/Users/shuxiangqian/Code/dataset/";
SensetimeSLAMGravity g_gravity;
DataRecorder *DataRecorder::data_recorder = nullptr;

//Read Data0-0.txt format data
struct RawDataReader {
    FILE *fp;

    RawDataReader(const std::string file_name) {
        fp = fopen((source_dir + file_name).c_str(), "rb");
        if (fp == nullptr) {
            fprintf(stderr, "%s fopen error!\n", file_name.c_str());
        }
    }

    ~RawDataReader() {
        if (fp) {
            fclose(fp);
            fp = nullptr;
        }
    }

    template<typename type>
    void Read(type &data, const int N = 1) {
        fread(&data, sizeof(type), N, fp);
    }
};

void readF(int i) {
    // -----------------------------read-----------------------------
    char buffer[16];
    sprintf(buffer, "Data-%d.txt", i);
    RawDataReader reader(buffer);
    typedef unsigned char ubyte;
    //===========image info=================
    ubyte type;
    int width, height;
    double timestamp;

    reader.Read<ubyte>(type);
    reader.Read<double>(timestamp);
    reader.Read<int>(width);
    reader.Read<int>(height);

    //raw image data
    const int image_size = width * height;
    ubyte *img = new ubyte[image_size];
    reader.Read<ubyte>(img[0], image_size);
//    cout << "image info is:" << endl;
//    cout << "time: " << setprecision(12) << timestamp << endl;
//    cout << "type: " << type << endl;
//    cout << "width: " << width << endl;
//    cout << "height: " << height << endl;
    //===========gravity info=================
    ubyte gtype;
    double gtime;
    double gdata[3];
    reader.Read<ubyte>(gtype);
    reader.Read<double>(gtime);
    reader.Read<double>(gdata[0], 3);
//    cout << "gravity info is:" << endl;
//    cout << "time: " << setprecision(12) << gtime << endl;
//    cout << "type: " << gtype << endl;
//    cout << "data: " << gdata[0] << " " << gdata[1] << " " << gdata[2] << endl;

    Mat show(height, width, CV_8U, img);
    cv::imshow("show", show);
    cv::waitKey(120);
}

int main() {

    // read data form source file
    int start = 50, end = 100;   // 657
    for (int i = start; i <= end; i++) {
        cout << "open " << i << "th image" << endl;
        readF(i);
//        cout << "open " << i << "th image" << endl;
    }

    return 0;
}
