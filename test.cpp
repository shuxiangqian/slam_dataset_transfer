//
// Created by shuxiangqian on 2019-03-29.
#include <cstdio>
#include <vector>

FILE *file;
int idx = 1;
static int g_width;       ///< size of process image
static int g_height;      ///< size of process image

typedef struct SensetimeSLAMImage {
    unsigned char *data;    /* contains the intensity value for each image pixel */
    double timeStamp;       /* timestamp in second */
} SensetimeSLAMImage;

typedef struct SensetimeSLAMGravity {
    double data[3];
    double timestamp;
} SensetimeSLAMGravity;
SensetimeSLAMGravity g;

void WriteGravity() {

    unsigned char type = 0x12;
    fwrite(&type, sizeof(unsigned char), 1, file);
    fwrite(&g.timestamp, sizeof(double), 1, file);
    fwrite(g.data, sizeof(double), 3, file);
}

void SensetimeSLAMRunWithImage_V3(const SensetimeSLAMImage& I) {
    char buffer[16];
    sprintf(buffer, "Data-%d.txt", idx);
    idx++;
    file = fopen(buffer, "w");
    // image
    unsigned char type = 0x00;
    fwrite(&type, sizeof(unsigned char), 1, file);
    double time = I.timeStamp;
    fwrite(&time, sizeof(double), 1, file);
    fwrite(&g_width, sizeof(int), 1, file);
    fwrite(&g_height, sizeof(int), 1, file);
    fwrite(I.data, sizeof(unsigned char), g_height * g_width, file);
    // gravity
    WriteGravity();
    fclose(file);
}


int main() {
    g_height = 480;
    g_width = 640;
    double data1[3] = {1, 1, 1};
    unsigned char data2[640 * 480] = {1};
    double time = 100;

    SensetimeSLAMImage img;
    img.timeStamp = time;
    img.data = data2;

    g.data[0] = data1[0];
    g.data[1] = data1[1];
    g.data[2] = data1[2];
    g.timestamp = time;

    for (int i = 0; i < 10; i++) {
        SensetimeSLAMRunWithImage_V3(img);
    }
}
