//
// Created by shuxiangqian on 2019-03-30.
#include <iostream>
#include <cstdio>

using namespace std;

#define ROW 20
#define COL 10
std::string dir = "/Users/shuxiangqian/Code/dataTest/";
//const char[20] = ""

class DataRecorder {
private:
    static DataRecorder *data_recorder;
//    FILE *file;

    DataRecorder() {}

public:
    ~DataRecorder();

//    int idx;
    int g_width;               ///< size of process image
    int g_height;              ///< size of process image
    //std::string dir;
    FILE *file;

    static DataRecorder *GetInstance() {
        if (data_recorder == nullptr)
            data_recorder = new DataRecorder();
        return data_recorder;
    }

    void openFile(int i) {
        char buffer[16];
        sprintf(buffer, "Data-%d.txt", i);
        std::string filename = buffer;
        file = fopen((dir + filename).c_str(), "w");
//        file = fopen(buffer,"w");
//        idx++;
    }

    void closeFile() {
        fclose(file);
    }

    template<typename T>
    void write(T *data, int size, int n) {
        fwrite(data, size, n, file);
    }
};

DataRecorder *DataRecorder::data_recorder = nullptr;

int main() {
    unsigned char type = 18;
    double time = 1000;
    unsigned char data[ROW][COL];
    int count = 1;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            data[i][j] = (unsigned char) count;
            count++;
        }
    }

    DataRecorder *d1 = DataRecorder::GetInstance();
    d1->g_height = ROW;
    d1->g_width = COL;
    d1->openFile(1);
//    d1->file = d1->openFile(1);
//    d1->idx = 1;
    //d1->dir = "Users/shuxiangqian/Code/C++/file/data";

    for (int i = 0; i < COL; i++) {
        //d1->openFile(d1->idx);
        //cout << "opening " << d1->idx - 1 << ".txt" << endl;
        d1->write(&type, sizeof(unsigned char), 1);
        d1->write(&time, sizeof(double), 1);
        d1->write(data, sizeof(unsigned char), ROW * COL);
//        d1->closeFile();
        //cout << "closing " << d1->idx - 1 << ".txt" << endl;
        time += 10;
    }
    d1->closeFile();

    return 0;
}