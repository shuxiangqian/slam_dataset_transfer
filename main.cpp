#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
//    std::cout << "Hello, World!" << std::endl;
    int a = 100;
    char buffer[16];
    int b[5] = {1, 2, 3, 4, 5};
//    ofstream f1("1.txt");
//    if (!f1) {
//        cout << "Failed to create file..." << endl;
//        exit(0);
//    }
//    f1 << a;
//    f1 << a;
//    f1.close();
    //FILE *f1 = fopen("1.txt", "w");
//    if (f1 == nullptr) {
//        cout << "file open filed!" << endl;
//        exit(0);
//    }
    int id = 2;
    for (int i = 0; i < 1; i++) {
        sprintf(buffer, "Data-%d.txt", id);
        id++;
        FILE *f = fopen(buffer, "w");
        cout << "writing..." << endl;
        fwrite(b, sizeof(int), 5, f);
        fclose(f);
        cout << "file closed..." << endl;
    }

    FILE *f1 = fopen("Data-2.txt", "r");
    if (!f1) {
        cout << "Failed to create file..." << endl;
        exit(0);
    }
    cout << "file opening..." << endl;
    vector<int> v;
    for (int i = 0; i < 5; i++) {
        int a;
        fread(&a, sizeof(int), 1, f1);
        v.push_back(a);
    }
    fclose(f1);
    cout << v.size() << endl;
    for (auto it:v)
        cout << it << "  ";
    cout << endl;


    //fwrite(&a, sizeof(int), 100, f1);
//    fclose(f1);
    cout << "file closed..." << endl;

    return 0;
}