#pragma once

#include <JuceHeader.h>
#define RAD 0.01745329251

using namespace juce;

class Knob : public Slider {
public:
	const float angle_s = 120 * RAD;
	float angle_r = 300 * RAD;
	int n_ticks = 8;
	bool stepped = false;

	Knob();
	// ~Knob() override;

	void paint(Graphics& g) override;

	double getValue();

	//void resized() override;

private:
	const float twoPi = MathConstants<float>::twoPi;
	const float halfPi = MathConstants<float>::halfPi;
	const float pi = MathConstants<float>::pi;

	Colour knob_bg_c = Colour(0xFF3f3d43);
	Rectangle<float> tip_bb = { 0, 0, 30, 6 };

	void drawTip(Graphics& g,
		Rectangle<float>& bb,
		Rectangle<float>& tip_bb, 
		float margin);

	void drawTicks(Graphics& g, float r);

	float getKnobCurrAngle();

	void mouseMove(const MouseEvent& event) override;

	void mouseExit(const MouseEvent& event) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Knob)
};