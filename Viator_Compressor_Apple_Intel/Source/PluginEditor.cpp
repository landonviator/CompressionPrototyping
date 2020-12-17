/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Viator_Compressor_Mac_IntelAudioProcessorEditor::Viator_Compressor_Mac_IntelAudioProcessorEditor (Viator_Compressor_Mac_IntelAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    int leftMargin = 64;
    int topMargin = 80;
    sliders.reserve(4);
    sliders = {
        &inputGainSlider, &thresholdSlider, &releaseSlider, &outputGainSlider
    };
    
    sliderTracks.reserve(4);
    sliderTracks = {
        &inputGainSliderTrack, &thresholdSliderTrack, &releaseSliderTrack, &outputGainSliderTrack
    };
    
    //Sliders
    for (size_t i {0}; i < sliders.size(); i++) {
        addAndMakeVisible(sliders[i]);
        sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 256, 32);
        sliders[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        sliders[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(1, 1, 1, 0.0f));
        sliders[i]->setLookAndFeel(&otherLookAndFeel);
        
        //Slider Tracks
        addAndMakeVisible(sliderTracks[i]);
        sliderTracks[i]->addListener(this);
        sliderTracks[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sliderTracks[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 128, 24);
        sliderTracks[i]->setDoubleClickReturnValue(true, 0);
        sliderTracks[i]->setNumDecimalPlacesToDisplay(1);
        sliderTracks[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
        sliderTracks[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
        sliderTracks[i]->setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
        sliderTracks[i]->setColour(0x1001312, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
        sliderTracks[i]->setColour(0x1001311, juce::Colour::fromFloatRGBA(.2, .77, 1, 0));
        sliderTracks[i]->setBounds(leftMargin + 8, leftMargin + 4, 130, 130);
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setRange(-48.0f, 48.0f, 0.5f);
            sliderTracks[i]->setRange(-48.0f, 48.0f, 0.5f);
            sliderTracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0.0f);
            inputGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, inputGainSliderId, inputGainSlider);
            
        } else if (sliders[i] == &thresholdSlider){
            sliders[i]->setRange(0.0f, 60.0f, 0.5f);
            sliderTracks[i]->setRange(0.0f, 60.0f, 0.5f);
            sliderTracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0);
            thresholdSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, threshSliderId, thresholdSlider);

        } else if (sliders[i] == &releaseSlider){
            sliders[i]->setRange(0.0f, 100.0f, 1.0f);
            sliderTracks[i]->setRange(0.0f, 100.0f, 1.0f);
            sliderTracks[i]->setTextValueSuffix(" %");
            sliders[i]->setTextValueSuffix(" Ms");
            sliders[i]->setDoubleClickReturnValue(true, 100);
            releaseSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, releaseSliderId, releaseSlider);

        } else {
            sliders[i]->setRange(-48.0f, 48.0f, 0.5f);
            sliderTracks[i]->setRange(-48.0f, 48.0f, 0.5f);
            sliderTracks[i]->setTextValueSuffix(" dB");
            sliders[i]->setTextValueSuffix(" dB");
            sliders[i]->setDoubleClickReturnValue(true, 0.0f);
            outputGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, outputGainSliderId, outputGainSlider);
        }
        
        if (sliders[i] == &inputGainSlider){
            sliders[i]->setBounds(leftMargin, topMargin, 145, 145);
            sliderTracks[i]->setBounds(leftMargin + 6, topMargin + 2, 133, 133);
        } else {
            sliders[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth(), topMargin, 145, 145);
            sliderTracks[i]->setBounds(sliders[i - 1]->getX() +  sliders[i - 1]->getWidth() + 6, topMargin + 2, 133, 133);
        }
    }
    
    //SliderTracks
    //for (int i = 0; i < sliderTracks.size(); i++) {
//        addAndMakeVisible(sliderTracks[i]);
//        sliderTracks[i]->addListener(this);
//        sliderTracks[i]->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//        sliderTracks[i]->setRange(-48.0f, 48.0f, 0.5f);
//        sliderTracks[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 128, 24);
//        sliderTracks[i]->setTextValueSuffix(" dB");
//        sliderTracks[i]->setDoubleClickReturnValue(true, 0);
//        sliderTracks[i]->setNumDecimalPlacesToDisplay(1);
//        sliderTracks[i]->setColour(0x1001700, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
//        sliderTracks[i]->setColour(0x1001400, juce::Colour::fromFloatRGBA(.2, .77, 1, 0.75));
//        sliderTracks[i]->setColour(0x1001300, juce::Colour::fromFloatRGBA(1, 1, 1, 0));
//        sliderTracks[i]->setColour(0x1001312, juce::Colour::fromFloatRGBA(0, 0, 0, .25));
//        sliderTracks[i]->setColour(0x1001311, juce::Colour::fromFloatRGBA(.2, .77, 1, 0));
//        sliderTracks[i]->setBounds(leftMargin + 8, leftMargin + 4, 130, 130);
   // }
    
    addAndMakeVisible(inputLabel);
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.attachToComponent(&inputGainSlider, false);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
//    addAndMakeVisible(ratioLabel);
//    ratioLabel.setText("Ratio", juce::dontSendNotification);
//    ratioLabel.attachToComponent(&ratioSlider, false);
//    ratioLabel.setJustificationType(juce::Justification::centred);
//    ratioLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(threshLabel);
    threshLabel.setText("Compression", juce::dontSendNotification);
    threshLabel.attachToComponent(&thresholdSlider, false);
    threshLabel.setJustificationType(juce::Justification::centred);
    threshLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
//    addAndMakeVisible(attackLabel);
//    attackLabel.setText("Attack", juce::dontSendNotification);
//    attackLabel.attachToComponent(&attackSlider, false);
//    attackLabel.setJustificationType(juce::Justification::centred);
//    attackLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Edge", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputGainSlider, false);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setColour(0x1000281, juce::Colour::fromFloatRGBA(1, 1, 1, 0.5f));
    
    setSize (708, 273);
}

Viator_Compressor_Mac_IntelAudioProcessorEditor::~Viator_Compressor_Mac_IntelAudioProcessorEditor()
{
}

//==============================================================================
void Viator_Compressor_Mac_IntelAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromFloatRGBA(0.14f, 0.16f, 0.2f, 1.0));

}

void Viator_Compressor_Mac_IntelAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Viator_Compressor_Mac_IntelAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    for (size_t i {0}; i < sliders.size(); i++) {
        if (slider == sliderTracks[i]){
            sliders[i]->setValue(sliderTracks[i]->getValue());
        }
    }
}

