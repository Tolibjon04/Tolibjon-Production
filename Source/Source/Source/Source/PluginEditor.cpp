#include "PluginProcessor.h"
#include "PluginEditor.h"

TolibjonProductionAudioProcessorEditor::TolibjonProductionAudioProcessorEditor (TolibjonProductionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (450, 260);

    auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 20);
        addAndMakeVisible(slider);

        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(juce::FontOptions(14.0f, juce::Font::bold));
        label.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(label);
    };

    setupSlider(compSlider, compLabel, "COMPRESSION");
    setupSlider(warmSlider, warmLabel, "WARMTH");
    setupSlider(gainSlider, gainLabel, "GAIN");

    compAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMPRESSION", compSlider);
    warmAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WARMTH", warmSlider);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
}

TolibjonProductionAudioProcessorEditor::~TolibjonProductionAudioProcessorEditor() {}

void TolibjonProductionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1a1a1e));

    g.setColour (juce::Colour (0xff2d2d35));
    g.drawRect (getLocalBounds(), 2);

    g.setColour (juce::Colour (0xffe5a93b));
    g.setFont (juce::FontOptions (20.0f, juce::Font::bold));
    g.drawFittedText ("TOLIBJON PRODUCTION", getLocalBounds().removeFromTop (45), juce::Justification::centred, 1);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::FontOptions (12.0f));
    g.drawFittedText ("Vocal Strip VST3", getLocalBounds().removeFromBottom (25), juce::Justification::centred, 1);
}

void TolibjonProductionAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (20);
    area.removeFromTop (35);
    area.removeFromBottom (15);

    int sliderWidth = area.getWidth() / 3;

    auto compArea = area.removeFromLeft (sliderWidth);
    compLabel.setBounds (compArea.removeFromTop (20));
    compSlider.setBounds (compArea);

    auto warmArea = area.removeFromLeft (sliderWidth);
    warmLabel.setBounds (warmArea.removeFromTop (20));
    warmSlider.setBounds (warmArea);

    auto gainArea = area;
    gainLabel.setBounds (gainArea.removeFromTop (20));
    gainSlider.setBounds (gainArea);
}
