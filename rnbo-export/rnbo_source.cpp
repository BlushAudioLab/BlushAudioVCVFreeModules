/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2022 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing@cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

Please see https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ for additional information
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define floor(x) ((long)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

rnbomatic()
{
}

~rnbomatic()
{
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

Index voice() {
    return this->_voiceIndex;
}

number random(number low, number high) {
    number range = high - low;
    return rand01() * range + low;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

inline number safemod(number f, number m) {
    if (m != 0) {
        if (m < 0) {
            m = -m;
        }

        if (f >= m) {
            if (f >= m * 2.0) {
                number d = f / m;
                int i = (int)(rnbo_trunc(d));
                d = d - i;
                f = d * m;
            } else {
                f -= m;
            }
        } else if (f <= -m) {
            if (f <= -m * 2.0) {
                number d = f / m;
                int i = (int)(rnbo_trunc(d));
                d = d - i;
                f = d * m;
            } else {
                f += m;
            }
        }
    } else {
        f = 0.0;
    }

    return f;
}

number wrap(number x, number low, number high) {
    number lo;
    number hi;

    if (low == high)
        return low;

    if (low > high) {
        hi = low;
        lo = high;
    } else {
        lo = low;
        hi = high;
    }

    number range = hi - lo;

    if (x >= lo && x < hi)
        return x;

    if (range <= 0.000000001)
        return lo;

    long numWraps = (long)(rnbo_trunc((x - lo) / range));
    numWraps = numWraps - ((x < lo ? 1 : 0));
    number result = x - range * numWraps;

    if (result >= hi)
        return result - range;
    else
        return result;
}

template <typename T> inline void poke_default(
    T& buffer,
    SampleValue value,
    SampleValue sampleIndex,
    int channel,
    number overdub
) {
    number bufferSize = buffer->getSize();
    const Index bufferChannels = (const Index)(buffer->getChannels());

    if (bufferSize > 0 && (5 != 5 || (sampleIndex >= 0 && sampleIndex < bufferSize)) && (5 != 5 || (channel >= 0 && channel < bufferChannels))) {
        if (overdub != 0) {
            number currentValue = buffer->getSample(channel, sampleIndex);

            {
                value = value * (1. - overdub) + currentValue * overdub;
            }
        }

        buffer->setSample(channel, sampleIndex, value);
        buffer->setTouched(true);
    }
}

inline number linearinterp(number frac, number x, number y) {
    return x + (y - x) * frac;
}

inline number cubicinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = z - y - w + x;
    number f1 = w - x - f0;
    number f2 = y - w;
    number f3 = x;
    return f0 * a * a2 + f1 * a2 + f2 * a + f3;
}

inline number splineinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
    number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
    number f2 = -0.5 * w + 0.5 * y;
    return f0 * a * a2 + f1 * a2 + f2 * a + x;
}

inline number cosT8(number r) {
    number t84 = 56.0;
    number t83 = 1680.0;
    number t82 = 20160.0;
    number t81 = 2.4801587302e-05;
    number t73 = 42.0;
    number t72 = 840.0;
    number t71 = 1.9841269841e-04;

    if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
        number rr = r * r;
        return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
    } else if (r > 0.0) {
        r -= 1.57079632679489661923132169163975144;
        number rr = r * r;
        return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    } else {
        r += 1.57079632679489661923132169163975144;
        number rr = r * r;
        return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    }
}

inline number cosineinterp(number frac, number x, number y) {
    number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
    return x * (1.0 - a2) + y * a2;
}

template <typename T> inline array<SampleValue, 2> peek_default(T& buffer, SampleValue sampleIndex, Index channel) {
    number bufferSize = buffer->getSize();
    const Index bufferChannels = (const Index)(buffer->getChannels());

    if (bufferSize == 0 || (5 == 5 && (sampleIndex < 0 || sampleIndex >= bufferSize)) || (5 == 5 && (channel < 0 || channel >= bufferChannels))) {
        return {0, 0};
    } else {
        number bufferreadsample_result;

        {
            auto& __buffer = buffer;

            if (sampleIndex < 0.0) {
                bufferreadsample_result = 0.0;
            }

            SampleIndex index1 = (SampleIndex)(rnbo_trunc(sampleIndex));

            {
                {
                    {
                        {
                            bufferreadsample_result = __buffer->getSample(channel, index1);
                        }
                    }
                }
            }
        }

        number sampleValue = bufferreadsample_result;
        return {sampleValue, sampleIndex};
    }
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo();
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
    if (!stringCompare(paramid, "saturation")) {
        return 0;
    }

    if (!stringCompare(paramid, "input_gain")) {
        return 1;
    }

    if (!stringCompare(paramid, "boost")) {
        return 2;
    }

    if (!stringCompare(paramid, "expansion")) {
        return 3;
    }

    return INVALID_INDEX;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    SampleValue ** inputs,
    Index numInputs,
    SampleValue ** outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);

    this->gen_01_perform(
        in1,
        this->gen_01_mix,
        this->gen_01_boost,
        this->gen_01_expansion,
        this->gen_01_saturation,
        this->gen_01_input_gain,
        out1,
        n
    );

    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->globaltransport_dspsetup(forceDSPSetup);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        this->setProbingIndex(-1);
        break;
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        return addressOf(this->d);
        break;
    default:
        return nullptr;
    }
}

DataRefIndex getNumDataRefs() const {
    return 1;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->gen_01_d = new Float64Buffer(this->d);
    }
}

void initialize() {
    this->d = initDataRef("d", true, nullptr);
    this->assign_defaults();
    this->setState();
    this->d->setIndex(0);
    this->gen_01_d = new Float64Buffer(this->d);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "saturation"));
    this->param_02_getPresetValue(getSubState(preset, "input_gain"));
    this->param_03_getPresetValue(getSubState(preset, "boost"));
    this->param_04_getPresetValue(getSubState(preset, "expansion"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "saturation"));
    this->param_02_setPresetValue(getSubState(preset, "input_gain"));
    this->param_03_setPresetValue(getSubState(preset, "boost"));
    this->param_04_setPresetValue(getSubState(preset, "expansion"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(tempo, false))
        {}
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(state, false))
        {}
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(beattime, false))
        {}
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(numerator, denominator, false))
        {}
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        this->param_01_value_set(v);
        break;
    case 1:
        this->param_02_value_set(v);
        break;
    case 2:
        this->param_03_value_set(v);
        break;
    case 3:
        this->param_04_value_set(v);
        break;
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        return this->param_01_value;
    case 1:
        return this->param_02_value;
    case 2:
        return this->param_03_value;
    case 3:
        return this->param_04_value;
    default:
        return 0;
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 4;
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "saturation";
    case 1:
        return "input_gain";
    case 2:
        return "boost";
    case 3:
        return "expansion";
    default:
        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "saturation";
    case 1:
        return "input_gain";
    case 2:
        return "boost";
    case 3:
        return "expansion";
    default:
        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = 0;
            info->max = 2;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 1:
            info->type = ParameterTypeNumber;
            info->initialValue = 5;
            info->min = 0;
            info->max = 10;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 2:
            info->type = ParameterTypeNumber;
            info->initialValue = 0.5;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 3:
            info->type = ParameterTypeNumber;
            info->initialValue = 2500;
            info->min = 50;
            info->max = 5000;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 2:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
    case 0:
        {
            value = (value < 0 ? 0 : (value > 2 ? 2 : value));
            ParameterValue normalizedValue = (value - 0) / (2 - 0);
            return normalizedValue;
        }
    case 1:
        {
            value = (value < 0 ? 0 : (value > 10 ? 10 : value));
            ParameterValue normalizedValue = (value - 0) / (10 - 0);
            return normalizedValue;
        }
    case 3:
        {
            value = (value < 50 ? 50 : (value > 5000 ? 5000 : value));
            ParameterValue normalizedValue = (value - 50) / (5000 - 50);
            return normalizedValue;
        }
    default:
        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 2:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
    case 0:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (2 - 0);
            }
        }
    case 1:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (10 - 0);
            }
        }
    case 3:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 50 + value * (5000 - 50);
            }
        }
    default:
        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        return this->param_01_value_constrain(value);
    case 1:
        return this->param_02_value_constrain(value);
    case 2:
        return this->param_03_value_constrain(value);
    case 3:
        return this->param_04_value_constrain(value);
    default:
        return value;
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}

void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}

void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    this->gen_01_saturation_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->gen_01_input_gain_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->gen_01_boost_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->gen_01_expansion_set(v);
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 1;
}

Index getNumOutputChannels() const {
    return 1;
}

void allocateDataRefs() {
    this->gen_01_d->requestSize(11000, 1);
    this->gen_01_d->setSampleRate(this->sr);
    this->gen_01_d = this->gen_01_d->allocateIfNeeded();

    if (this->d->hasRequestedSize()) {
        if (this->d->wantsFill())
            this->zeroDataRef(this->d);

        this->getEngine()->sendDataRefUpdated(0);
    }
}

void initializeObjects() {
    this->gen_01_d_count_init();
    this->gen_01_pad_factor_init();
    this->gen_01_last_width_init();
    this->gen_01_noise_11_init();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 2 ? 2 : (v < 0 ? 0 : v));
    return v;
}

static number gen_01_saturation_constrain(number v) {
    if (v < 0)
        v = 0;

    if (v > 2)
        v = 2;

    return v;
}

void gen_01_saturation_set(number v) {
    v = this->gen_01_saturation_constrain(v);
    this->gen_01_saturation = v;
}

static number param_02_value_constrain(number v) {
    v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    return v;
}

static number gen_01_input_gain_constrain(number v) {
    if (v < 0)
        v = 0;

    if (v > 10)
        v = 10;

    return v;
}

void gen_01_input_gain_set(number v) {
    v = this->gen_01_input_gain_constrain(v);
    this->gen_01_input_gain = v;
}

static number param_03_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

static number gen_01_boost_constrain(number v) {
    if (v < 0)
        v = 0;

    if (v > 1)
        v = 1;

    return v;
}

void gen_01_boost_set(number v) {
    v = this->gen_01_boost_constrain(v);
    this->gen_01_boost = v;
}

static number param_04_value_constrain(number v) {
    v = (v > 5000 ? 5000 : (v < 50 ? 50 : v));
    return v;
}

static number gen_01_expansion_constrain(number v) {
    if (v < 50)
        v = 50;

    if (v > 5000)
        v = 5000;

    return v;
}

void gen_01_expansion_set(number v) {
    v = this->gen_01_expansion_constrain(v);
    this->gen_01_expansion = v;
}

void gen_01_perform(
    const Sample * in1,
    number mix,
    number boost,
    number expansion,
    number saturation,
    number input_gain,
    Sample * out1,
    Index n
) {
    RNBO_UNUSED(mix);
    auto __gen_01_pad_factor_value = this->gen_01_pad_factor_value;
    auto __gen_01_last_width_value = this->gen_01_last_width_value;
    auto __gen_01_d_count_value = this->gen_01_d_count_value;
    Index i;

    for (i = 0; i < n; i++) {
        number temp_0 = in1[(Index)i];
        number x_2 = in1[(Index)i];

        if (__gen_01_d_count_value < 1 || __gen_01_d_count_value > 5000) {
            __gen_01_d_count_value = 5000;
        }

        number varispeed_3 = this->maximum(
            1,
            1 + ((__gen_01_last_width_value == 0. ? 0. : __gen_01_pad_factor_value / __gen_01_last_width_value)) * saturation
        );

        number total_gain_4 = (varispeed_3 == 0. ? 0. : input_gain / varispeed_3);
        number x_1_5 = x_2 * total_gain_4;

        if (total_gain_4 < 1) {
            temp_0 * total_gain_4;
        }

        number bridge_rectifier_6 = rnbo_abs(x_1_5);
        number overspill_7 = 0;
        number target_width_8 = expansion;

        if (bridge_rectifier_6 < 0.01) {
            __gen_01_pad_factor_value = __gen_01_pad_factor_value * 0.9999;
        }

        if (bridge_rectifier_6 > 1.57079633) {
            bridge_rectifier_6 = 1.57079633;
            target_width_8 = 8;
        }

        number bridge_rectifier_2_9 = rnbo_sin(bridge_rectifier_6);

        if (x_1_5 > 0) {
            x_1_5 = bridge_rectifier_2_9;
            overspill_7 = temp_0 - bridge_rectifier_2_9;
        }

        if (x_1_5 < 0) {
            x_1_5 = -bridge_rectifier_2_9;
            overspill_7 = -temp_0 - bridge_rectifier_2_9;
        }

        number v_10 = overspill_7 * saturation;
        this->poke_default(this->gen_01_d, __gen_01_d_count_value + 5000, v_10, 0, 0);
        this->poke_default(this->gen_01_d, __gen_01_d_count_value, v_10, 0, 0);
        __gen_01_d_count_value = __gen_01_d_count_value - 1;
        __gen_01_pad_factor_value = __gen_01_pad_factor_value + v_10;

        if (target_width_8 * this->gen_01_noise_11_next() > __gen_01_last_width_value) {
            __gen_01_last_width_value = 1;
        } else {
            __gen_01_last_width_value = __gen_01_last_width_value - 1;

            if (__gen_01_last_width_value < 2) {
                __gen_01_last_width_value = 2;
            }

            number peek_3 = 0;
            number peek_4 = 0;
            array<SampleValue, 2> result_12 = this->peek_default(this->gen_01_d, __gen_01_d_count_value + __gen_01_last_width_value, 0);
            peek_4 = result_12[1];
            peek_3 = result_12[0];
            __gen_01_pad_factor_value = __gen_01_pad_factor_value - peek_3;
        }

        if (__gen_01_pad_factor_value < 0) {
            __gen_01_pad_factor_value = 0;
        }

        if (boost < 1) {
            x_1_5 = x_1_5 * boost;
        }

        number expr_5_13 = x_1_5;
        out1[(Index)i] = expr_5_13;
    }

    this->gen_01_d_count_value = __gen_01_d_count_value;
    this->gen_01_last_width_value = __gen_01_last_width_value;
    this->gen_01_pad_factor_value = __gen_01_pad_factor_value;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

number gen_01_d_count_getvalue() {
    return this->gen_01_d_count_value;
}

void gen_01_d_count_setvalue(number val) {
    this->gen_01_d_count_value = val;
}

void gen_01_d_count_reset() {
    this->gen_01_d_count_value = 0;
}

void gen_01_d_count_init() {
    this->gen_01_d_count_value = 0;
}

number gen_01_pad_factor_getvalue() {
    return this->gen_01_pad_factor_value;
}

void gen_01_pad_factor_setvalue(number val) {
    this->gen_01_pad_factor_value = val;
}

void gen_01_pad_factor_reset() {
    this->gen_01_pad_factor_value = 0;
}

void gen_01_pad_factor_init() {
    this->gen_01_pad_factor_value = 0;
}

number gen_01_last_width_getvalue() {
    return this->gen_01_last_width_value;
}

void gen_01_last_width_setvalue(number val) {
    this->gen_01_last_width_value = val;
}

void gen_01_last_width_reset() {
    this->gen_01_last_width_value = 0;
}

void gen_01_last_width_init() {
    this->gen_01_last_width_value = 0;
}

void gen_01_noise_11_init() {
    this->gen_01_noise_11_last = (Index)(systemticks() + this->voice() + this->random(0, 10000));
    this->gen_01_noise_11_recip = (number)2 / (number)8388607;
}

void gen_01_noise_11_seed(number v) {
    this->gen_01_noise_11_last = (Index)(v);
}

number gen_01_noise_11_next() {
    this->gen_01_noise_11_last = (Index)((BinOpInt)((UBinOpInt)(rnbo_imul(1664525, (int32_t)(this->gen_01_noise_11_last)) + 1013904223) | (UBinOpInt)0));
    number itemp = (BinOpInt)((UBinOpInt)0x007fffff & (UBinOpInt)this->gen_01_noise_11_last);
    return itemp * this->gen_01_noise_11_recip - 1.;
}

void gen_01_noise_11_reset() {
    this->gen_01_noise_11_last = 0;
    this->gen_01_noise_11_recip = 0;
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getTempo() {
    return this->globaltransport_getTempoAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState() {
    return this->globaltransport_getStateAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getBeatTimeAtMsTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState() == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(this->currenttime(), tempo);
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getTempo() != tempo) {
        MillisecondTime ct = this->currenttime();
        this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTimeAtMsTime(ct));
        this->globaltransport_beatTimeChanges->push(ct);

        fillSignal(
            this->globaltransport_tempo,
            this->vs,
            tempo,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastTempo = tempo;
        this->globaltransport_tempoNeedsReset = true;
        return true;
    }

    return false;
}

number globaltransport_getBeatTime() {
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime());
}

bool globaltransport_setState(number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(this->currenttime(), TransportState(state));
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getState() != state) {
        fillSignal(
            this->globaltransport_state,
            this->vs,
            state,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastState = TransportState(state);
        this->globaltransport_stateNeedsReset = true;

        if (state == 0) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime());
            this->globaltransport_beatTimeChanges->push(this->currenttime());
        }

        return true;
    }

    return false;
}

bool globaltransport_setBeatTime(number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(this->currenttime(), beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime());
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(this->currenttime());
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignatureAtMsTime(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignature() {
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime());
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime() + msOffset);
}

bool globaltransport_setTimeSignature(number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(this->currenttime(), (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature();

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(this->currenttime());
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    gen_01_in1 = 0;
    gen_01_mix = 0.5;
    gen_01_boost = 0;
    gen_01_expansion = 50;
    gen_01_saturation = 1;
    gen_01_input_gain = 0;
    param_01_value = 1;
    param_02_value = 5;
    param_03_value = 0.5;
    param_04_value = 2500;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    gen_01_d_count_value = 0;
    gen_01_pad_factor_value = 0;
    gen_01_last_width_value = 0;
    gen_01_noise_11_last = 0;
    param_01_lastValue = 0;
    param_02_lastValue = 0;
    param_03_lastValue = 0;
    param_04_lastValue = 0;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number gen_01_in1;
    number gen_01_mix;
    number gen_01_boost;
    number gen_01_expansion;
    number gen_01_saturation;
    number gen_01_input_gain;
    number param_01_value;
    number param_02_value;
    number param_03_value;
    number param_04_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number gen_01_d_count_value;
    number gen_01_pad_factor_value;
    number gen_01_last_width_value;
    Float64BufferRef gen_01_d;
    Index gen_01_noise_11_last;
    number gen_01_noise_11_recip;
    number param_01_lastValue;
    number param_02_lastValue;
    number param_03_lastValue;
    number param_04_lastValue;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef d;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

