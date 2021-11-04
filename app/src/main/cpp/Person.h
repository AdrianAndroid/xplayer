//
// Created by 赵健 on 2021/9/28.
//

#ifndef NATIVECPP_PERSON_H
#define NATIVECPP_PERSON_H

#include <iostream>

using namespace std;

class Person {
private:
    int age;
public:
    void setAge(int age) {
        this->age = age;
        cout << "[Person] setAge = " << age << endl;
    }

    void initSDK() {
        cout << "[Person] initSDK()" << endl;
    }

    void releaseSDK() {
        cout << "[Person] releaseSDK" << endl;
    }
};


#endif //NATIVECPP_PERSON_H
