/*
  ==============================================================================

    SineOsc.h
    Created: 10 Feb 2020 9:48:07pm
    Author:  Torrance Cui

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


struct SineSound : public SynthesiserSound
{
    
    SineSound() {}    
    
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
    

    SineSound() {}

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }

};

class SineVoice : public SynthesiserVoice
{
public:

    SineVoice() {}
    
    bool canPlaySound (SynthesiserSound* sound) override

    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<SineSound*> (sound) != nullptr;
    }
    
//    DUMMY FUNCTION 
//    void setParam1(float* param1);
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* sound, int /*currentPitchWheelPosition*/) override;
    
    void stopNote (float /*velocity*/, bool allowTailOff) override;
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    

    //    DUMMY FUNCTION 
    //    void setParam1(float* param1);

    void startNote(int midiNoteNumber, float velocity,
        SynthesiserSound* sound, int /*currentPitchWheelPosition*/) override;

    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
};
