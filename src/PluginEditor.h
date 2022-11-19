#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "components/Knob.h"

using namespace juce;

class PelladelayAudioProcessorEditor  : public AudioProcessorEditor,
                                                private Slider::Listener
{
public:
    const int knob_size = 150;

    PelladelayAudioProcessorEditor (PealldelayAudioProcessor&);
    ~PelladelayAudioProcessorEditor() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    PealldelayAudioProcessor& audioProcessor;

    void sliderValueChanged(Slider* slider) override {
        if (slider == &slider_dry_wet)
            audioProcessor.dry_wet_ratio = 1 - slider->getValue();
        else if (slider == &slider_delay_time) {
            
            int delay_time = slider->getValue(); // this is in ms
            unsigned int delay_length = (unsigned int)(delay_time / 1000. * audioProcessor.getSampleRate());
            unsigned int old_del_len = audioProcessor.delay_length;
            
            // if the new delay lenght is bigger than before, clean the window 
            // (cause it's dirty)
            if (old_del_len < delay_length)
                for (size_t i = 0; i < 2; i++)
                    for (size_t j = old_del_len; j < delay_length; j++)
                        audioProcessor.window[i][j] = 0;
            
            audioProcessor.delay_length = delay_length;
        }
        else if (slider == &slider_feedback)
            audioProcessor.feedback = slider->getValue();
    }

    Knob slider_dry_wet;
    Knob slider_delay_time;
    Knob slider_feedback;
    FlexBox container;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PelladelayAudioProcessorEditor)
};
