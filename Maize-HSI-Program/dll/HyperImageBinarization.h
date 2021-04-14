// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HYPERIMAGEBINARIZATION_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HYPERIMAGEBINARIZATION_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef HYPERIMAGEBINARIZATION_EXPORTS
#define HYPERIMAGEBINARIZATION_API extern "C" __declspec(dllexport)
#else
#define HYPERIMAGEBINARIZATION_API __declspec(dllimport)
#endif

#include "opencv.hpp"

HYPERIMAGEBINARIZATION_API void hyperImageBinarization(unsigned char* inputBinFile, int waveNumbers, int waveSamples, int lineNumbers,int dividend,int divisor, unsigned char* output);

HYPERIMAGEBINARIZATION_API void extractAndSaveImage(const std::string binaryFile, const std::string& targetPath, int waveNumbers, int waveSamples, int lineNumbers);

HYPERIMAGEBINARIZATION_API void calculateTotalReflectingRate(char* binaryFileDirectory, char* whiteBoardFileDirectory, char* blackBoardFileDirectory,unsigned char* maskImage, int waveLengthNumber, int waveLengthSample, int imageWidth, double* outputTotalReflecting);

HYPERIMAGEBINARIZATION_API void hyperImageBinarization_oilSeed(unsigned char* inputBinFile, int waveNumbers, int waveSamples, int lineNumbers, int dividend, int divisor, unsigned char* output);

HYPERIMAGEBINARIZATION_API void hyperImageBinarization_170829(unsigned char* inputBinFile, int waveNumbers, int waveSamples, int lineNumbers, int dividend, int divisor,double firstOTSUMultiple,double secondOTSUMultiple, unsigned char* output);
