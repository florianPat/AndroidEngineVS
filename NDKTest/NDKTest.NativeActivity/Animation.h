#pragma once

#include "Vector.h"
#include "TextureAtlas.h"
#include "stdint.h"
#include "Clock.h"

class Animation
{
public:
	enum class PlayMode
	{
		LOOPED,
		LOOP_REVERSED,
		NORMAL,
		REVERSED
	};
protected:
	Vector<Sprite> keyFrames;
	size_t keyFrameIt;
	size_t keyFrameItReverse;
	int64_t currentTime = 0;
	int64_t frameDuration;
	PlayMode playMode;
	int randomElement = 0;
	Clock clock;

	bool paused = false;
public:
	Animation(Vector<TextureRegion>& keyFrames, int64_t frameDuration, PlayMode type);
	Animation(const Vector<ShortString>& regionNames, const TextureAtlas& atlas, int64_t frameDuration = Time::seconds(0.2f).asMicroseconds(), PlayMode type = PlayMode::LOOPED);
	PlayMode getPlayMode();
	int64_t getFrameDuration();
	void setFrameDuration(int64_t frameDuration);
	void setKeyFrames(Vector<TextureRegion>& keyFrames);
	bool isAnimationFinished();
	const Sprite& getKeyFrame();
	void setPlayMode(PlayMode& playMode);
	void restartFrameTimer();
	void pause();
	void resume();
	void restart();
};