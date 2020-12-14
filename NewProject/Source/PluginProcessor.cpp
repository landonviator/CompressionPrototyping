/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
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

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    params.reserve(10);
    
    
    auto inputGainParam = std::make_unique<juce::AudioParameterFloat>(inputGainSliderId, inputGainSliderName, -48, 48, 0);
    auto ratioParam = std::make_unique<juce::AudioParameterInt>(ratioSliderId, ratioSliderName, 1, 20, 1);
    auto threshParam = std::make_unique<juce::AudioParameterFloat>(threshSliderId, threshSliderName, -60.0f, 0.0f, 0.0f);
    auto attackParam = std::make_unique<juce::AudioParameterInt>(attackSliderId, attackSliderName, 10, 250, 250);
    auto releaseParam = std::make_unique<juce::AudioParameterInt>(releaseSliderId, releaseSliderName, 10, 5000, 5000);
    auto outputGainParam = std::make_unique<juce::AudioParameterFloat>(outputGainSliderId, outputGainSliderName, -48, 48, 0);
    
    
    params.push_back(std::move(inputGainParam));
    params.push_back(std::move(ratioParam));
    params.push_back(std::move(threshParam));
    params.push_back(std::move(attackParam));
    params.push_back(std::move(releaseParam));
    params.push_back(std::move(outputGainParam));
    

    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    compressorProcessor.prepare(spec);
    inputGainProcessor.prepare(spec);
    outputGainProcessor.prepare(spec);
    
    compressorProcessor.setRatio(4.0f);
    compressorProcessor.setAttack(250.0f);
    
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    
    auto* rawGain = treeState.getRawParameterValue(inputGainSliderId);
    inputGainProcessor.setGainDecibels(*rawGain);
    
    //auto* rawRatio = treeState.getRawParameterValue(ratioSliderId);
    auto* rawThresh = treeState.getRawParameterValue(threshSliderId);
    //auto* rawAttack = treeState.getRawParameterValue(attackSliderId);
    auto* rawRelease = treeState.getRawParameterValue(releaseSliderId);
    //compressorProcessor.setRatio(*rawRatio);
    compressorProcessor.setThreshold(*rawThresh);
    //compressorProcessor.setAttack(*rawAttack);
    compressorProcessor.setRelease(*rawRelease);
    
    auto* rawOutputGain = treeState.getRawParameterValue(outputGainSliderId);
//    if (*rawThresh <= -30.0f){
//        *rawOutputGain = abs(*rawThresh) * .5;
//        std::cout << *rawOutputGain << std::endl;
//    }
    outputGainProcessor.setGainDecibels(*rawOutputGain);

    inputGainProcessor.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    compressorProcessor.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    outputGainProcessor.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
