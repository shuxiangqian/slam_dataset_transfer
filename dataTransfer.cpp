//
// Created by shuxiangqian on 2019-04-01.
#include <iostream>
#include "DataRecorder.h"
#include <cstdio>
#include <vector>

using namespace std;

std::string source_dir = "/Users/shuxiangqian/RKSLAM_NEWR/";
std::string dest_dir = "/Users/shuxiangqian/Code/dataset/";
SensetimeSLAMGravity g_gravity;
SensetimeSLAMImage g_image;
DataRecorder *DataRecorder::data_recorder = nullptr;
DataRecorder *g_recorder = DataRecorder::GetInstance();

//Read Data0-0.txt format data
struct RawDataReader {
    FILE *fp;

    RawDataReader(const std::string file_name) {
        std::string temp = (source_dir + file_name).c_str();
        fp = fopen((source_dir + file_name).c_str(), "rb");
        if (fp == NULL) {
            fprintf(stderr, "%s fopen error!\n", file_name.c_str());
        }
    }

    ~RawDataReader() {
        if (fp) {
            fclose(fp);
            fp = NULL;
        }
    }

    template<typename type>
    void Read(type &data, const int N = 1) {
        fread(&data, sizeof(type), N, fp);
    }
};

void readF(int i) {
    // -----------------------------read-----------------------------
    enum MeasurementType {
        ACCE, GYRO, TYPES
    };
    char buffer[16];
    sprintf(buffer, "Data0-%d.txt", i);
    RawDataReader reader(buffer);
    typedef unsigned char ubyte;
    //===========image info=================
    int width, height;
    double timestamp;

    reader.Read<int>(width);
    reader.Read<int>(height);
    reader.Read<double>(timestamp);
    g_recorder->g_width = width;
    g_recorder->g_height = height;

    //raw image data
    const int image_size = width * height;
    ubyte *img = new ubyte[image_size];
    reader.Read<ubyte>(img[0], image_size);
    g_image.data = img;
    g_image.timeStamp = timestamp;
    // =========IMU info===================
    int nIMU, typeIMU;
    reader.Read<int>(nIMU);
    std::cout << "total " << nIMU << " IMU " << std::endl;

    std::vector<SensetimeSLAMIMU> imus(nIMU);

    for (int i_IMU = 0; i_IMU < nIMU; ++i_IMU) {
        SensetimeSLAMIMU &imu = imus[i_IMU];

        reader.Read<int>(typeIMU);

        if (typeIMU == ACCE) {
            reader.Read<double>(imu.acceleration[0], 3);
            //std::cout << "ACC" << std::endl;
        } else if (typeIMU == GYRO) {
            reader.Read<double>(imu.gyroscope[0], 3);
            //std::cout << "GYRO" << std::endl;
        } else {
            //fprintf(stderr, "imu type error\n");
            //std::cout << "ERROR" << std::endl;
        }
        reader.Read<double>(imu.timeStamp);
    }
    //===========gravity info=================
    SensetimeSLAMAttitude imuAttitude;
    reader.Read<SensetimeSLAMAttitude>(imuAttitude);
    g_gravity.timestamp = imuAttitude.timeStamp;
    g_gravity.data[0] = imuAttitude.gravity[0];
    g_gravity.data[1] = imuAttitude.gravity[1];
    g_gravity.data[2] = imuAttitude.gravity[2];
    //------------------------------write--------------------------------
    g_recorder->WirteImgGrav(g_image, g_gravity);
}

int main() {

    g_recorder->idx = 50;
    g_recorder->dir = dest_dir;
    char buffer[16];
    sprintf(buffer, "Data-%d.txt", 1);
    std::string filename = buffer;
    g_recorder->file = fopen((dest_dir + filename).c_str(), "w");
    // read data form source file
    int start = g_recorder->idx, end = 657;   // 657
    for (int i = start; i <= end; i++) {
        readF(i);
    }
    fclose(g_recorder->file);

    return 0;
}

