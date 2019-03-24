#pragma once

#include <unordered_map>
#include "Vector2.h"
#include "Rect.h"
#include "AssetManager.h"

//TODO: Add ellipse / (path) processing and rotation / scale transforms

//NOTE: Class to get out rects with specific ids in specifig groups from Inkscape-files
class InkscapeAnimationElement
{
	//TODO: Changed map to unordered_map. Does it matter?
	std::unordered_map<String, std::unordered_map<String, IntRect>> elementMap;
private:
	bool FindGroupLayer(String& lineContent) const;
public:
	InkscapeAnimationElement() = default;
	InkscapeAnimationElement(const String& inkscapeFileName, const Vector<String>& regionNames);
	InkscapeAnimationElement(const String& inkscapeFileName);
	IntRect getElementRect(String& keyFrameId, String& elementId) const;
	std::unordered_map<String, IntRect> getElementMap(const String& keyFrameId) const;
	bool isElementInMap(const String& keyFrameId) const;
};