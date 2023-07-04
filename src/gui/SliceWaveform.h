#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../core/SliceManager.h"
#include "../core/SlicePlayer.h"

class SliceWaveform : public juce::Component
{
public:
    enum class PlayState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

	static constexpr auto durationViewRange = 5;

	class Marker : public juce::Component
	{
	public:
		Marker(SlicePlayer* player);

		auto paint(juce::Graphics& g) -> void override;
		auto getPositionInPlayback() const -> const double;
		auto setPositionInPlayback(double positionInPlayback) -> void;

	private:
		SlicePlayer* m_slicePlayer = nullptr;
		double m_positionInPlayback = 0;
		friend class SliceWaveform;
	};

	SliceWaveform(juce::AudioFormatManager& formatManager, SlicePlayer* player, SliceManager* manager);

	auto paint(juce::Graphics& g) -> void override;

	auto setMarker(int index, double positionInPlayback) -> void;
	auto removeMarker(int index) -> void;
	auto updateMarkerPositions() -> void;

	auto thumbnail() -> juce::AudioThumbnail&;
	auto thumbnailChanged() -> void;

private:
	juce::AudioThumbnailCache m_thumbnailCache;
	juce::AudioThumbnail m_thumbnail;
	SlicePlayer* m_slicePlayer;
	SliceManager* m_sliceManager;
	std::array<std::unique_ptr<Marker>, SliceManager::MAX_NUM_SLICES> m_waveformMarkers;
	float m_zoomFactor = 1.0f;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliceWaveform)
};