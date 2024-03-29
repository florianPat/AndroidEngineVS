#pragma once

#include <unordered_map>
#include <map>
#include "Component.h"
#include <memory>
#include "gomSort.h"

class Actor
{
	friend class GameObjectManager;

	const unsigned int id;
	std::unordered_map<int, std::unique_ptr<Component>> components;
private:
	void update(float dt);
	void draw(unsigned int componentId);
	void sort(std::multimap<gomSort::SortKey, unsigned long long, gomSort::SortCompare>& sortedActors);
	unsigned long long GetActorComponentId(unsigned int componentId);
public:
	Actor(unsigned int id);
	void addComponent(std::unique_ptr<Component> component);
	void removeComponent(unsigned int componentId);
	template <typename T> T* getComponent(unsigned int componentId);
	int getId() const { return id; };
};

template<typename T>
inline T* Actor::getComponent(unsigned int componentId)
{
	auto result = components.find(componentId);
	Component* componentPtr = result->second.get();
	if (result != components.end())
	{
		//NOTE: Only used to really verify that it is ok what I am doing. RTTI should be switched off in release mode
#ifndef NDEBUG
		assert(typeid((*componentPtr)) == typeid(T));
		assert(dynamic_cast<T*>(componentPtr) != nullptr);
#endif
		return (T*) componentPtr;
	}
	else
		return nullptr;
}
