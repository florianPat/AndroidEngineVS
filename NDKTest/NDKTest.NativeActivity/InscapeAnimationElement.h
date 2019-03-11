#pragma once

#include <map>
#include "Vector2.h"
#include "Rect.h"
#include "AssetManager.h"

//TODO: Add ellipse / (path) processing and rotation / scale transforms

//NOTE: Class to get out rects with specific ids in specifig groups from Inkscape-files
class InkscapeAnimationElement
{
	std::map<std::string, std::map<std::string, IntRect>> elementMap;
private:
	bool FindGroupLayer(std::string& lineContent) const;
public:
	InkscapeAnimationElement() = default;
	InkscapeAnimationElement(const std::string& inkscapeFileName, const Vector<std::string>& regionNames);
	InkscapeAnimationElement(const std::string& inkscapeFileName);
	IntRect getElementRect(std::string& keyFrameId, std::string& elementId) const;
	std::map<std::string, IntRect> getElementMap(const std::string& keyFrameId) const;
	bool isElementInMap(const std::string& keyFrameId) const;
};