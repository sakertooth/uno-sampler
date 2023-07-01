#include "BeatPad.h"

#include "../UnoEditor.h"

BeatPad::BeatPad()
{
	for (int i = 0; i < static_cast<int>(m_beatPad.size()); ++i)
	{
		auto& pad = m_beatPad[static_cast<size_t>(i)];
		addAndMakeVisible(pad);
		pad.setComponentID("BeatPad::Pad" + std::to_string(i));
		pad.getButton().onStateChange = [this, &pad] {
			if (pad.getButton().isDown())
			{
				auto main = dynamic_cast<UnoEditor*>(getParentComponent());
				main->selectPad(&pad);
			}
		};
	}
}

void BeatPad::resized()
{
	auto area = getLocalBounds();
	const auto areaHeight = area.getHeight();

	auto rowOneRegion = area.removeFromTop(areaHeight / 2);
	auto rowTwoRegion = area.removeFromTop(areaHeight / 2);
	auto spacing = rowOneRegion.getWidth() / static_cast<int>(m_beatPad.size() / 2);
	auto margin = 5;

	for (size_t row = 0; row < 2; ++row)
	{
		auto& rowRegion = row == 0 ? rowOneRegion : rowTwoRegion;
		for (size_t buttonIndex = 0; buttonIndex < m_beatPad.size() / 2; ++buttonIndex)
		{
			const size_t index = row * m_beatPad.size() / 2 + buttonIndex;
			m_beatPad[index].setBounds(rowRegion.removeFromLeft(spacing).reduced(margin));
		}
	}
}

auto BeatPad::findSlice(BeatPad::Pad* pad) -> std::optional<int>
{
	if (pad == nullptr) { return std::nullopt; }
	auto it = std::find(m_beatPad.begin(), m_beatPad.end(), *pad);
	if (it == m_beatPad.end()) { return std::nullopt; }
	return std::distance(m_beatPad.begin(), it);
}

BeatPad::Pad::Pad()
	: m_button("", juce::Colours::white, juce::Colours::white.darker(), juce::Colours::white.darker(0.6f))
{
	addAndMakeVisible(m_button);
	setSize(32, 32);

	auto path = juce::Path{};
	path.addRoundedRectangle(getLocalBounds(), 2.0f);
	m_button.setShape(path, true, true, true);
}

auto BeatPad::Pad::getButton() -> juce::ShapeButton&
{
	return m_button;
}

auto BeatPad::Pad::operator==(const Pad& other) -> bool
{
	return getComponentID() == other.getComponentID();
}

auto BeatPad::Pad::operator!=(const Pad& other) -> bool
{
	return !(*this == other);
}

auto BeatPad::Pad::getState() -> State
{
	return m_state;
}

auto BeatPad::Pad::setState(State state) -> void
{
	switch (state)
	{
	case State::Empty:
	{
		if (m_state == State::Empty) return;
		const auto color = juce::Colours::white;
		m_button.setColours(color, color.darker(), color.darker(0.6f));
		break;
	}
	case State::Filled:
	{
		if (m_state == State::Filled) return;
		const auto color = juce::Colours::cyan;
		m_button.setColours(color, color.darker(), color.darker(0.6f));
		break;
	}
	default:
		break;
	}

	m_state = state;
}