/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Viator_Compressor_Mac_SiliconAudioProcessor::Viator_Compressor_Mac_SiliconAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

Viator_Compressor_Mac_SiliconAudioProcessor::~Viator_Compressor_Mac_SiliconAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Viator_Compressor_Mac_SiliconAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(4);
    
    
    auto inputGainParam = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -48, 48, 0);
    //auto ratioParam = std::make_unique<juce::AudioParameterInt>(ratioSliderId, ratioSliderName, 1, 20, 1);
    auto threshParam = std::make_unique<juce::AudioParameterFloat>(threshSliderId, threshSliderName, 0.0f, 60.0f, 0.0f);
    //auto attackParam = std::make_unique<juce::AudioParameterInt>(attackSliderId, attackSliderName, 10, 250, 250);
    auto releaseParam = std::make_unique<juce::AudioParameterFloat>(releaseSliderId, releaseSliderName, 0.0f, 100.0f, 0.0f);
    auto outputGainParam = std::make_unique<juce::AudioParameterFloat>(outputGainSliderId, outputGainSliderName, -48, 48, 0);
    
    params.push_back(std::move(inputGainParam));
    //params.push_back(std::move(ratioParam));
    params.push_back(std::move(threshParam));
    //params.push_back(std::move(attackParam));
    params.push_back(std::move(releaseParam));
    params.push_back(std::move(outputGainParam));
    

    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Viator_Compressor_Mac_SiliconAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Viator_Compressor_Mac_SiliconAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Viator_Compressor_Mac_SiliconAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Viator_Compressor_Mac_SiliconAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Viator_Compressor_Mac_SiliconAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Viator_Compressor_Mac_SiliconAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Viator_Compressor_Mac_SiliconAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Viator_Compressor_Mac_SiliconAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Viator_Compressor_Mac_SiliconAudioProcessor::getProgramName (int index)
{
    return {};
}

void Viator_Compressor_Mac_SiliconAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Viator_Compressor_Mac_SiliconAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    compressorProcessor.prepare(spec);
    compressorProcessor.get<1>().setAttack(1000.0f);
    compressorProcessor.get<1>().setRatio(4.0f);
    
}

void Viator_Compressor_Mac_SiliconAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Viator_Compressor_Mac_SiliconAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void Viator_Compressor_Mac_SiliconAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    auto* rawGain = treeState.getRawParameterValue(inputGainSliderId);
    compressorProcessor.get<0>().setGainDecibels(*rawGain);

    auto* rawThresh = treeState.getRawParameterValue(threshSliderId);
    compressorProcessor.get<1>().setThreshold(scaleRange(*rawThresh, 0.0f, 60.0f, -24.0f, -48.0f));

    auto* rawRelease = treeState.getRawParameterValue(releaseSliderId);
    compressorProcessor.get<1>().setRelease(scaleRange(*rawRelease, 0.0f, 100.0f, 1700.0f, 400.0f));

    auto* rawOutputGain = treeState.getRawParameterValue(outputGainSliderId);
    compressorProcessor.get<2>().setGainDecibels((*rawOutputGain) + (*rawThresh * 0.25f) - (*rawRelease * 0.06f));
    //compressorProcessor.get<2>().setGainDecibels(*rawOutputGain);

    juce::dsp::AudioBlock<float> audioBlock (buffer);
    compressorProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

float Viator_Compressor_Mac_SiliconAudioProcessor::scaleRange(float input, float inputLow, float inputHigh, float outputLow, float outputHigh){
    return ((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow;
}

//==============================================================================
bool Viator_Compressor_Mac_SiliconAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Viator_Compressor_Mac_SiliconAudioProcessor::createEditor()
{
    return new Viator_Compressor_Mac_SiliconAudioProcessorEditor (*this);
}

//==============================================================================
void Viator_Compressor_Mac_SiliconAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void Viator_Compressor_Mac_SiliconAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData (data, size_t (sizeInBytes));
    if (tree.isValid()) {
        treeState.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Viator_Compressor_Mac_SiliconAudioProcessor();
}
