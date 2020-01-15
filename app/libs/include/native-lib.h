#ifndef DARKMARK_NATIVE_LIB_H
#define DARKMARK_NATIVE_LIB_H

#include <stdint.h>
#include <vector>
#include <string>

// creation_time of mp4 is counted from 19040101
#define SECPERHOUR 3600
#define SECPERDAY 86400
#define SECPERYEAR (SECPERDAY * 365)

// 20150101 - 19040101
// ((2015 - 1904) * 365 + 27) * 24 * 60 * 60
#define SEC_BASE 3502828800

// 20170101 - 19040101
// ((2017 - 1904) * 365 + 28) * 24 * 60 * 60
// 少了8个小时，我们在东八区
#define SEC_BASE2 3565987200
#define SEC_BASE3 (SEC_BASE2 - 8*60*60)

//            2017 2018 2019 2020 2021 2022 2023
int year[] = {365, 365, 365, 366, 365, 365, 365};
char y[7][3] = {"17", "18", "19", "20", "21", "22", "23"};
int month0[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int monthA[] ={31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
int monthB[] ={31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
char m[12][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
char d[31][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10",
                 "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                 "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
                 "31"};
char a[] = "0123456789";

int mDataLength;
int mCurrentIndex = 0;
uint8_t *mData;

static union ____indian_swap{
    unsigned char byte[4];
    unsigned int size;
}indian_a , indian_b;



#endif //DARKMARK_NATIVE_LIB_H
