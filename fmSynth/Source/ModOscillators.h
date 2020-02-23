//
//  ModOscillators.h
//  fmSynth
//
//  Created by Jack Kilgore on 2/21/20.
//

#pragma one
#include "Oscillator.h"

struct ModOscSound_0 : public OscillatorSound
{

    ModOscSound_0() {}

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }

};


class ModOscVoice_0 : public OscillatorVoice
{
public:

    ModOscVoice_0() {
        params = NULL;
    }
    ModOscVoice_0(AudioProcessorValueTreeState* params) : OscillatorVoice(params){}

    bool canPlaySound(SynthesiserSound* sound) override
    {
       return dynamic_cast<ModOscSound_0*> (sound) != nullptr;
    }
    
    void setCarrFrequency(float carrFrequency){carrierFrequency = carrFrequency;}
    
    void setModParams();
    
    void setInitParams() override;
        
protected:
    void parameterUpdate() override;

private:
    
    float carrierFrequency, multipleOfCarrier, detune, amount;
    int rawMultiple;
};
