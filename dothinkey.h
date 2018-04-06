#ifndef DOTHINKEY_H
#define DOTHINKEY_H

#include <string>
#include <Windows.h>
#include "imagekit.h"
#include "dtccm2.h"
#include "iniparser.h"
#include "logger.h"
#include <QObject>
#include <QPixmap>

#define CAMERA_START 1
#define CAMERA_STOP 2
#define GRAB_START 1
#define GRAB_STOP 2

class Dothinkey : public QObject
{
    Q_OBJECT
public:
    explicit Dothinkey(QWidget *parent = 0);
    ~Dothinkey();
    BOOL DothinkeyEnum();   //Enumerate the dothinkey devices
    BOOL DothinkeyOpen();   //Open Camera Devices
    BOOL DothinkeyClose();  //Close Camera Devices
    BOOL DothinkeyLoadIniFile(int channel);   // 0 is camera channel 0, 1 is camera channel 1
    BOOL DothinkeyStartCamera(int channel);
    BOOL DothinkeyGrabImage(int channel, QImage& output);

    struct CameraChannel
    {
        CameraChannel()
            : m_iDevID(-1)
            , m_fMclk(12.0f)
            , m_fAvdd(2.8f)
            , m_fDovdd(1.8f)
            , m_fDvdd(1.5f)
            , m_fAfvcc(2.8f)
            , m_vpp(0.0f)
            , m_uFocusPos(0)
        {}

        bool CloseCameraChannel();
        SensorTab current_sensor;
        char* m_charCurrentDevice;
        int m_iDevID;
        ULONG m_GrabSize;
        float m_fMclk;
        float m_fAvdd;
        float m_fDovdd;
        float m_fDvdd;
        float m_fAfvcc;
        float m_vpp;
        UINT m_uFocusPos;
        float m_fFrameFps;
        int m_iFrameErr;
        int m_iFrameCnt;
    };
private:
    char *DeviceName[4];

    BOOL SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp);
    void MipiRaw10ToP10(BYTE *pIn, BYTE *pOut, int width, int height);
    BOOL SaveBmpFile(std::string sfilename, BYTE *pBuffer, UINT width, UINT height);

    CameraChannel m_CameraChannels[2];
};

#endif // DOTHINKEY_H
