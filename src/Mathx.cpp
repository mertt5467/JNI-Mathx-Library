#include <jni.h>
#include "Mathx.h"
#include "Mathx_Stats.h"
#include "Mathx_Geo.h"
#include <cmath>
#include <vector>

template<typename JArrayT, typename NativeT>
    struct JniTraits;
    template <> struct JniTraits<jdoubleArray, jdouble>{
        static jdouble* get(JNIEnv* env, jdoubleArray a){
            return static_cast<jdouble*>(env->GetPrimitiveArrayCritical(a, nullptr));
        }
        static void release(JNIEnv* env, jdoubleArray a, jdouble* arr){
            env->ReleasePrimitiveArrayCritical(a, arr, 0);
        }
    };
    template <> struct JniTraits<jintArray, jint>{
        static jint* get(JNIEnv* env, jintArray a){
            return static_cast<jint*>(env->GetPrimitiveArrayCritical(a, nullptr));
        }
        static void release(JNIEnv* env, jintArray a, jint* arr){
            env->ReleasePrimitiveArrayCritical(a, arr, 0);
        }
    };
    template <> struct JniTraits<jlongArray, jlong>{
        static jlong* get(JNIEnv* env, jlongArray a){
            return static_cast<jlong*>(env->GetPrimitiveArrayCritical(a, nullptr));
        }
        static void release(JNIEnv* env, jlongArray a, jlong* arr){
            env->ReleasePrimitiveArrayCritical(a, arr, 0);
        }
    };
    template <> struct JniTraits<jfloatArray, jfloat>{
        static jfloat* get(JNIEnv* env, jfloatArray a){
            return static_cast<jfloat*>(env->GetPrimitiveArrayCritical(a, nullptr));
        }
        static void release(JNIEnv* env, jfloatArray a, jfloat* arr){
            env->ReleasePrimitiveArrayCritical(a, arr, 0);
        }
    };
    template <typename JArrayT, typename NativeT>
    class JniArray{
    private:
        JArrayT jArray;
        NativeT* rawPtr;
        JNIEnv* env;
        jsize len;
    public:
        JniArray(JNIEnv* env, JArrayT jArray): env(env), jArray(jArray), rawPtr(nullptr){
            if(jArray){
                len = env->GetArrayLength(jArray);
                rawPtr = JniTraits<JArrayT, NativeT>::get(env, jArray);
            }
        }
        ~JniArray(){
            if(rawPtr){
                JniTraits<JArrayT, NativeT>::release(env, jArray, rawPtr);
            }
        }
        NativeT* data(){ return rawPtr; }
        jsize size(){ return len; }
        bool isNull(){ return rawPtr == nullptr; }
    };
namespace kernel{
    double sqrt(double s){
        double x = s / 2.0;
        for (int i = 0; i <= 15; i++){
            double xNew = 1.0 / 2.0 * (x + s / x);
            x = xNew;
        }
        return x;
    }

    double pow(double b, int exp){
        double result = 1.0;
        if (exp == 0) {
            return 1;
        } else if (exp < 0) {
            for(int i = 0; i > exp; i--){
                result /= b;
            }
        } else {
            for (int i = 0; i < exp; i++) {
                result *= b;
            }
        }
        return result;
    }
    double factorial(int n){
        if (n <= 0) {
            return 1;
        } else {
            double result = 1;
            for (int i = 1; i <= n; i++) {
                result *= i;
            }
            return result;
        }
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
namespace kernel::geo{
    double sin(double r){
        double rtn = 0;
        int sign = 1;
        for (int i = 1; i < 150; i += 2) {
            rtn += sign * (pow(r, i) / factorial(i));
            sign *= -1;
        }
        return rtn;
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
}
namespace kernel::stats{
    template<typename T>
    double mean(const T *arr, int len){
        if (arr == nullptr || len <= 0){
            return NAN;
        }
        double sum = 0;
        #pragma omp simd reduction(+:sum)
        for (int i = 0; i < len; i++) sum += (double)arr[i];
        return sum / len;
    }
    template <typename T>
    double stdDev(const T *arr, int len){
        if (arr == nullptr || len <= 1){
            return NAN;
        }
        double mean_val = kernel::stats::mean(arr, len);
        double sum = 0;
        for (int i = 0; i < len; i++) sum += (double)(arr[i] - mean_val) * (double)(arr[i] - mean_val);
        return kernel::sqrt(sum / (len - 1));
    }
}

extern "C"{
    namespace mathx{
        JNIEXPORT jdouble JNICALL Java_Mathx_sqrt(JNIEnv *env, jclass clazz, jdouble x){
            return kernel::sqrt(x);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_factorial(JNIEnv *env, jclass clazz, jint n){
            return kernel::factorial(n);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_exp(JNIEnv *env, jclass clazz, jdouble n){
            return kernel::exp(n);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_ln(JNIEnv *env, jclass clazz, jdouble n){
            return kernel::ln(n);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_pi(JNIEnv *env, jclass clazz, jint n){
            return kernel::pi(n);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_euler(JNIEnv *env, jclass clazz, jint n){
            return kernel::euler(n);
        }
    }
    namespace mathx::stats{
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_mean___3D(JNIEnv *env, jclass clazz, jdoubleArray values){
            JniArray<jdoubleArray, jdouble> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::mean(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_mean___3I(JNIEnv *env, jclass clazz, jintArray values){
            JniArray<jintArray, jint> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::mean(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_mean___3J(JNIEnv *env, jclass clazz, jlongArray values){
            JniArray<jlongArray, jlong> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::mean(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_mean___3F(JNIEnv *env, jclass clazz, jfloatArray values){
            JniArray<jfloatArray, jfloat> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::mean(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_stdDev___3D(JNIEnv *env, jclass clazz, jdoubleArray values){
            JniArray<jdoubleArray, jdouble> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::stdDev(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_stdDev___3I(JNIEnv *env, jclass clazz, jintArray values){
            JniArray<jintArray, jint> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::stdDev(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_stdDev___3J(JNIEnv *env, jclass clazz, jlongArray values){
            JniArray<jlongArray, jlong> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::stdDev(arr.data(), arr.size());
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Stats_stdDev___3F(JNIEnv *env, jclass clazz, jfloatArray values){
            JniArray<jfloatArray, jfloat> arr(env, values);
            if(arr.isNull()) return NAN;
            return kernel::stats::stdDev(arr.data(), arr.size());
        }
    }
    namespace mathx::geo{
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Geo_cos(JNIEnv *env, jclass clazz, jdouble radians){
        return kernel::geo::cos(radians);
        }
        JNIEXPORT jdouble JNICALL Java_Mathx_00024Geo_sin(JNIEnv *env, jclass clazz, jdouble radians){
            return kernel::geo::sin(radians);
        }
    }
}