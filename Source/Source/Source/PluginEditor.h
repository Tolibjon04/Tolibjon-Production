#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class TolibjonProductionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TolibjonProductionAudioProcessorEditor (TolibjonProductionAudioProcessor&);
    ~TolibjonProductionAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TolibjonProductionAudioProcessor& audioProcessor;

    juce::Slider compSlider;
    juce::Slider warmSlider;
    juce::Slider gainSlider;

    juce::Label compLabel;
    juce::Label warmLabel;
    juce::Label gainLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> warmAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TolibjonProductionAudioProcessorEditor)
};
