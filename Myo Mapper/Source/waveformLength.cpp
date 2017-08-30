#include "waveformLength.h"

WaveformLength::WaveformLength ()
{
}

WaveformLength::~WaveformLength()
{
}

void WaveformLength::setValue (float Value)
{
    input = Value;
    wl = std::abs (input-previousInput)*factor;
    previousInput = input;
}

float WaveformLength::getValue()
{
    return wl;
}

void WaveformLength::set3DValue (Vector3D<float> Value3D)
{
    
    input3D = Value3D;
    
    wl3D.x = std::abs (input3D.x-previousInput3D.x) * factor;
    wl3D.y = std::abs (input3D.y-previousInput3D.y) * factor;
    wl3D.z = std::abs (input3D.z-previousInput3D.z) * factor;

    previousInput3D = input3D;
}


Vector3D<float> WaveformLength::get3DValue()
{
    return wl3D;
}
