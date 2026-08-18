#include "PluginProcessor.h"
#include "PluginEditor.h"

TolibjonProductionAudioProcessor::TolibjonProductionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

TolibjonProductionAudioProcessor::~TolibjonProductionAudioProcessor() {}

const juce::String TolibjonProductionAudioProcessor::getName() const { return JucePlugin_Name; }
bool TolibjonProductionAudioProcessor::acceptsMidi() const { return false; }
bool TolibjonProductionAudioProcessor::producesMidi() const { return false; }
bool TolibjonProductionAudioProcessor::isMidiEffect() const { return false; }
double TolibjonProductionAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int TolibjonProductionAudioProcessor::getNumPrograms() { return 1; }
int TolibjonProductionAudioProcessor::getCurrentProgram() { return 0; }
void TolibjonProductionAudioProcessor::setCurrentProgram (int index) {}
const juce::String TolibjonProductionAudioProcessor::getProgramName (int index) { return {}; }
void TolibjonProductionAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

void TolibjonProductionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    compressor.prepare(spec);
    lowCut.prepare(spec);

    auto lowCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 80.0f);
    *lowCut.get<0>().coefficients = *lowCutCoefficients;
    *lowCut.get<1>().coefficients = *lowCutCoefficients;
}

void TolibjonProductionAudioProcessor::releaseResources() {}

bool TolibjonProductionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void TolibjonProductionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float compThresh = apvts.getRawParameterValue("COMPRESSION")->load();
    float warmDrive = apvts.getRawParameterValue("WARMTH")->load();
    float outGain = apvts.getRawParameterValue("GAIN")->load();

    compressor.setThreshold(compThresh);
    compressor.setRatio(4.0f);
    compressor.setAttack(15.0f);
    compressor.setRelease(100.0f);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    lowCut.process(context);
    compressor.process(context);

    // Warmth (Saturation) & Output Gain
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float input = channelData[sample];
            
            // Saturation logic
            float driveVal = 1.0f + (warmDrive / 20.0f);
            float driven = input * driveVal;
            float saturated = std::tanh(driven);

            // Apply gain (dB to linear)
            float linearGain = juce::Decibels::decibelsToGain(outGain);
            channelData[sample] = saturated * linearGain;
        }
    }
}

bool TolibjonProductionAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* TolibjonProductionAudioProcessor::createEditor()
{
    return new TolibjonProductionAudioProcessorEditor (*this);
}

void TolibjonProductionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TolibjonProductionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout TolibjonProductionAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSION", "Compression", juce::NormalisableRange<float>(-40.0f, 0.0f, 0.1f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("WARMTH", "Warmth", juce::NormalisableRange<float>(0.0f, 100.0f, 0.5f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN", "Output Gain", juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TolibjonProductionAudioProcessor();
}
