//
// Created by shuxiangqian on 2019-03-30.
#ifndef SENSE_DATARECORDER_H
#define SENSE_DATARECORDER_H

//#include "SensetimeSLAM3.h"
#include <cstdio>
#include <list>
#include <cmath>

typedef struct SensetimeSLAMGravity {
    double data[3];
    double timestamp;
} SensetimeSLAMGravity;

typedef struct SensetimeSLAMImage {
    unsigned char *data;    /* contains the intensity value for each image pixel */
    double timeStamp;       /* timestamp in second */
} SensetimeSLAMImage;

typedef struct SensetimeSLAMAttitude {
    double quaternion[4];    /* attitude of the device */
    double gravity[3];        /* gravity direction */
    double timeStamp;       /* timestamp in second */
    SensetimeSLAMAttitude() : timeStamp(0.0) {};
} SensetimeSLAMAttitude;

typedef struct SensetimeSLAMIMU {
    double acceleration[3];    /* raw acceleration data in gravity unit. */
    double gyroscope[3];    /* raw angular velocity data in radian */
    double timeStamp;       /* timestamp in second */
} SensetimeSLAMIMU;

class DataRecorder {
private:
    static DataRecorder *data_recorder;
    //FILE *file;

    DataRecorder() {}

public:
    ~DataRecorder();

    FILE *file;
    std::list<SensetimeSLAMGravity> g_grav;
    int idx;
    int g_width;               ///< size of process image
    int g_height;              ///< size of process image
    //std::mutex g_imu_lock;
    std::string dir;

    static DataRecorder *GetInstance() {
        if (data_recorder == nullptr)
            data_recorder = new DataRecorder();
        return data_recorder;
    }

    void WriteAllData(const SensetimeSLAMImage &I) {
        openFile(idx);
        // image
        unsigned char type = 0;
        write(&type, sizeof(unsigned char), 1);
        double time = I.timeStamp;
        write(&time, sizeof(double), 1);
        write(&g_width, sizeof(int), 1);
        write(&g_height, sizeof(int), 1);
        write(I.data, sizeof(unsigned char), g_height * g_width);
        // gravity
        WriteGravity(time);
        closeFile();
    }

    void UpdateGravity(const SensetimeSLAMGravity &gravity) {
        g_grav.push_back(gravity);
        if (g_grav.size() > 5)
            g_grav.pop_front();
    }

    void WriteGravity(double img_time) {
        SensetimeSLAMGravity g = g_grav.front();
        double dt = fabs(img_time - g.timestamp);
        std::list<SensetimeSLAMGravity>::iterator it = g_grav.begin();
        it++;
        for (; it != g_grav.end(); it++) {
            double dt1 = fabs(it->timestamp - img_time);
            if (dt1 > dt)
                continue;
            g = *it;
            dt = dt1;
        }

        double time = g.timestamp;
        double *data = g.data;
        unsigned char type = 18;
        write(&type, sizeof(unsigned char), 1);
        write(&time, sizeof(double), 1);
        write(data, sizeof(double), 3);
    }

    void WirteImgGrav(const SensetimeSLAMImage &I, const SensetimeSLAMGravity& g){
        //openFile(idx);
        // -----------------image-----------------
        unsigned char type = 0;
        write(&type, sizeof(unsigned char), 1);
        double time = I.timeStamp;
        write(&time, sizeof(double), 1);
        write(&g_width, sizeof(int), 1);
        write(&g_height, sizeof(int), 1);
        write(I.data, sizeof(unsigned char), g_height * g_width);
        // -----------------gravity-----------------
        time = g.timestamp;
        double gdata[3];
        gdata[0] = g.data[0];
        gdata[1] = g.data[1];
        gdata[2] = g.data[2];
        type = 18;
        write(&type, sizeof(unsigned char), 1);
        write(&time, sizeof(double), 1);
        write(gdata, sizeof(double), 3);
        //closeFile();
    }

    void openFile(int i) {
        char buffer[16];
        sprintf(buffer, "Data-%d.txt", i);
        std::string filename = buffer;
        file = fopen((dir + filename).c_str(), "w");
        idx++;
    }

    void closeFile() {
        fclose(file);
    }

    template<typename T>
    void write(T *data, int size, long n) {
        fwrite(data, size, n, file);
    }
};

#endif //SENSE_DATARECORDER_H
