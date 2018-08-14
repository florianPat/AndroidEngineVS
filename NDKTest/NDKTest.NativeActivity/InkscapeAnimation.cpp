#include "InkscapeAnimation.h"
#include "Utils.h"

void InkscapeAnimation::setupInkscapeKeyFrames(const std::vector<std::string>& regionNames)
{
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		if (iae.isElementInMap(*it))
			inkscapeKeyFrames.push_back(std::map<std::string, IntRect>{ iae.getElementMap(*it) });
	}
}

InkscapeAnimation::InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas & atlas, const std::string& inkscapeFileName, TextureAssetManager* assetManager, int64_t frameDuration, PlayMode type)
	: Animation(regionNames, atlas, frameDuration, type), iae(inkscapeFileName, regionNames, assetManager), inkscapeKeyFrames()
{
	setupInkscapeKeyFrames(regionNames);
}

InkscapeAnimation::InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas & atlas, const InkscapeAnimationElement & iae, int64_t frameDuration, PlayMode type)
	: Animation(regionNames, atlas, frameDuration, type), iae(iae), inkscapeKeyFrames()
{
	setupInkscapeKeyFrames(regionNames);
}

void InkscapeAnimation::setInkscapeAnimationElement(const std::string& inkscapeFileName, const std::vector<std::string>& regionNames, TextureAssetManager* assetManager)
{
	inkscapeKeyFrames.clear();

	iae = InkscapeAnimationElement(inkscapeFileName, regionNames, assetManager);

	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		inkscapeKeyFrames.push_back(std::map<std::string, IntRect>{ iae.getElementMap(*it) });
	}
}

IntRect InkscapeAnimation::getInkscapeAnimationElementKeyFrame(const std::string & keyFrameId) const
{
	size_t i = (playMode == PlayMode::LOOPED || playMode == PlayMode::NORMAL) ? keyFrameIt : keyFrameItReverse;

	auto result = inkscapeKeyFrames[i].find(keyFrameId);
	if (result != inkscapeKeyFrames[i].end())
		return result->second;
	else
	{
		InvalidCodePath;
		return IntRect();
	}
}
