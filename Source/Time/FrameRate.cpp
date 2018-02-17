#include "FrameRate.h"
bool FrameRate::initialized = false;
unsigned long long FrameRate::frameCount = 0, FrameRate::smoothedDeltaTimeFrameCount = 100;
double FrameRate::deltaTime = 0, FrameRate::framesPerSecond = 1, FrameRate::time = 0, FrameRate::smoothedDeltaTime = 0;
