//
// Created by shuxiangqian on 2019-03-30.
#include <iostream>
#include <cstdio>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//#define WRITE_FILE

#ifdef  WRITE_FILE
std::string dest_dir = "/Users/shuxiangqian/Code/C++/file/";
FILE *file;

template<typename T>
void write(T *data, int size, long n) {
    fwrite(data, size, n, file);
}

#endif

//Read Data0-0.txt format data
struct RawDataReader {
    FILE *fp;

    RawDataReader(const std::string file_name) {
        std::string temp = (file_name).c_str();
        fp = fopen((file_name).c_str(), "rb");
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

    template<typename T>
    void Read(T *data, int size, const int N = 1) {
        fread(data, size, N, fp);
    }
};

int main() {

    RawDataReader reader("rotat_large.sensors");
//    RawDataReader reader("rotat.sensors");
    unsigned char type;
    double img_time, gtime;
    double img_time_pre = 0.0, gtime_pre = 0.0;
    unsigned char data2[640 * 480] = {0};
    int img_height1, img_width1;
    double gravity[3];
#ifdef WRITE_FILE
    file = fopen("rotat_large.sensors", "w");
    if (!file) {
        cout << "file open filed!" << endl;
    }
#endif

    for (size_t i = 0; i < 10;) {
        // read image
        cout << "i = " << i << endl;
        reader.Read<unsigned char>(&type, sizeof(unsigned char));
        if (type == 0) {
            reader.Read<double>(&img_time, sizeof(double));
            reader.Read<int>(&img_width1, sizeof(int));
            reader.Read<int>(&img_height1, sizeof(int));
            reader.Read<unsigned char>(data2, sizeof(unsigned char), img_width1 * img_height1);
            i++;
#ifdef WRITE_FILE
            if (i == 58 || i == 67 || i == 76 || i == 84 ||i == 92 || i == 101) {
                write(&type, sizeof(unsigned char), 1);
                write(&img_time, sizeof(double), 1);
                write(&img_width1, sizeof(int), 1);
                write(&img_height1, sizeof(int), 1);
                write(data2, sizeof(unsigned char), img_width1 * img_height1);
            }
#endif
            Mat show(img_height1, img_width1, CV_8U, data2);
            Mat temp, dst;
            transpose(show, temp);
            flip(temp, dst, 1);

            double dt = img_time - img_time_pre;
            img_time_pre = img_time;
            cout << setprecision(6) << endl;
            cout << "image dt: " << dt * 1000 << endl;
            cv::imshow("show1", show);
            imshow("show2", dst);
            cv::waitKey(0);
            string fname = to_string(i) + ".png";
            imwrite(fname, dst);
        } else if (type == 18) {
            reader.Read<double>(&gtime, sizeof(double));
            reader.Read<double>(gravity, sizeof(double), 3);
            double dt = gtime - gtime_pre;
            gtime_pre = gtime;
            cout << setprecision(6) << endl;
            cout << "gravity dt: " << dt * 1000 << endl;
#ifdef WRITE_FILE
            if (i == 57 || i == 58 || i == 66|| i == 67|| i == 75 || i == 76 || i == 83|| i == 84|| i == 91 || i == 92 || i == 100 || i == 101) {
                write(&type, sizeof(unsigned char), 1);
                write(&gtime, sizeof(double), 1);
                write(gravity, sizeof(double), 3);
            }
#endif
        } else {
            cout << "type error!" << endl;
        }
//        reader.Read<double>(&time, sizeof(double));
//        reader.Read<int>(&img_width1, sizeof(int));
//        reader.Read<int>(&img_height1, sizeof(int));
//        reader.Read<unsigned char>(data1, sizeof(unsigned char), img_width1 * img_height1);
//        cout << "type = " << type << endl;
//        cout << "time = " << setprecision(12) << time << endl;
//        cout << "img_width = " << img_width1 << endl;
//        cout << "img_height = " << img_height1 << endl;
        // read gravity
//        reader.Read<unsigned char>(&type, sizeof(unsigned char));
//        reader.Read<double>(&gtime, sizeof(double));
//        reader.Read<double>(gravity, sizeof(double), 3);
//        cout << "gtype = " << type << endl;
//        cout << "gtime = " << setprecision(12) << gtime << endl;
//        cout << "gravity = " << gravity[0] << " " << gravity[1] << " " << gravity[2] << endl;

//        reader.Read<unsigned char>(&type, sizeof(unsigned char));
//        reader.Read<double>(&time, sizeof(double));
//        reader.Read<int>(&img_width2, sizeof(int));
//        reader.Read<int>(&img_height2, sizeof(int));
//        reader.Read<unsigned char>(data2, sizeof(unsigned char), img_width2 * img_height2);
//        cout << "type = " << type << endl;
//        cout << "img_width = " << img_width2 << endl;
//        cout << "img_height = " << img_height2 << endl;

//        Mat show(img_height1, img_width1, CV_8U, data2);
//        Mat show1(img_height2, img_width2, CV_8U, data2);
//        cv::imshow("show", show);
//        cv::imshow("show1", show1);
//        cv::waitKey(0);
    }
#ifdef WRITE_FILE
    fclose(file);
#endif

    return 0;
}