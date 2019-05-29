//
// Created by shuxiangqian on 2019-03-30.
//
#include <iostream>
#include <math.h>
#include <list>

using namespace std;

int main() {
    list<double> l;
    l.push_back(1.1);
    l.push_back(2.22);
    l.push_back(3.08);
    l.push_back(4.2);
    l.push_back(5.13);

    double t1 = 1.6;

    double time = l.front();
    list<double>::iterator it = l.begin();
    double dt1 = fabs(*it - t1);
    it++;
    for (; it != l.end(); it++) {
        double dt2 = fabs(*it - t1);
        if (dt2 > dt1)
            continue;
        time = *it;
        dt1 = dt2;
    }
    cout << "time = " << time << endl;

    return 0;
}
