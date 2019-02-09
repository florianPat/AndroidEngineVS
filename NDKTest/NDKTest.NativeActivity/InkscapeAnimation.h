#pragma once

#include "InscapeAnimationElement.h"
#include "Animation.h"

//NOTE: Get rect from id to keyFrame
class InkscapeAnimation : public Animation
{
	InkscapeAnimationElement iae;
	std::vector<std::map<std::string, IntRect>> inkscapeKeyFrames;
private:
	void setupInkscapeKeyFrames(const std::vector<std::string>& regionNames);
public:
	InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas& atlas, const std::string& inkscapeFileName, int64_t frameDuration = Time::seconds(0.2f).asMicroseconds(), PlayMode type = PlayMode::LOOPED);
	InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas& atlas, const InkscapeAnimationElement& iae, int64_t frameDuration = Time::seconds(0.2f).asMicroseconds(), PlayMode type = PlayMode::LOOPED);
	//NOTE: Should only be used if Animation::SetKeyFrames() was used before
	void setInkscapeAnimationElement(const std::string& inkscapeFileName, const std::vector<std::string>& regionNames);
	IntRect getInkscapeAnimationElementKeyFrame(const std::string& keyFrameId) const;
};