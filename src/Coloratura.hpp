#include "plugin.hpp"
#include "samplerate.h"

constexpr static size_t HISTORY_SIZE = 1 << 21;
dsp::DoubleRingBuffer<float, HISTORY_SIZE> historyBuffer;
dsp::DoubleRingBuffer<float, 16> outBuffer;
SRC_STATE* src;
float lastWet = 0.f;
float clockFreq = 1.f;
dsp::Timer clockTimer;
dsp::SchmittTrigger clockTrigger;
float clockPhase = 0.f;