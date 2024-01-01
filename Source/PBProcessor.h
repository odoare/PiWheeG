/*
  ==============================================================================

    MidiProcessor.h
    Created: 31 Dec 2023 8:04:25pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
//#include <iostream>

class PBProcessor
{
public:

  juce::MidiBuffer processedBuffer;

  void process(juce::MidiBuffer& midiMessages, const int negativeValue, const int positiveValue)
  {
    processedBuffer.clear();
    for (const auto m : midiMessages)
    {
      auto msg = m.getMessage();
      if (msg.isPitchWheel())
      {
        if ( (msg.getPitchWheelValue())<positiveValue && (msg.getPitchWheelValue())>negativeValue)
        {
          processedBuffer.addEvent(juce::MidiMessage::pitchWheel(msg.getChannel(),8192),m.samplePosition+1);
          //std::cout << "FILTER" << std::endl;
        }
        else
          processedBuffer.addEvent(msg,m.samplePosition);
      }
      else
        processedBuffer.addEvent(msg,m.samplePosition);
    }
    midiMessages.swapWith(processedBuffer);
  }
};
