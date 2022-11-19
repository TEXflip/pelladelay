#include "Knob.h"

#define KNOB_BB_DIST 15
#define INN_R 6
#define TICK_SIZE 2

Knob::Knob() : Slider(RotaryVerticalDrag, NoTextBox) {
	
}

// Knob::~Knob() {

// }

double Knob::getValue(){
	double v = Slider::getValue();
	if (stepped){
		Range<double> r = getRange();
		double s = n_ticks/r.getLength();
		return floor(s*v)/s;
	}
	else
		return v;
}

float Knob::getKnobCurrAngle() {
	double value = Slider::valueToProportionOfLength(getValue());

	if (stepped) {
		float step = angle_r / n_ticks;
		return angle_s + floor(angle_r * value / step) * step;
	}

	return angle_r * value + angle_s;
}

void Knob::drawTip(Graphics& g, 
	Rectangle<float>& bb,
	Rectangle<float>& tip_bb, 
	float margin)
{
	float bb_r = (bb.getWidth()) / 2.f;
	
	float th = getKnobCurrAngle();

	float bb_cx = bb.getCentreX();
	float bb_cy = bb.getCentreY();
	float c = cos(th);
	float s = sin(th);

	AffineTransform t = {
		c,
		-s,
		c * (bb_r - tip_bb.getWidth() - margin) + s * tip_bb.getHeight() * 0.5f + bb_cx,
		s,
		c,
		s * (bb_r - tip_bb.getWidth() - margin) - c * tip_bb.getHeight() * 0.5f + bb_cy,
	};

	Path p;
	//p.addRectangle(tip_bb);
	p.addRoundedRectangle(tip_bb, 4, 1);
	g.fillPath(p, t);
}

void Knob::drawTicks(Graphics& g, float r)
{
	if (n_ticks < 1) return;

	auto bb = getLocalBounds().toFloat().getCentre();

	float x;
	float y;
	for (float a = angle_s; a <= angle_s + angle_r + RAD; a += angle_r / n_ticks)
	{
		x = cos(a) * r + bb.x;
		y = sin(a) * r + bb.y;
		g.fillEllipse(x - TICK_SIZE, y - TICK_SIZE, TICK_SIZE * 2, TICK_SIZE * 2);
	}
}

void Knob::paint(Graphics& g)
{
	auto bb = getLocalBounds().toFloat();
	bb.expand(-KNOB_BB_DIST, -KNOB_BB_DIST);
	float tick_r = bb.getWidth() * 0.5f + TICK_SIZE * 3;
	g.setColour(knob_bg_c);
	g.fillEllipse(bb);

	bb.expand(-INN_R, -INN_R);
	auto centre = bb.getCentre();
	float th = getKnobCurrAngle();
	AffineTransform t = AffineTransform().translation(-centre.x, -centre.y).rotated(th).translated(centre.x, centre.y);
	Path p;
	float star_r = bb.getWidth() * 0.5f;
	p.addStar(centre, 30, star_r - INN_R * 0.5f, star_r);
	p = p.createPathWithRoundedCorners(4);
	g.setColour(Colour(0xFF0d0d0d));
	g.fillPath(p, t);
	// g.setColour(Colour(0xD0000000));
	// g.strokePath(p, t);
	//g.fillEllipse(bb);

	g.setColour(Colour(0xFFFFFFFF));
	drawTip(g, bb, tip_bb, 0);
	drawTicks(g, tick_r);

	g.setColour(Colour(0xFF3f3d43));
	double value = getValue();
	g.drawFittedText(String(value, 2), getLocalBounds(), Justification::centred, 1);

	g.setColour(Colour(0xFFFFFFFF));
	g.drawFittedText(getName(), getLocalBounds(), Justification::centredBottom, 1);
	//g.drawEllipse(bb, 2.f);
}

void Knob::mouseMove(const MouseEvent& event) {
	auto bb = getLocalBounds().toFloat();
	float dist = event.position.getDistanceFrom(bb.getCentre());
	if (dist < bb.getWidth() * 0.5 - KNOB_BB_DIST)
		knob_bg_c = Colour(0xFF0074cc);
	else
		knob_bg_c = Colour(0xFF3f3d43);
	repaint();
}

void Knob::mouseExit(const MouseEvent& event) {
	knob_bg_c = Colour(0xFF3f3d43);
	repaint();
}
//
//void Knob::resized()
//{
//}
