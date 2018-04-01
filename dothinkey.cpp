#include "dothinkey.h"

Dothinkey::Dothinkey(Logger* logger)
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
}

void Dothinkey::DothinkeyEnum()
{
    logger->write("[DothinkeyEnum] is called");
    DeviceName[4] = { 0 };
    int DeviceNum;
    EnumerateDevice(DeviceName, 4, &DeviceNum);
    if (DeviceNum == 0)
    {
        //Log::GetInstance()->Write("[DothinkeyEnum] Cannot find any device!");
        logger->write("[DothinkeyEnum] Cannot find any device!");
        return;
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
}

//Only one channel of dothinkey
void Dothinkey::DothinkeyOpen()
{
    if (m_iDevIDA >= 0) CloseDevice(m_iDevIDA);
    int iDevIDA = -1;
    if (strlen(this->DeviceName[0]) && OpenDevice(this->DeviceName[0], &iDevIDA, 0) != DT_ERROR_OK)
    {
        logger->write("[DothinkeyOpen] Open device fail!");
        return;
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
}
