#include "dothinkey.h"

Dothinkey::Dothinkey(Logger* logger, QGraphicsView* view, QWidget *parent)
    : m_iDevIDA(-1)
    , m_iDevIDB(-1)
    , m_fMclkA(12.0f)
    , m_fMclkB(12.0f)
    , m_fAvddA(2.8f)
    , m_fDvddA(1.5f)
    , m_fDovddA(1.8f)
    , m_fAfvccA(2.8f)
    , m_fAvddB(2.8f)
    , m_fDvddB(1.5f)
    , m_fDovddB(1.8f)
    , m_fAfvccB(2.8f)
    , m_uFocusPosA(0)
    , m_uFocusPosB(0)
    , m_vppA(0.0f)
    , m_vppB(0.0f)
{
    this->logger = logger;
    this->view = view;
    imageGrabbingThread = new imageGrabbingWorkerThread();
    connect(imageGrabbingThread,  SIGNAL(done()), this, SLOT(finishGrabbing()));
}

Dothinkey::~Dothinkey()
{
    //Clear things here
    if (imageGrabbingThread != nullptr) {
        imageGrabbingThread->quit();
    }
}

BOOL Dothinkey::DothinkeyEnum()
{
    imageGrabbingThread->start();
    logger->write("[DothinkeyEnum] is called");
    DeviceName[4] = { 0 };
    int DeviceNum;
    EnumerateDevice(DeviceName, 4, &DeviceNum);
    if (DeviceNum == 0)
    {
        //Log::GetInstance()->Write("[DothinkeyEnum] Cannot find any device!");
        logger->write("[DothinkeyEnum] Cannot find any device!");
        return DT_ERROR_FAILED;
    }
    for (int i = 0; i < DeviceNum; i++)
    {
        if (DeviceName[i] != nullptr)
        {
            QString str = "[DothinkeyEnum] Device Found: ";
            str.append(DeviceName[i]);
            logger->write(str);
        }
    }
    return DT_ERROR_OK;
}

//Only one channel of dothinkey
BOOL Dothinkey::DothinkeyOpen()
{
    if (m_iDevIDA >= 0) CloseDevice(m_iDevIDA);
    int iDevIDA = -1;
    if (OpenDevice(this->DeviceName[0], &iDevIDA, 0) != DT_ERROR_OK)
    {
        logger->write("[DothinkeyOpen] Open device fail!");
        return DT_ERROR_FAILED;
    }
    else
    {
        BYTE pSN[32];
        int iBufferSize = 32;
        int pRetLen = 0;
        if (GetDeviceSN(pSN, iBufferSize, &pRetLen, iDevIDA) == DT_ERROR_OK)
        {
            std::string s(reinterpret_cast<const char *>(pSN), 32);
            if (s.length() > 0) {
                logger->write("[DothinkeyOpen] Open device success!");
                m_iDevIDA = iDevIDA;
            }
        }
    }
    return DT_ERROR_OK;
}

BOOL Dothinkey::DothinkeyLoadIniFile(int channel) {
    SensorTab *pCurrentSensor;
    channel == 0 ? (pCurrentSensor = &this->current_sensor_a) : (pCurrentSensor = &this->current_sensor_b);
    iniParser *iniParser_ = new iniParser();
    //std::string str_filename = "C:\\Sparrow\\IMX362_4L_3024_063_34.ini";
    std::string str_filename = "C:\\Sparrow\\IMX214_4L_3120_063_34.ini";
    iniParser_->SetIniFilename(str_filename);

    if (channel == 0)
    {
        m_fMclkA = (float)iniParser_->ReadIniData("Sensor", "mclk", 0x01) / 1000;
        m_fAvddA = (float)iniParser_->ReadIniData("Sensor", "avdd", 0x00) / 1000;
        m_fDovddA = (float)iniParser_->ReadIniData("Sensor", "dovdd", 0x00) / 1000;
        m_fDvddA = (float)iniParser_->ReadIniData("Sensor", "dvdd", 0x00) / 1000;
    }
    else if (channel == 1)
    {
        m_fMclkB = (float)iniParser_->ReadIniData("Sensor", "mclk", 0x01) / 1000;
        m_fAvddB = (float)iniParser_->ReadIniData("Sensor", "avdd", 0x00) / 1000;
        m_fDovddB = (float)iniParser_->ReadIniData("Sensor", "dovdd", 0x00) / 1000;
        m_fDvddB = (float)iniParser_->ReadIniData("Sensor", "dvdd", 0x00) / 1000;
    }
    pCurrentSensor->width = iniParser_->ReadIniData("Sensor", "width", 0);
    pCurrentSensor->height = iniParser_->ReadIniData("Sensor", "height", 0);
    pCurrentSensor->type = iniParser_->ReadIniData("Sensor", "type", 2);
    pCurrentSensor->port = iniParser_->ReadIniData("Sensor", "port", 0);
    pCurrentSensor->pin = iniParser_->ReadIniData("Sensor", "pin", 0);
    pCurrentSensor->SlaveID = iniParser_->ReadIniData("Sensor", "SlaveID", 0);
    pCurrentSensor->mode = iniParser_->ReadIniData("Sensor", "mode", 0);
    pCurrentSensor->FlagReg = iniParser_->ReadIniData("Sensor", "FlagReg", 0);
    pCurrentSensor->FlagMask = iniParser_->ReadIniData("Sensor", "FlagMask", 0xff);
    pCurrentSensor->FlagData = iniParser_->ReadIniData("Sensor", "FlagData", 0);
    pCurrentSensor->FlagReg1 = iniParser_->ReadIniData("Sensor", "FlagReg1", 0);
    pCurrentSensor->FlagMask1 = iniParser_->ReadIniData("Sensor", "FlagMask1", 0x0);
    pCurrentSensor->FlagData1 = iniParser_->ReadIniData("Sensor", "FlagData1", 0);
    pCurrentSensor->outformat = iniParser_->ReadIniData("Sensor", "outformat", 0x00);
    pCurrentSensor->mclk = iniParser_->ReadIniData("Sensor", "mclk", 0x01);
    pCurrentSensor->avdd = iniParser_->ReadIniData("Sensor", "avdd", 0x00);
    pCurrentSensor->dovdd = iniParser_->ReadIniData("Sensor", "dovdd", 0x00);
    pCurrentSensor->dvdd = iniParser_->ReadIniData("Sensor", "dvdd", 0x00);
    pCurrentSensor->ParaList = new USHORT[8192 * 4];
    pCurrentSensor->ParaListSize = 0;
    pCurrentSensor->SleepParaList = NULL;
    pCurrentSensor->SleepParaListSize = NULL;

    iniParser_->GetI2CData(pCurrentSensor);

    delete iniParser_;
    return DT_ERROR_OK;
}

/*
 * Dothinkey Start Camera Funcion
 * params: int channel : Select camera channel
 * This function is used to power on the camera modules
 */
BOOL Dothinkey::DothinkeyStartCamera(int channel)
{
    SensorTab *pSensor = nullptr;
    ULONG *grabSize = nullptr;
    int iDevID = -1;
    if (channel == 0)
    {
        pSensor = &current_sensor_a;
        iDevID = m_iDevIDA;
        grabSize = &m_GrabSizeA;
    }
    else if (channel == 1)
    {
        pSensor = &current_sensor_b;
        iDevID = m_iDevIDB;
        grabSize = &m_GrabSizeB;
    }
    channel == 0 ? pSensor = &current_sensor_a : &current_sensor_b;
    channel == 0 ? iDevID = m_iDevIDA : m_iDevIDB;
    //ToDo: KillDataBuffer
    SetSoftPinPullUp(IO_NOPULL, 0);
    if (SetSensorClock(false, (USHORT)(0 * 10), iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        logger->write("[DothinkeyStartCamera] Set Clock Fail!");
        return false;
    }
    Sleep(1);

    if (SetVoltageMclk(*pSensor, iDevID, m_fMclkA, m_fAvddA, m_fDvddA, m_fDovddA, m_fAfvccA, m_vppA) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        logger->write("[DothinkeyStartCamera] Set Voltage and Mclk Failed!");
        return false;
    }
    logger->write("[DothinkeyStartCamera] Start Camera Success!");

    //I2C init
    SetSensorI2cRate(I2C_400K, iDevID);
    SetSensorI2cRapid(TRUE, iDevID);

    //check sensor is on line or not ,if on line,init sensor to work....
    SensorEnable(pSensor->pin ^ 0x02, 1, iDevID); //reset
    Sleep(20);
    SensorEnable(pSensor->pin, 1, iDevID);
    Sleep(50);

    //check sensoris on line...
    if (SensorIsMe(pSensor, CHANNEL_A, 0, iDevID) != DT_ERROR_OK)
    {
        logger->write("[DothinkeyStartCamera] Sensor is not ok!");
        return DT_ERROR_FAILED;
    }
    //init sensor....
    if (InitSensor(pSensor->SlaveID,
        pSensor->ParaList,
        pSensor->ParaListSize,
        pSensor->mode, iDevID) != DT_ERROR_OK)
    {
        logger->write("[DothinkeyStartCamera] Init Sensor Failed! \r\n");
        return DT_ERROR_FAILED;
    }

    if (pSensor->type == D_YUV || pSensor->type == D_YUV_SPI || pSensor->type == D_YUV_MTK_S)
        SetYUV422Format(pSensor->outformat, iDevID);
    else
        SetRawFormat(pSensor->outformat, iDevID);
    InitRoi(0, 0, pSensor->width, pSensor->height, 0, 0, 1, 1, pSensor->type, TRUE, iDevID);
    SetSensorPort(pSensor->port, pSensor->width, pSensor->height, iDevID);
    CalculateGrabSize(grabSize, iDevID);
    //open video....
    OpenVideo(*grabSize, iDevID);
    SetMonoMode(true, iDevID);
    //InitDisplay and InitIsp are required to call in order to capture frame
    InitDisplay(nullptr, pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, NULL, iDevID);
    InitIsp(pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, iDevID);
    return DT_ERROR_OK;
}

BOOL Dothinkey::SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp)
{
    SENSOR_POWER Power[10] = { POWER_AVDD, POWER_DOVDD, POWER_DVDD, POWER_AFVCC, POWER_VPP };
    int Volt[10] = { 0 };
    int Current[10] = { 300, 300, 300, 300, 100 };//300mA
    BOOL OnOff[10] = { TRUE,TRUE,TRUE,TRUE,TRUE };
    CURRENT_RANGE range[5] = { CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA };
    //set power to 0V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        logger->write("[SetVoltageMclk] Set Voltage Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    //wait for the power is all to zero....
    Sleep(50);
    if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        logger->write("[SetVoltageMclk] Open PowerOnOff Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    Sleep(50);
    // 1. set power the avdd
    Volt[POWER_DOVDD] = (int)(Dovdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        logger->write("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 2, set power the dvdd
    Volt[POWER_DVDD] = (int)(Dvdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        logger->write("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 3, set power the dovdd
    Volt[POWER_AVDD] = (int)(Avdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        logger->write("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);
    //4. set power the afvcc and vpp
    Volt[POWER_AFVCC] = (int)(Afvcc * 1000); // 2.8V
    Volt[POWER_VPP] = (int)(vpp * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        logger->write("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    //should wait for 50ms to be ready...
    Sleep(50);
    //first set pin definition...
    {
        BYTE  pinDef[40] = { 0 };
        if (CurrentSensor.port == PORT_MIPI || CurrentSensor.port == PORT_HISPI)
        {
            pinDef[0] = 20;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 13;
            pinDef[18] = 15;
            pinDef[19] = 14;
            pinDef[20] = 19;
            pinDef[21] = 18;
            pinDef[22] = 20;
            pinDef[23] = 16;
            pinDef[24] = 20;
            pinDef[25] = 20;
        }
        else  //standard parallel..
        {

            pinDef[0] = 16;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 20;
            pinDef[18] = 20;
            pinDef[19] = 20;
            pinDef[20] = 13;
            pinDef[21] = 20;
            pinDef[22] = 14;
            pinDef[23] = 15;
            pinDef[24] = 18;
            pinDef[25] = 19;
        }
        SetSoftPin(pinDef, iDevID);
    }
    EnableSoftPin(TRUE, iDevID);
    EnableGpio(TRUE, iDevID);

    int SampleSpeed[5] = { 100,100,100,100,100 };
    PmuSetSampleSpeed(Power, SampleSpeed, 5, iDevID);
    Sleep(10);
    {
        PmuSetCurrentRange(Power, range, 5, iDevID);
        PmuSetOcpCurrentLimit(Power, Current, 5, iDevID);
    }
    if (SetSensorClock(TRUE, (USHORT)(Mclk * 10), iDevID) != DT_ERROR_OK)
    {
        logger->write("[SetVoltageMclk] Set Mclk Failed!");
        return DT_ERROR_FAILED;
    }
    SetSoftPinPullUp(IO_PULLUP, iDevID);
    return TRUE;
}

BOOL Dothinkey::DothinkeyGrabImage(int channel)
{
    logger->write("[DothinkeyGrabImage] Start");
    LPBYTE bmpBuffer = NULL;
    SensorTab *pSensor = nullptr;
    ULONG retSize = 0;
    int iDevID = -1;
    UINT crcCount = 0;
    int grabSize;
    if (channel == 0)
    {
        pSensor = &current_sensor_a;
        iDevID = this->m_iDevIDA;
        grabSize = this->m_GrabSizeA;
    }
    else if (channel == 1)
    {
        pSensor = &current_sensor_b;
        iDevID = this->m_iDevIDB;
        grabSize = this->m_GrabSizeB;
    }

    USHORT width = pSensor->width;
    USHORT height = pSensor->height;
    //BYTE type = pSensor->type;
    FrameInfo frameInfo;
    bmpBuffer = (LPBYTE)malloc(width * height * 4);
    if (bmpBuffer == NULL)
    {
        logger->write("[DothinkeyGrabImage] Malloc BMP buffer fail.");
    }
    //allocate the bmp buffer.
    UINT nSize = width * height * 3 + 1024 * 1024;
    LPBYTE CameraBuffer = NULL;
    CameraBuffer = (LPBYTE)malloc(nSize);
    if ((CameraBuffer == NULL))
    {
        return DT_ERROR_FAILED;
    }
    memset(CameraBuffer, 0, nSize);
    logger->write("[DothinkeyGrabImage] Grabbing image...");
    int ret = GrabFrame(CameraBuffer, grabSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    }
    logger->write("[DothinkeyGrabImage] Grabbing image finished");
    //BOOL bRaw10 = FALSE;
    ImageProcess(CameraBuffer, bmpBuffer, width, height, &frameInfo, iDevID);
    logger->write("[DothinkeyGrabImage] Display 1");
    //DisplayRGB24(bmpBuffer, &frameInfo, iDevID);
    //logger->write("[DothinkeyGrabImage] Display 2");
    QImage * image = new QImage((const uchar*) bmpBuffer, width, height, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(*image);
    QGraphicsScene * scene = new QGraphicsScene();
    logger->write("[DothinkeyGrabImage] Display");
    scene->addPixmap(pixmap);
    view->setScene(scene);
    view->show();
    delete(bmpBuffer);
    delete(CameraBuffer);
    bmpBuffer = NULL;
    CameraBuffer = NULL;
    logger->write("[DothinkeyGrabImage] Finish");
    return DT_ERROR_OK;
}

BOOL Dothinkey::DothinkeyGrabbingThread(bool on)
{
    logger->write("Force stop is clicked");
    imageGrabbingThread->stop();
    return DT_ERROR_OK;
}

BOOL Dothinkey::SaveBmpFile(std::string sfilename, BYTE *pBuffer, UINT width, UINT height)
{
    int				 OffBits;
    HFILE			 bmpFile;
    BITMAPFILEHEADER bmpHeader; // Header for Bitmap file
    BITMAPINFO		 bmpInfo;

    OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    DWORD dwSizeImage = width * height * 3;//IMAGESIZE_X*IMAGESIZE_Y*3;

    bmpHeader.bfType = ((WORD)('M' << 8) | 'B');
    bmpHeader.bfSize = OffBits + dwSizeImage;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = OffBits;

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = width;
    bmpInfo.bmiHeader.biHeight = height;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    bmpFile = _lcreat(sfilename.c_str(), FALSE);
    if (bmpFile < 0)
    {
        logger->write("Cannot create the bmp file.\r\n");
        return FALSE;
    }

    UINT len;
    len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));
    len = _lwrite(bmpFile, (LPSTR)&bmpInfo, sizeof(BITMAPINFOHEADER));
    len = _lwrite(bmpFile, (LPSTR)pBuffer, bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
    _lclose(bmpFile);

    return TRUE;
}

void Dothinkey::finishGrabbing()
{
    DothinkeyGrabImage(0);
}
