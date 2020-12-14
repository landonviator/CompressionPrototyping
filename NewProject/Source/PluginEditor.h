/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    
    juce::Slider inputGainSlider, ratioSlider, thresholdSlider, attackSlider, releaseSlider, outputGainSlider;
    std::vector<juce::Slider*> sliders;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> inputGainSliderAttach, ratioSliderAttach, thresholdSliderAttach, attackSliderAttach, releaseSliderAttach, outputGainSliderAttach;
    
    juce::Label inputLabel, ratioLabel, threshLabel, attackLabel, releaseLabel, outputLabel;
    
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
