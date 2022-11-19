/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PealldelayAudioProcessor::PealldelayAudioProcessor()
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

PealldelayAudioProcessor::~PealldelayAudioProcessor()
{
}

//==============================================================================
const juce::String PealldelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PealldelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PealldelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PealldelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PealldelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PealldelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PealldelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PealldelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PealldelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void PealldelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PealldelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    window[0] = std::vector<float>();
    window[1] = std::vector<float>();

    double sr = this->getSampleRate();
    max_delay_length = (unsigned int)(max_delay_time / 1000. * sr);
    delay_length = (unsigned int)(0.5 * sr);

    window[0].resize(max_delay_length);
    window[1].resize(max_delay_length);

    for (size_t i = 0; i < max_delay_length; i++)
    {
        window[0][i] = 0;
        window[1][i] = 0;
    }
}

void PealldelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PealldelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PealldelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int buf_len = buffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        int* w = &w_i[channel];
        std::vector<float>* win = &window[channel];

        for (int i = 0; i < buf_len; i++) {
            (*win)[*w] += channelData[i];
            (*win)[*w] *= feedback;
            channelData[i] = (1 - dry_wet_ratio) * (*win)[(*w + 1) % delay_length] + channelData[i] * dry_wet_ratio;
            *w = ((*w+1) % delay_length); // in this way overflow is avoided
        }
    }
}

//==============================================================================
bool PealldelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PealldelayAudioProcessor::createEditor()
{
    return new PelladelayAudioProcessorEditor (*this);
}

//==============================================================================
void PealldelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PealldelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PealldelayAudioProcessor();
}
