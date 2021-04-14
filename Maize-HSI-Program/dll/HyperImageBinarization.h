// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HYPERIMAGEBINARIZATION_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HYPERIMAGEBINARIZATION_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
