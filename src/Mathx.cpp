#include <jni.h>
#include "Mathx.h"
#include <cmath>
namespace mathx{
    double sqrt(double s){
        double x = s / 2.0;
        for (int i = 0; i <= 15; i++){
            double xNew = 1.0 / 2.0 * (x + s / x);
            x = xNew;
        }
        return x;
    }

    double pow(double b, int exp){
        if (exp == 0) {
            return 1;
        } else if (exp < 0) {
            return (1.0 / b) * (pow(b, exp + 1));
        } else {
            return b * pow(b, exp - 1);
        }
    }

    double factorial(int n){
        if (n <= 0) {
            return 1;
        } else {
            return (double)n * factorial(n - 1);
        }
    }

    double stdDev(const double *arr, int len){
        if (arr == nullptr || len <= 1){
        return NAN;
    }
    double sum = 0;
    for (int i = 0; i < len; i++){
        sum += arr[i];
    }
    double mean = sum / len;
    sum = 0;
    for (int i = 0; i < len; i++){
        sum += (arr[i] - mean) * (arr[i] - mean);
    }
    return sqrt(sum / (len - 1));
    }

    double cos(double r){
        double rtn = 0;
        int sign = 1;
        for (int i = 0; i < 150; i += 2) {
            rtn += sign * (pow(r, i) / factorial(i));
            sign *= -1;
        }
        return rtn;
    }

    double ln(double x){
        if (x <= 0) {
            return NAN;
        } else {
            double y = (x - 1) / (x + 1);
            double rtn = 0;
            for (int i = 1; i < 150; i += 2) {
                rtn += pow(y, i) / i;
            }
            return 2 * rtn;
        }
    }
    
    double sin(double r){
        double rtn = 0;
        int sign = 1;
        for (int i = 1; i < 150; i += 2) {
            rtn += sign * (pow(r, i) / factorial(i));
            sign *= -1;
        }
        return rtn;
    }

    double exp(double x){
        double rtn = 0;
        for (int i = 0; i < 150; i++) {
            rtn += pow(x, i) / factorial(i);
        }
        return rtn;
    }

    double pi(int n){
        double rtn = 0;
        double denom = 1;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                rtn += 1.0 / denom;
            } else {
                rtn -= 1.0 / denom;
            }
            denom += 2;
        }
        return 4 * rtn;
    }

    double euler(int n){
        if (n <= 0) {
            return 1.0;
        } else {
            return 1.0 / factorial(n) + euler(n - 1);
        }
    }
}
extern "C"{
    JNIEXPORT jdouble JNICALL Java_Mathx_sqrt(JNIEnv *env, jclass clazz, jdouble x){
        return mathx::sqrt(x);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_power(JNIEnv *env, jclass clazz, jdouble base, jint exponent){
        return mathx::pow(base, exponent);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_factorial(JNIEnv *env, jclass clazz, jint n){
        return mathx::factorial(n);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_cos(JNIEnv *env, jclass clazz, jdouble radians){
        return mathx::cos(radians);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_sin(JNIEnv *env, jclass clazz, jdouble radians){
        return mathx::sin(radians);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_exp(JNIEnv *env, jclass clazz, jdouble n){
        return mathx::exp(n);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_ln(JNIEnv *env, jclass clazz, jdouble n){
        return mathx::ln(n);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_pi(JNIEnv *env, jclass clazz, jint n){
        return mathx::pi(n);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_euler(JNIEnv *env, jclass clazz, jint n){
        return mathx::euler(n);
    }
    JNIEXPORT jdouble JNICALL Java_Mathx_stdDev(JNIEnv *env, jclass clazz, jdoubleArray values){
        jsize len = env->GetArrayLength(values);
        const jdouble *rtn = env->GetDoubleArrayElements(values, NULL);
        const jdouble *arr = rtn;
        double x = mathx::stdDev(arr, len);
        env->ReleaseDoubleArrayElements(values, (jdouble *)rtn, 0);
        return x;
    }
}