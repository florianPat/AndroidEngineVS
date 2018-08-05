#pragma once

#include "Vector2.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include "Rect.h"
#include "RenderWindow.h"

//TODO: Add GJK just for implementatin ;)

class Physics
{
public:
	class FloatCircle
	{
	private:
		friend class Physics;
	public:
		float radius;
		Vector2f center;
	public:
		FloatCircle(const Vector2f& center, float radius);
		FloatCircle(float centerX, float centerY, float radius);
		FloatCircle() = default;
	};

	class OBB
	{
	private:
		friend class Physics;

		static constexpr float PI = 3.1415927f;

		float angle;
		Vector2f xAxis, yAxis;
	public:
		float width, height;
		Vector2f pos;
		Vector2f origin;
	public:
		//angle has to be in degrees!
		OBB(float left, float top, float width, float height, float angle);
		OBB(Vector2f& topLeft, float width, float height, float angle);
		//Local origin
		OBB(float left, float top, float width, float height, float angle, Vector2f origin);
		OBB(Vector2f& topLeft, float width, float height, float angle, Vector2f origin);
		//angle has to be in degrees!
		void setAngle(float newAngle);
		float getAngle() const;
		OBB() = default;
	};

	class Collider
	{
	public:
		enum class Type
		{
			rect,
			obb,
			circle
		};
	private:
		friend class Physics;

		Type type;
	public:
		union
		{
			FloatRect rect;
			OBB obb;
			FloatCircle circle;
		} collider;

		Collider(FloatRect& rect);
		Collider(OBB& obb);
		Collider(FloatCircle& circle);

		Collider(FloatRect&& rect);
		Collider(OBB&& obb);
		Collider(FloatCircle&& circle);

		Type GetType() const;
	private:
		Collider();

		bool intersects(const Collider& other) const;
		bool collide(const Collider& other, Vector2f* minTransVec) const;

		void getPointsAxis(Vector2f* points, Vector2f* axis) const;
		Vector2f getProjectionMinMax(const Vector2f* points, const Vector2f& axis, bool isXAxis) const;
	};
private:
	//NOTE: All is public, but really you should only use the two methods, could make a constructor for that and therefore make it a class but yeah ;)
	struct PhysicElement
	{
		bool collisionIdInPointer;
		std::vector<std::string> collisionIdValue;
		std::vector<std::string>* collisionIdPointer;

		bool collidersInPointer;
		union
		{
				Collider* collidersPointer;
				Collider collidersValue;
		} colliders;
	public:
		Collider* getCollider() const;
		std::vector<std::string>* getCollisionIds() const;
	};
public:
	class Body
	{
		friend class Physics;

		enum class TriggerBodyPart
		{
			NONE,
			HEAD,
			SHOES,
			LEFT,
			RIGHT
		};

		struct TriggerInformation
		{
			std::string triggerElementCollision = "";
			TriggerBodyPart triggerBodyPart = TriggerBodyPart::NONE;
		};

		bool isStatic;
		bool isTrigger;
		bool triggered = false;
		TriggerInformation triggerInformation = {};
		Vector2f pos;
		std::string id;
		std::vector<PhysicElement> physicsElements;
	public:
		Vector2f vel = { 0.0f, 0.0f };
	public:
		//Should be called, if the object is moving
		Body(Vector2f& pos, std::string name, Collider* collider, std::vector<std::string>* collisionId, bool isTrigger = false, bool isStatic = false);
		Body(Vector2f& pos, std::string name, Collider* collider, bool isTrigger = false, bool isStatic = false, std::vector<std::string> collisionId = {});
		//Should be called if the object, is a static one
		Body(std::string name, Collider collider, bool isTrigger = false, bool isStatic = true, std::vector<std::string> collisionId = {});
		//To have one name for a lot of Colliders. The body you have to pass by value, because pos and that does not make sense to manipulate here!
		Body(std::string name, std::vector<Collider> colliders, bool isTrigger = false);
	public:
		bool getIsTriggerd();
		Vector2f& getPos();
		void setPos(Vector2f newPos);
		TriggerInformation& getTriggerInformation();
		std::string& getId();
	};
private:
	static constexpr float gravity = 9.81f;
	std::unordered_map<std::string, std::unique_ptr<Body>> bodies;
private:
	void handleCollision(Body* itBody, Body* collideElementBody, Collider & bodyCollider,
						 const Collider& elementCollider);
public:
	Physics();
	void update(float dt);
	void debugRenderBodies(RenderWindow& window);
	//Use if you need a reference to the body, to get back triggerInformation etc.
	Body* addElementPointer(std::unique_ptr<Body> body);
	//Use this otherwise
	void addElementValue(Body body);
	bool removeElementById(std::string& id);
	static void applySpriteToBoundingBox(const sf::Sprite& sprite, Collider& boundingBox);
	std::vector<std::string> getAllCollisionIdsWhichContain(const std::string& string);
};