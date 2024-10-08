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

  int currentValue{0};

  juce::MidiBuffer processedBuffer;

  void process(juce::MidiBuffer& midiMessages, const int negativeValue, const int positiveValue)
  {
    processedBuffer.clear();
    for (const auto m : midiMessages)
    {
      auto msg = m.getMessage();
      if (msg.isPitchWheel())
      {
        currentValue = msg.getPitchWheelValue();
        if ( currentValue<positiveValue && currentValue>negativeValue)
        {
          processedBuffer.addEvent(juce::MidiMessage::pitchWheel(msg.getChannel(),8192),m.samplePosition+1);
          //std::cout << "FILTER" << std::endl;
        }
        else
        {
          if (currentValue>8192)
            processedBuffer.addEvent(juce::MidiMessage::pitchWheel(msg.getChannel(),juce::jmap<int>(currentValue,positiveValue,16383,8193,16383)),m.samplePosition+1);
          else
            processedBuffer.addEvent(juce::MidiMessage::pitchWheel(msg.getChannel(),juce::jmap<int>(currentValue,0,negativeValue,0,8191)),m.samplePosition+1);
          // processedBuffer.addEvent(msg,m.samplePosition);  
        }
      }
      else
        processedBuffer.addEvent(msg,m.samplePosition);
    }
    midiMessages.swapWith(processedBuffer);
  }
};
