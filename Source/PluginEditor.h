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
class PitchBendGateAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PitchBendGateAudioProcessorEditor (PitchBendGateAudioProcessor&);
    ~PitchBendGateAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PitchBendGateAudioProcessor& audioProcessor;

    juce::Slider gatePositiveValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gatePositiveValueAttachment;
    juce::Label gatePositiveValueLabel{"gatePositiveValueLabel", "Gate positive value"};

    juce::Slider gateNegativeValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gateNegativeValueAttachment;
    juce::Label gateNegativeValueLabel{"gateNegativeValueLabel", "Gate negative value"};

    juce::Image logo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchBendGateAudioProcessorEditor)
};
