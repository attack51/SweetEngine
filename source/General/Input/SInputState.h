#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include 

typedef uint8_t SDiditalAxis;
#define DIGITAL_AXIS_NONE   0
#define DIGITAL_AXIS_LEFT   1
#define DIGITAL_AXIS_RIGHT  2
#define DIGITAL_AXIS_UP     4
#define DIGITAL_AXIS_DOWN   8

class SInputState
{
public:
    SInputState();
    ~SInputState();

    //digital axis
    const SDiditalAxis& GetDigitalAxis() const;
    SVector2 GetVectorDigitalAxis() const;

    void ClearDigitalAxis();
    void DigitalAxisPress(const SDiditalAxis& axis);
    void DigitalAxisRelease(const SDiditalAxis& axis);

    //mouse offset
    const SVector2& GetMouseOffset() const;
    void ClearMouseOffset();
    void ResetMousePos();

    void SetMousePos(SVector2 mousePos);

protected:
    SDiditalAxis m_digitalAxis;

    SVector2 m_mousePos;
    SVector2 m_mouseOffset;

};