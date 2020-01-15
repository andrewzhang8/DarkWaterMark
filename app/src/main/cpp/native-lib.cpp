#include <jni.h>
#include <string>
#include "../../../libs/include/native-lib.h"

extern "C" {
using namespace std;

#ifdef ANDROID
#include <android/log.h>
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(^_^)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif

unsigned int readUint32Lit(){
    unsigned int k = 0;
    memcpy(&k, mData + mCurrentIndex, 4);    // size =4
    mCurrentIndex += 4;
    indian_a.size = k;
    for(int i = 0, j = 3; i < 4; i++, j--)
        indian_b.byte[i] = indian_a.byte[j];

    return indian_b.size;
}

JNIEXPORT void JNICALL
Java_com_zcf_darkmark_MainActivity_writeDeviceInfo (
        JNIEnv *env, jobject , jstring file, jstring info) {
    LOGE("write begin");

    int len = 0;
    unsigned char p[5];
    unsigned int k = 0;

    const char *fn = env->GetStringUTFChars(file, JNI_FALSE);
    const char *dinfo = env->GetStringUTFChars(info, JNI_FALSE);
    LOGE("fopen");
    LOGE("device = %s", dinfo);
    FILE *fp = fopen(fn, "rb+");
    if (fp == NULL) {
        LOGE("failed to open!");
        return ;
    }

    fseek(fp, 0, SEEK_END);
    mDataLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    mData = new uint8_t[mDataLength];
    fread(mData, 1, mDataLength, fp);

    while (mCurrentIndex != mDataLength) {
        LOGE("index = %d", mCurrentIndex);
        len = readUint32Lit();
        LOGE("len = %d", len);
        memcpy(&k, mData + mCurrentIndex, 4);
        mCurrentIndex += 4;
        indian_a.size = k;
        memcpy(p, indian_a.byte, sizeof(indian_a.byte));
        p[4] = 0;

        string str = (char *)p;
        LOGE("%s", p);
//        break;
        if (str == "free") {
            LOGE("free box, write");
            fseek(fp, mCurrentIndex, SEEK_SET);
            fwrite(dinfo, 1, 16, fp);
            fclose(fp);
            break;
        } else {
            mCurrentIndex += len - 8;
        }
    }
    mCurrentIndex = 0;
}

JNIEXPORT jstring JNICALL
Java_com_zcf_darkmark_MainActivity_readDeviceInfo (
        JNIEnv *env, jobject , jstring file) {
    LOGE("read begin");
    char info[16] = {0};
    int len = 0;
    unsigned char p[5];
    unsigned int k = 0;
    mCurrentIndex = 0;
    const char *filename = env->GetStringUTFChars(file, JNI_FALSE);
    FILE *fp = fopen(filename, "rb+");
    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    mDataLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    mData = new uint8_t[mDataLength];
    fread(mData, 1, mDataLength, fp);

    while (mCurrentIndex != mDataLength) {
        LOGE("index = %d", mCurrentIndex);
        len = readUint32Lit();
        LOGE("len = %d", len);
        memcpy(&k, mData + mCurrentIndex, 4);
        mCurrentIndex += 4;
        indian_a.size = k;
        memcpy(p, indian_a.byte, sizeof(indian_a.byte));
        p[4] = 0;

        string str = (char *)p;
        LOGE("%s", p);
//        break;
        if (str == "free") {
            LOGE("free box, read");
            fseek(fp, mCurrentIndex, SEEK_SET);
            fread(info, 1, 16, fp);
            fclose(fp);
            break;
        } else {
            mCurrentIndex += len - 8;
        }
    }
    mCurrentIndex = 0;

    return env->NewStringUTF(info);
}

JNIEXPORT jstring JNICALL
Java_com_zcf_darkmark_MainActivity_readFileTime (
        JNIEnv *env, jobject ,jstring file) {
    LOGE("get time!");
    char time[] = "2015-01-01 00:00:00";
    int len = 0;
    unsigned char p[5];
    unsigned int k = 0;
    mCurrentIndex = 0;
    unsigned int sec = 0;

    const char *filename = env->GetStringUTFChars(file, JNI_FALSE);
    FILE *fp = fopen(filename, "rb+");
    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    mDataLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    mData = new uint8_t[mDataLength];
    fread(mData, 1, mDataLength, fp);

    while (mCurrentIndex != mDataLength) {
        LOGE("index = %d", mCurrentIndex);
        len = readUint32Lit();
        LOGE("len = %d", len);
        memcpy(&k, mData + mCurrentIndex, 4);
        mCurrentIndex += 4;
        indian_a.size = k;
        memcpy(p, indian_a.byte, sizeof(indian_a.byte));
        p[4] = 0;

        string str = (char *)p;
        LOGE("%s", p);
        if (str == "moov") {
            LOGE("moov box, read");
            mCurrentIndex += 12;
            sec = readUint32Lit();
            LOGE("creation time: %u", sec);
            break;
        } else {
            mCurrentIndex += len - 8;
        }
    }
    sec -= SEC_BASE3;               //从20170101计算的秒数
    LOGE("creation time: %u", sec);
    int year = sec / SECPERYEAR;    //几年
    LOGE("year = %d", year);
    sec %= SECPERYEAR;              //不满一年的秒数
    LOGE("creation time: %u", sec);
    memcpy(time + 2, &y[year][0], 2);   //更新年份
    int day = sec / SECPERDAY;      //换算成天数（不满一年）
    LOGE("day = %d", day);
    sec %= SECPERDAY;               //不满一天的秒数
    LOGE("creation time: %u", sec); //55183s
    int month = 0;
//    if (year < 3) {                 //没到2020年，都是平年
        for (int i = 0; i < 12; ++i) {
            if (day < monthA[i]) {
                month = i;
                break;
            }
        }
        LOGE("month = %d", month + 1);
        memcpy(time + 5, m[month], 2);  //更新月份
        if (month != 0)
            day -= monthA[month - 1];       //当月第几天
        LOGE("day = %d", day);
        memcpy(time + 8, &d[day-1][0], 2);    //更新天份
//    }

    int h = sec / SECPERHOUR;
    sec %= SECPERHOUR;
    LOGE("hour = %d", h);
    memcpy(time + 11, a + h/10, 1);   //更新小时
    memcpy(time + 12, a + h%10, 1);

    int min = sec / 60;
    LOGE("min = %d", min);
    memcpy(time + 14, a + min/10, 1);   //更新分钟
    memcpy(time + 15, a + min%10, 1);

    sec %= 60;
    LOGE("sec = %d", sec);
    memcpy(time + 17, a + sec/10, 1);   //更新秒数
    memcpy(time + 18, a + sec%10, 1);

    mCurrentIndex = 0;

    return env->NewStringUTF(time);
}

}