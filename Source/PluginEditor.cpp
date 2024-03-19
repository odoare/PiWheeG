/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchBendGateAudioProcessorEditor::PitchBendGateAudioProcessorEditor (PitchBendGateAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
  logo = juce::ImageCache::getFromMemory(BinaryData::logo686_png, BinaryData::logo686_pngSize);

  gateNegativeValue.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
  gateNegativeValue.setTextBoxStyle(juce::Slider::TextBoxBelow,true,100,20);
  gateNegativeValue.setTextBoxIsEditable(true);
  gateNegativeValue.setColour(juce::Slider::trackColourId, juce::Colours::green);
  gateNegativeValue.setColour(juce::Slider::backgroundColourId, juce::Colours::black); 
  gateNegativeValue.setColour(juce::Slider::thumbColourId, juce::Colours::green);  
  addAndMakeVisible(gateNegativeValue);
  gateNegativeValueAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Gate negative value",gateNegativeValue);
  addAndMakeVisible(gateNegativeValueLabel);
  gateNegativeValueLabel.setJustificationType(juce::Justification::centred);
  gateNegativeValueLabel.attachToComponent(&gateNegativeValue,false);

  gatePositiveValue.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
  gatePositiveValue.setTextBoxStyle(juce::Slider::TextBoxBelow,true,100,20);
  gatePositiveValue.setTextBoxIsEditable(true);
  gatePositiveValue.setColour(juce::Slider::trackColourId, juce::Colours::black);
  gatePositiveValue.setColour(juce::Slider::backgroundColourId, juce::Colours::green);  
  gatePositiveValue.setColour(juce::Slider::thumbColourId, juce::Colours::green);  
  addAndMakeVisible(gatePositiveValue);
  gatePositiveValueAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Gate positive value",gatePositiveValue);
  addAndMakeVisible(gatePositiveValueLabel);
  gatePositiveValueLabel.setJustificationType(juce::Justification::centred);
  gatePositiveValueLabel.attachToComponent(&gatePositiveValue,false);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (400, 200);
}

PitchBendGateAudioProcessorEditor::~PitchBendGateAudioProcessorEditor()
{
}

//==============================================================================
void PitchBendGateAudioProcessorEditor::paint (juce::Graphics& g)
{

    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/8;
    auto uy = (1-2*border)*getHeight()/4;

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    //auto bluegreengrey = juce::Colour::fromFloatRGBA(0.17f,0.22f,0.27f,1.0f);
    auto bluegreengrey = juce::Colour::fromFloatRGBA(0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker(), perpendicular * height,
                           bluegreengrey, perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();

    auto r = juce::Rectangle<float>(uxb+6*ux,uyb+1.8*uy,2*ux,2*uy*227/269);
    g.drawImage(logo, r);

    g.setColour(juce::Colours::grey);
    g.setFont(28);
    g.drawMultiLineText("PBG", uxb+4*ux, uyb+2.8*uy, 2*ux, juce::Justification::centred);
    g.setFont(14);
    g.drawMultiLineText("Pitch Bend Gate", uxb+4*ux, uyb+3.1*uy, 2*ux, juce::Justification::centred);
    g.setFont(16);
    g.drawMultiLineText("v0.01", uxb+4*ux, uyb+3.4*uy, 2*ux, juce::Justification::centred);

    g.setColour(juce::Colours::grey);
    g.setFont(14);
    g.drawMultiLineText("Usage: All pitchbend values the range defined by the left and right sliders are converted to the neutral value of 8192. Other values are remapped to fit in the range [0-16383]",
                          uxb+0.3*ux, uyb+2*uy, 3.5*ux, juce::Justification::horizontallyJustified);
}

void PitchBendGateAudioProcessorEditor::resized()
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/8;
    auto uy = (1-2*border)*getHeight()/4;
    
    gateNegativeValue.setBounds(uxb,uyb+0.5*uy,4*ux,uy);
    gatePositiveValue.setBounds(uxb+4*ux,uyb+0.5*uy,4*ux,uy);

}
