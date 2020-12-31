/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define inputGainSliderId "inputGain"
#define inputGainSliderName "Input Gain"

#define ratioSliderId "ratio"
#define ratioSliderName "Ratio"

#define threshSliderId "thresh"
#define threshSliderName "Compression"

#define attackSliderId "attack"
#define attackSliderName "Attack"

#define releaseSliderId "release"
#define releaseSliderName "Edge"

#define outputGainSliderId "outputGain"
#define outputGainSliderName "Output Gain"

//==============================================================================
/**
*/
class Viator_Compressor_Mac_SiliconAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Viator_Compressor_Mac_SiliconAudioProcessor();
    ~Viator_Compressor_Mac_SiliconAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float scaleRange (float input, float inputLow, float inputHigh, float outputLow, float outputHigh);
    
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    //juce::dsp::Compressor<float> compressorProcessor;
    //juce::dsp::Gain<float> inputGainProcessor;
    //juce::dsp::Gain<float> outputGainProcessor;
    
    
    using InputGainProcessor = juce::dsp::Gain<float>;
    using CompressorProcessor = juce::dsp::Compressor<float>;
    using OutputGainProcessor = juce::dsp::Gain<float>;

    juce::dsp::ProcessorChain<InputGainProcessor, CompressorProcessor, OutputGainProcessor> compressorProcessor;

    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Viator_Compressor_Mac_SiliconAudioProcessor)
};
