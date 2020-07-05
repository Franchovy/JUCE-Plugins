/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicPluginAudioProcessorEditor::BasicPluginAudioProcessorEditor (BasicPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);


    // Define volumeSlider Parameter
    volumeSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    volumeSlider.setRange (0.0, 127.0, 1.0);
    volumeSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    volumeSlider.setPopupDisplayEnabled (true, false, this);
    volumeSlider.setTextValueSuffix (" dB");
    volumeSlider.setValue(1.0);
    volumeSlider.setSize(40, 200);

    addAndMakeVisible (volumeSlider);

    juce::Font labelFont = volumeSliderLabel.getFont();
    labelFont.setBold(true);
    labelFont.setHeight(20.0f);

    volumeSliderLabel.setText("Gain", juce::sendNotificationSync);
    volumeSliderLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    volumeSliderLabel.setFont(labelFont);

    addAndMakeVisible(volumeSliderLabel);

    // add the listener to the slider
    volumeSlider.addListener (this);
}

BasicPluginAudioProcessorEditor::~BasicPluginAudioProcessorEditor()
{
}

//==============================================================================
void BasicPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll (juce::Colours::white);

    // set the current drawing colour to black
    g.setColour (juce::Colours::black);

}

void BasicPluginAudioProcessorEditor::resized()
{
    juce::FlexBox parameters;

    auto sliderItem = juce::FlexItem(volumeSlider);
    sliderItem.height = volumeSlider.getHeight();
    sliderItem.width = volumeSlider.getWidth();

    parameters.items.add(sliderItem);
    parameters.performLayout(juce::Rectangle<int>(40, 60, getWidth() - 40, getHeight() - 60));

    volumeSliderLabel.setBounds(volumeSlider.getX() - 5, volumeSlider.getY() - 35, 80, 40);

}

void BasicPluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    processor.gain = volumeSlider.getValue();
}
