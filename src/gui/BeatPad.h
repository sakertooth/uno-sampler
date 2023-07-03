#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../core/SliceManager.h"

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
        auto getSliceToPlay() -> const std::optional<int>&;

        auto setState(State state) -> void;
        auto setSliceToPlay(int sliceIndex) -> void;
    private:
        juce::ShapeButton m_button;
        State m_state = State::Empty;
        bool m_pressed;
        std::optional<int> m_sliceToPlay;
        friend class BeatPad;
    };

    BeatPad();
    auto resized() -> void override;

    auto getPad(int index) -> Pad*;
    auto getIndexOfPad(Pad* pad) -> std::optional<int>;
    auto getSelectedPad() -> Pad*;
    auto getSliceForPad(Pad* pad) -> int;

    auto setSelectedPad(int index) -> void;
    auto setSelectedPad(Pad* pad) -> void;

private:
    std::array<Pad, SliceManager::MAX_NUM_SLICES> m_pads;
    Pad* m_selectedPad = nullptr;
    friend class Pad;
};