/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    sliders.reserve(6);
    sliders = {
        &inputGainSlider, &ratioSlider, &thresholdSlider, &attackSlider, &releaseSlider, &outputGainSlider
    };
    
    //Sliders
    for (size_t i {0}; i < sliders.size(); i++) {
        addAndMakeVisible(sliders[i]);
        sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 256, 32);
        sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setRange(-24.0f, 24.0f, 0.5f);
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0.0f);
            
        } else if (sliders[i] == &ratioSlider){
            sliders[i]->setRange(1, 20, 1);
            sliders[i]->setTextValueSuffix(" X");
            sliders[i]->setDoubleClickReturnValue(true, 1);
            
        } else if (sliders[i] == &thresholdSlider){
            sliders[i]->setRange(-60.0f, 0.0f, 0.5f);
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            
        } else if (sliders[i] == &attackSlider){
            sliders[i]->setRange(5, 100, 1);
            sliders[i]->setTextValueSuffix(" Ms");
            sliders[i]->setDoubleClickReturnValue(true, 100);
            
        } else if (sliders[i] == &releaseSlider){
            sliders[i]->setRange(5, 100, 1);
            sliders[i]->setTextValueSuffix(" Ms");
            sliders[i]->setDoubleClickReturnValue(true, 100);
            
        } else {
            sliders[i]->setRange(-24.0f, 24.0f, 0.5f);
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0.0f);
        }
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setBounds(64, 64, 64, 300);
        } else {
            sliders[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth() * 2, 64, 64, 300);
        }
        
        
    }
    
    addAndMakeVisible(inputLabel);
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.attachToComponent(&inputGainSlider, false);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(ratioLabel);
    ratioLabel.setText("Ratio", juce::dontSendNotification);
    ratioLabel.attachToComponent(&ratioSlider, false);
    ratioLabel.setJustificationType(juce::Justification::centred);
    ratioLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(threshLabel);
    threshLabel.setText("Thresh", juce::dontSendNotification);
    threshLabel.attachToComponent(&thresholdSlider, false);
    threshLabel.setJustificationType(juce::Justification::centred);
    threshLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, false);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputGainSlider, false);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    setSize (800, 400);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromFloatRGBA(0.14f, 0.16f, 0.2f, 1.0));
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    
}
