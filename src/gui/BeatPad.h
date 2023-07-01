#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../core/SampleSlice.h"

class BeatPad : public juce::Component
{
public:
    class Pad : public juce::Component
    {
    public:
        enum class State
        {
            Empty,
            Filled
        };

        Pad();

        auto operator==(const Pad& other) -> bool;
        auto operator!=(const Pad& other) -> bool;

        auto getButton() -> juce::ShapeButton&;
        auto getState() -> State;

        auto setState(State state) -> void;
    private:
        juce::ShapeButton m_button;
        State m_state = State::Empty;
        friend class BeatPad;
    };

    BeatPad();
    auto resized() -> void override;

    auto getPad(int index) -> Pad*;
    auto getIndexOfPad(BeatPad::Pad* pad) -> std::optional<int>;
    auto getSelectedPad() -> Pad*;
    auto setSelectedPad(int index) -> void;
    auto setSelectedPad(Pad* pad) -> void;

private:
    std::array<Pad, SampleSlice::MAX_NUM_SLICES> m_pads;
    Pad* m_selectedPad = nullptr;
    friend class Pad;
};