/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FmSynthAudioProcessor::FmSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    valTreeState(*this, nullptr, "PARAMETERS", createParameterLayout()),
    synth(keyboardState),
    lfo_1(&valTreeState,1),
    lfo_2(&valTreeState,2),
    lfo_3(&valTreeState,3),
    lfo_4(&valTreeState,4)
#endif
{}

FmSynthAudioProcessor::~FmSynthAudioProcessor()
{
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout FmSynthAudioProcessor::createParameterLayout()
{
    // all the parameters should be set here!

    NormalisableRange<float> cutoffRange(0.0, 20000.0, 1.0, .3, false);

    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    auto gain = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, 0.0f, 1.0f, 1.0f);
    auto cutoff = std::make_unique<AudioParameterFloat>(FILTER_CUTOFF_ID, FILTER_CUTOFF_NAME, cutoffRange, 20000.0f);
    auto carrierWave = std::make_unique<AudioParameterInt>(CARRIER_WAVE_ID, CARRIER_WAVE_NAME, 1, 5, 1);
    auto carrierOctave = std::make_unique<AudioParameterInt>(OCTAVE_ID, OCTAVE_NAME, 1, 4, 2);
    auto modWave = std::make_unique<AudioParameterInt>(MOD_WAVE_ID, MOD_WAVE_NAME, 1, 5, 1);
    auto modMultiple = std::make_unique<AudioParameterInt>(MOD_MULTIPLE_ID, MOD_MULTIPLE_NAME, -5, 5, 2);
    auto modDetune = std::make_unique<AudioParameterFloat>(MOD_DETUNE_ID, MOD_DETUNE_NAME, 0.01f, 2000.0f, 0.0f);
    auto modAmt = std::make_unique<AudioParameterFloat>(MOD_AMT_ID, MOD_AMT_NAME, 0.0f, 1000.0f, 20.0f);
    
    auto lfo_gain           = std::make_unique<AudioParameterInt>(LFO_GAIN_ID, LFO_GAIN_NAME, 0, 4, 0);
    auto lfo_cutoff         = std::make_unique<AudioParameterInt>(LFO_FILTER_CUTOFF_ID, LFO_FILTER_CUTOFF_NAME, 0, 4, 0);
    auto lfo_carrierWave    = std::make_unique<AudioParameterInt>(LFO_CARRIER_WAVE_ID, LFO_CARRIER_WAVE_NAME, 0, 4, 0);
    auto lfo_carrierOctave  = std::make_unique<AudioParameterInt>(LFO_OCTAVE_ID, LFO_OCTAVE_NAME, 0, 4, 0);
    auto lfo_modWave        = std::make_unique<AudioParameterInt>(LFO_MOD_WAVE_ID, LFO_MOD_WAVE_NAME, 0, 4, 0);
    auto lfo_modMultiple    = std::make_unique<AudioParameterInt>(LFO_MOD_MULTIPLE_ID, LFO_MOD_MULTIPLE_NAME, 0, 4, 0);
    auto lfo_modDetune      = std::make_unique<AudioParameterInt>(LFO_MOD_DETUNE_ID, LFO_MOD_DETUNE_NAME, 0, 4, 0);
    auto lfo_modAmt         = std::make_unique<AudioParameterInt>(LFO_MOD_AMT_ID, LFO_MOD_AMT_NAME, 0, 4, 0);
    
    auto LFO_freq_1 = std::make_unique<AudioParameterFloat>(LFO_FREQ_ID_1, LFO_FREQ_NAME_1, 0.01, 100.0, 15.0);
    auto LFO_freq_2 = std::make_unique<AudioParameterFloat>(LFO_FREQ_ID_2, LFO_FREQ_NAME_2, 0.01, 100.0, 15.0);
    auto LFO_freq_3 = std::make_unique<AudioParameterFloat>(LFO_FREQ_ID_3, LFO_FREQ_NAME_3, 0.01, 100.0, 15.0);
    auto LFO_freq_4 = std::make_unique<AudioParameterFloat>(LFO_FREQ_ID_4, LFO_FREQ_NAME_4, 0.01, 100.0, 15.0);
    
    auto LFO_amt_1 = std::make_unique<AudioParameterFloat>(LFO_AMT_ID_1, LFO_AMT_NAME_1, 0.0, 1.0, .25);
    auto LFO_amt_2 = std::make_unique<AudioParameterFloat>(LFO_AMT_ID_2, LFO_AMT_NAME_2, 0.0, 1.0, .25);
    auto LFO_amt_3 = std::make_unique<AudioParameterFloat>(LFO_AMT_ID_3, LFO_AMT_NAME_3, 0.0, 1.0, .25);
    auto LFO_amt_4 = std::make_unique<AudioParameterFloat>(LFO_AMT_ID_4, LFO_AMT_NAME_4, 0.0, 1.0, .25);
    
    auto LFO_val_1 = std::make_unique<AudioParameterFloat>(LFO_VAL_ID_1, LFO_VAL_NAME_1, -1.0f, 1.0f, 0.0f);
    auto LFO_val_2 = std::make_unique<AudioParameterFloat>(LFO_VAL_ID_2, LFO_VAL_NAME_2, -1.0f, 1.0f, 0.0f);
    auto LFO_val_3 = std::make_unique<AudioParameterFloat>(LFO_VAL_ID_3, LFO_VAL_NAME_3, -1.0f, 1.0f, 0.0f);
    auto LFO_val_4 = std::make_unique<AudioParameterFloat>(LFO_VAL_ID_4, LFO_VAL_NAME_4, -1.0f, 1.0f, 0.0f);
    
    params.push_back(std::move(gain));
    params.push_back(std::move(cutoff));
    params.push_back(std::move(carrierWave));
    params.push_back(std::move(carrierOctave));
    params.push_back(std::move(modWave));
    params.push_back(std::move(modMultiple));
    params.push_back(std::move(modDetune));
    params.push_back(std::move(modAmt));
    
    params.push_back(std::move(lfo_gain));
    params.push_back(std::move(lfo_cutoff));
    params.push_back(std::move(lfo_carrierWave));
    params.push_back(std::move(lfo_carrierOctave));
    params.push_back(std::move(lfo_modWave));
    params.push_back(std::move(lfo_modMultiple));
    params.push_back(std::move(lfo_modDetune));
    params.push_back(std::move(lfo_modAmt));
    
    params.push_back(std::move(LFO_freq_1));
    params.push_back(std::move(LFO_freq_2));
    params.push_back(std::move(LFO_freq_3));
    params.push_back(std::move(LFO_freq_4));
    params.push_back(std::move(LFO_amt_1));
    params.push_back(std::move(LFO_amt_2));
    params.push_back(std::move(LFO_amt_3));
    params.push_back(std::move(LFO_amt_4));
    params.push_back(std::move(LFO_val_1));
    params.push_back(std::move(LFO_val_2));
    params.push_back(std::move(LFO_val_3));
    params.push_back(std::move(LFO_val_4));

    return {params.begin(), params.end()};
}


const String FmSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FmSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool FmSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool FmSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double FmSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FmSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FmSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FmSynthAudioProcessor::setCurrentProgram(int index)
{
}

const String FmSynthAudioProcessor::getProgramName(int index)
{
    return {};
}

void FmSynthAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void FmSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.prepareToPlay(sampleRate, samplesPerBlock);
    /**
     Clear Voice and Sound Buffer for polySynth instance.
     */
    synth.clearVoices();
    synth.clearSounds();

    synth.addVoice<FMVoice, FMSound>(12, &valTreeState);
}

void FmSynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FmSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void FmSynthAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
    buffer.clear();

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
        buffer.clear(i, 0, buffer.getNumSamples());

    // WIP, NOT GENERAL ENOUGH -- Will be used to pass parameters to PolySynth.
    for (auto i = 0; i < synth.getNumVoices(); i++)
    {
        if ((tempVoice = dynamic_cast<OscillatorVoice*>(synth.getVoice(i))))
        {
        }
    }

    /**
        Render poly synth. NOTE does not support parameter changes of anything but pitch... YET
     */
    synth.renderNextAudioBlock(buffer, 0, buffer.getNumSamples(), midiMessages);

    filterCutoff = *valTreeState.getRawParameterValue(FILTER_CUTOFF_ID);
    filterL.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), filterCutoff));
    filterR.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), filterCutoff));
    filterL.processSamples(buffer.getWritePointer(0, 0), buffer.getNumSamples());
    filterR.processSamples(buffer.getWritePointer(1, 0), buffer.getNumSamples());

    //gain rescales the volume setting to be from 0 to 1
    gain = *valTreeState.getRawParameterValue(GAIN_ID);

    //loop through every channel and then each buffer to adjust the volume
    for (auto channel = 0; channel < buffer.getNumChannels(); channel++) {
        auto* channelBuffer = buffer.getWritePointer(channel);

        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
            channelBuffer[sample] *= gain;
        }
    }
}

//==============================================================================
bool FmSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FmSynthAudioProcessor::createEditor()
{
    return new FmSynthAudioProcessorEditor(*this, valTreeState);
}

//==============================================================================
void FmSynthAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FmSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FmSynthAudioProcessor();
}
