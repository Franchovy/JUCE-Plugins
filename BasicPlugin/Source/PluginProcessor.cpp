/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicPluginAudioProcessor::BasicPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

BasicPluginAudioProcessor::~BasicPluginAudioProcessor()
{
}

//==============================================================================
const String BasicPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BasicPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BasicPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void BasicPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    buffer.applyGain(gain);
}

//==============================================================================
bool BasicPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BasicPluginAudioProcessor::createEditor()
{
    return new BasicPluginAudioProcessorEditor (*this);
}

/**
 * Set the memory block to contain ValueTree of type "BasicPlugin" with the single parameter "gain"
 * @param destData
 */
void BasicPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ValueTree pluginData("BasicPlugin");
    pluginData.setProperty("gain", gain, nullptr);

    MemoryOutputStream outputData;
    pluginData.writeToStream(outputData);
    destData = outputData.getMemoryBlock();
}

void BasicPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    MemoryInputStream inputData(data, sizeInBytes, false);
    auto pluginData = ValueTree::readFromStream(inputData);

    if (pluginData.isValid() && pluginData.hasType("BasicPlugin") && pluginData.hasProperty("gain")) {
        gain = pluginData.getProperty("gain");

    } else {
        jassertfalse;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicPluginAudioProcessor();
}
