#ifndef DOTHINKEY_H
#define DOTHINKEY_H

#include <string>
#include <Windows.h>
#include "imagekit.h"
#include "dtccm2.h"
#include "iniparser.h"
#include "logger.h"
#include "imagegrabbingworkerthread.h"
#include <QObject>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#define CAMERA_START 1
#define CAMERA_STOP 2
#define GRAB_START 1
#define GRAB_STOP 2

class Dothinkey : public QObject
{
    Q_OBJECT
public:
    explicit Dothinkey(Logger* logger, QGraphicsView* view, QWidget *parent = 0);
    ~Dothinkey();
    BOOL DothinkeyEnum();   //Enumerate the dothinkey devices
    BOOL DothinkeyOpen();   //Open Camera Devices
    BOOL DothinkeyLoadIniFile(int channel);   // 0 is camera channel 0, 1 is camera channel 1
    BOOL DothinkeyStartCamera(int channel);
    BOOL DothinkeyGrabImage(int channel);
    BOOL DothinkeyGrabbingThread(bool on);
private slots:
    void finishGrabbing();
private:
    Logger* logger;
    QGraphicsView* view;
    imageGrabbingWorkerThread* imageGrabbingThread;
    char *DeviceName[4];

    BOOL SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp);
    void MipiRaw10ToP10(BYTE *pIn, BYTE *pOut, int width, int height);
    BOOL SaveBmpFile(std::string sfilename, BYTE *pBuffer, UINT width, UINT height);

    ULONG       m_GrabSizeA;
    ULONG       m_GrabSizeB;
    SensorTab   current_sensor_a;
    SensorTab   current_sensor_b;
    char*       m_charCurrentDeviceA;
    char*       m_charCurrentDeviceB;
    int         m_iDevIDA;
    int         m_iDevIDB;
    float       m_fMclkA;
    float       m_fMclkB;
    float       m_fAvddA;
    float       m_fDvddA;
    float       m_fDovddA;
    float       m_fAfvccA;
    float       m_fAvddB;
    float       m_fDvddB;
    float       m_fDovddB;
    float       m_fAfvccB;

    UINT        m_uFocusPosA;
    UINT        m_uFocusPosB;
    float       m_fFrameFpsA;
    int         m_iFrameErrA;
    int         m_iFrameCntA;
    float       m_fFrameFpsB;
    int         m_iFrameErrB;
    int         m_iFrameCntB;
    float m_vppA;
    float m_vppB;
};

#endif // DOTHINKEY_H
