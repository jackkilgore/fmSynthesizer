/*
  ==============================================================================

    Oscillator.cpp
    Created: 20 Feb 2020 8:44:34pm
    Author:  Tom

  ==============================================================================
*/

#include "Oscillator.h"

//=====Generic Voice Implementation==========

/**
 This is where we set up all the parameters needed when a note is pressed.
 */
void OscillatorVoice::startNote(int midiNoteNumber, float velocity,
    SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
{
    waveType = 0; // TODO: hook this up to GUI

    previousAngle = 0.0;
    nextSample = 0.0;
    currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();

    delta = cyclesPerSample * 2.0;
    twoPi = 2.0 * MathConstants<double>::pi;
    angleDelta = cyclesPerSample * twoPi;
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
        if (tailOff > 0.0) // [7]
        {
            while (--numSamples >= 0)
            {
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
    switch (waveType) 
    {
    case(0):
        return sineWave(angle);
    case(1):
        return squareWave(angle);
    case(2):
        return sawWave(angle);
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