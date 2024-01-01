#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchBendGateAudioProcessor::PitchBendGateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PitchBendGateAudioProcessor::~PitchBendGateAudioProcessor()
{
}

//==============================================================================
const juce::String PitchBendGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PitchBendGateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchBendGateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchBendGateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PitchBendGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchBendGateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PitchBendGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchBendGateAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PitchBendGateAudioProcessor::getProgramName (int index)
{
    return {};
}

void PitchBendGateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PitchBendGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PitchBendGateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PitchBendGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void PitchBendGateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    pitchBendProcessor.process(midiMessages,
                                apvts.getRawParameterValue("Gate negative value")->load(),
                                apvts.getRawParameterValue("Gate positive value")->load());
    //pitchBendProcessor.process(midiMessages, apvts.getRawParameterValue("Gate positive value")->load());
}

//==============================================================================
bool PitchBendGateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PitchBendGateAudioProcessor::createEditor()
{
    return new PitchBendGateAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PitchBendGateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void PitchBendGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data,sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchBendGateAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PitchBendGateAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterInt>("Gate positive value","Gate positive value",0,8192,0));
    layout.add(std::make_unique<juce::AudioParameterInt>("Gate negative value","Gate negative value",-8192,0,0));
    return layout;
}
