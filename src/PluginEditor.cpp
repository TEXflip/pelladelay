#include "PluginProcessor.h"
#include "PluginEditor.h"

PelladelayAudioProcessorEditor::PelladelayAudioProcessorEditor (PealldelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);


    setResizable(true, true);
    setResizeLimits(knob_size, knob_size, 1080, 1080);

    // getLookAndFeel().setDefaultSansSerifTypefaceName("Arial"); // TODO: use custom font
    getLookAndFeel().setColour(ResizableWindow::backgroundColourId, Colour(0xFF0d0d0d));

    addAndMakeVisible(slider_dry_wet);
    addAndMakeVisible(slider_delay_time);
    addAndMakeVisible(slider_feedback);
    slider_dry_wet.setName("Dry/Wet");
    slider_delay_time.setName("time");
    slider_feedback.setName("feedback");

    slider_dry_wet.setRange(0, 1);
    slider_dry_wet.addListener(this);

    slider_delay_time.setRange(1, 3000);
    slider_delay_time.addListener(this);

    slider_feedback.setRange(0, 1.2);
    slider_feedback.addListener(this);

    
    container.flexWrap = FlexBox::Wrap::wrap;
    container.justifyContent = FlexBox::JustifyContent::center;
    container.alignItems = FlexBox::AlignItems::center;

    container.items.add(FlexItem(slider_dry_wet).withMinWidth((float)knob_size).withMinHeight((float)knob_size));
    container.items.add(FlexItem(slider_delay_time).withMinWidth((float)knob_size).withMinHeight((float)knob_size));
    container.items.add(FlexItem(slider_feedback).withMinWidth((float)knob_size).withMinHeight((float)knob_size));

    container.performLayout(getLocalBounds().toFloat());
}

PelladelayAudioProcessorEditor::~PelladelayAudioProcessorEditor()
{
}

void PelladelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void PelladelayAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.02;
    auto dialSize = getWidth() * 0.25;


    slider_dry_wet.setBounds(0, 0, knob_size, knob_size);
    slider_delay_time.setBounds(160, 0, knob_size, knob_size);
    slider_feedback.setBounds(320, 0, knob_size, knob_size);


    container.performLayout(getLocalBounds().toFloat());
}
