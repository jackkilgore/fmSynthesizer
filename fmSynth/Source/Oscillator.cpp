/*
  ==============================================================================

    Oscillator.cpp
    Created: 20 Feb 2020 8:44:34pm
    Author:  Tom

  ==============================================================================
*/

#include "Oscillator.h"

//=====Generic Voice Implementation==========

void OscillatorVoice::setAngleDelta(float freq)
{
    auto cyclesPerSample = freq / getSampleRate();
    angleDelta = cyclesPerSample * twoPi;
    
}

/**
 This is where we set up all the parameters needed when a note is pressed.
 */
void OscillatorVoice::startNote(int midiNoteNumber, float velocity,
    SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
{
    previousAngle = 0.0;
    nextSample = 0.0;
    currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    currentOctave = (int)(*params->getRawParameterValue(OCTAVE_ID)); // 1, 2 ,3, or 4
    auto octave = (float)pow(2, currentOctave-2);

    auto cyclesPerSecond = octave * MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();
    
    twoPi = 2.0 * MathConstants<double>::pi;
    angleDelta = cyclesPerSample * twoPi;
    delta = cyclesPerSample * 2.0;
}

/**
 Little decay envelope, SEE MIDI Synth tutorial.
 */
void OscillatorVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0)
            tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

/**
 Render AUDIO for processing block.
 */
void OscillatorVoice::renderNextBlock(AudioBuffer<float>& outputBuffer,
    int startSample, int numSamples)
{
    if (angleDelta != 0.0)
    {
        parameterUpdate();
        if (tailOff > 0.0) // [7]
        {
            while (--numSamples >= 0)
            {
                if(carrier)
                    setAngleDelta(frequency + ModBuffer->getSample(0,startSample));
                
                auto currentSample = (float)(generateSample(currentAngle) * level * tailOff);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                angleCap();
                ++startSample;

                tailOff *= 0.99; // [8]

                if (tailOff <= 0.005)
                {
                    clearCurrentNote(); // [9]

                    angleDelta = 0.0;
                    break;
                }
            }
        }
        else
        {
            while (--numSamples >= 0) // [6]
            {
                if(carrier)
                    setAngleDelta(frequency + ModBuffer->getSample(0,startSample));
                
                auto currentSample = (float)(generateSample(currentAngle) * level);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                angleCap();
                ++startSample;
            }
        }
    }
}

float OscillatorVoice::generateSample(float angle)
{
    switch ((int)(*params->getRawParameterValue(CARRIER_WAVE_ID)))
    {
    case(SQUARE):
        return squareWave(angle);
    case(SAW):
        return sawWave(angle);
    case(SINE):
    default:
           return sineWave(angle);
    }
}
float OscillatorVoice::sineWave(float angle)
{
    return std::sin(angle);
}
float OscillatorVoice::squareWave(float angle)
{
    return std::tanh(std::sin(angle) * 30.0);
}
float OscillatorVoice::sawWave(float angle)
{
    if (angle < previousAngle) 
    {
        nextSample = -1.0;
        previousAngle = angle;
        return nextSample;
    }
    nextSample += delta;
    previousAngle = angle;
    return nextSample;
}

void OscillatorVoice::angleCap()
{
    if (currentAngle >= twoPi)
    {
        currentAngle -= twoPi;
    }
}

void OscillatorVoice::parameterUpdate()
{
    // check octave
    int change = (int)(*params->getRawParameterValue(OCTAVE_ID)) - currentOctave;
    if (change != 0)
    {
        float adjustment = (float)pow(2, change);
        delta *= adjustment;
        angleDelta *= adjustment;
        currentOctave = change + currentOctave;
    }

    // TODO : check frequency from Modulator here (do we do this before or after octave???)
}
