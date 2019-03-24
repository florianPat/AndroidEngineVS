#pragma once

#include "Vector2.h"
#include "Vector.h"
#include <unordered_map>
#include <memory>
#include "Rect.h"
#include "RenderWindow.h"

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
		Vector<String> collisionIdValue;
		Vector<String>* collisionIdPointer;

		bool collidersInPointer;
		union
		{
				Collider* collidersPointer;
				Collider collidersValue;
		} colliders;
	public:
		Collider* getCollider() const;
		Vector<String>* getCollisionIds() const;
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
			String triggerElementCollision = "";
			TriggerBodyPart triggerBodyPart = TriggerBodyPart::NONE;
		};

		bool isStatic;
		bool isTrigger;
		bool triggered = false;
		TriggerInformation triggerInformation = {};
		Vector2f pos;
		String id;
		Vector<PhysicElement> physicsElements;
	public:
		Vector2f vel = { 0.0f, 0.0f };
	public:
		//Should be called, if the object is moving
		Body(Vector2f& pos, String name, Collider* collider, Vector<String>* collisionId, bool isTrigger = false, bool isStatic = false);
		Body(Vector2f& pos, String name, Collider* collider, bool isTrigger = false, bool isStatic = false, Vector<String> collisionId = {});
		//Should be called if the object, is a static one
		Body(String name, Collider collider, bool isTrigger = false, bool isStatic = true, Vector<String> collisionId = {});
		//To have one name for a lot of Colliders. The body you have to pass by value, because pos and that does not make sense to manipulate here!
		Body(String name, Vector<Collider> colliders, bool isTrigger = false);
	public:
		bool getIsTriggerd();
		Vector2f& getPos();
		void setPos(Vector2f newPos);
		TriggerInformation& getTriggerInformation();
		String& getId();
	};
private:
	static constexpr float gravity = 9.81f;
	std::unordered_map<String, std::unique_ptr<Body>> bodies;
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
	bool removeElementById(String& id);
	static void applySpriteToBoundingBox(const Sprite& sprite, Collider& boundingBox);
	Vector<String> getAllCollisionIdsWhichContain(const String& string);
};