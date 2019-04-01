#include "Physics.h"
#include <math.h>
#include "Utils.h"
#include "RectangleShape.h"
#include "CircleShape.h"

void Physics::handleCollision(Body& itBody, Body& collideElementBody, Collider & bodyCollider,
	const Collider& elementCollider)
{
	if (itBody.isTrigger || collideElementBody.isTrigger)
	{
		if (bodyCollider.intersects(elementCollider))
		{
			if (itBody.isTrigger)
			{
				itBody.triggered = true;
				itBody.triggerInformation.triggerElementCollision = collideElementBody.id;
				return;
			}
			else
			{
				collideElementBody.triggered = true;
				collideElementBody.triggerInformation.triggerElementCollision = itBody.id;
				return;
			}
		}
	}

	Vector2f minTransVec = {};
	if (bodyCollider.collide(elementCollider, &minTransVec))
	{
		if (minTransVec.x > 0.0f)
		{
			itBody.vel.x = 0;
			itBody.triggerInformation.triggerBodyPart = Body::TriggerBodyPart::LEFT;
		}
		else if (minTransVec.x < 0.0f)
		{
			itBody.vel.x = 0;
			itBody.triggerInformation.triggerBodyPart = Body::TriggerBodyPart::RIGHT;
		}
		if (minTransVec.y > 0.0f)
		{
			itBody.vel.y = 0;
			itBody.triggerInformation.triggerBodyPart = Body::TriggerBodyPart::HEAD;
		}
		else if (minTransVec.y < 0.0f)
		{
			itBody.vel.y = 0;
			itBody.triggerInformation.triggerBodyPart = Body::TriggerBodyPart::SHOES;
		}

		itBody.pos += minTransVec;
	}
}

void Physics::Collider::getPointsAxis(Vector2f * points, Vector2f * axis) const
{
	switch (type)
	{
		case Collider::Type::obb:
		{
			OBB bodyOBB = collider.obb;

			points[0] = { bodyOBB.pos };
			points[1] = { bodyOBB.pos.x + bodyOBB.width, bodyOBB.pos.y };
			points[2] = { bodyOBB.pos.x + bodyOBB.width, bodyOBB.pos.y + bodyOBB.height };
			points[3] = { bodyOBB.pos.x, bodyOBB.pos.y + bodyOBB.height };

			//Global origin
			Vector2f origin = bodyOBB.pos + bodyOBB.origin;

			for (int i = 0; i < 4; ++i)
			{
				points[i] = Vector2f(bodyOBB.pos + bodyOBB.xAxis * (points[i].x - origin.x) + bodyOBB.yAxis * (points[i].y - origin.y));
			}

			axis[0] = bodyOBB.xAxis;
			axis[1] = bodyOBB.yAxis;

			break;
		}
		case Collider::Type::rect:
		{
			FloatRect bodyRect = collider.rect;

			points[0] = { bodyRect.left, bodyRect.top };
			points[1] = { bodyRect.left + bodyRect.width, bodyRect.top };
			points[2] = { bodyRect.left + bodyRect.width, bodyRect.top + bodyRect.height };
			points[3] = { bodyRect.left, bodyRect.top + bodyRect.height };

			axis[0] = { 1.0f, 0.0f };
			axis[1] = { 0.0f, 1.0f };
			break;
		}
		case Collider::Type::circle:
		{
			FloatCircle bodyCircle = collider.circle;

			points[0] = bodyCircle.center;

			axis[0] = { 0.0f, 0.0f };
			axis[1] = { 0.0f, 0.0f };

			break;
		}
		default:
		{
			InvalidCodePath;
		}
	}
}

Vector2f Physics::Collider::getProjectionMinMax(const Vector2f * points, const Vector2f & axis, bool isXAxis) const
{
	Vector2f result = { points[0].x * axis.x + points[0].y * axis.y, points[0].x * axis.x + points[0].y * axis.y };

	if (type != Type::circle)
	{
		for (int i = 1; i < 4; ++i)
		{
			float proj = points[i].x * axis.x + points[i].y * axis.y;

			if (proj < result.x)
				result.x = proj;
			else if (proj > result.y)
				result.y = proj;
		}
	}
	else
	{
		//TODO: Also, the circle goes a bit into the obb... (It happens because I just throw out the four corner points, but thats not really good for 
		//for example top left...
		//TODO: Use vohoo regions (or how its called) to make "corner collision" nicer
		float proj = (points[0].x + (isXAxis ? collider.circle.radius : 0)) * axis.x + (points[0].y + (isXAxis ? 0 : collider.circle.radius)) * axis.y;
		
		if (proj < result.x)
			result.x = proj;
		else if (proj > result.y)
			result.y = proj;


		proj = (points[0].x - (isXAxis ? collider.circle.radius : 0)) * axis.x + (points[0].y - (isXAxis ? 0 : collider.circle.radius)) * axis.y;

		if (proj < result.x)
			result.x = proj;
		else if (proj > result.y)
			result.y = proj;
	}

	return result;
}

Physics::Physics() : bodies()
{
}

#define DEFINE_BODIES_COLLIDERS Body& collideElementBody = *collisionIdIt; \
								Body& itBody = *it; \
								Collider& bodyRect = *itBody.physicsElements[0].getCollider(); \
								Collider& elementRect = *collideElementBody.physicsElements[0].getCollider();

void Physics::update(float dt)
{
	//TODO: Add broad phase collision detection!
	//NOTE: For now (the fighting game), it is not necessary, because I do not have lots of Colliders (7 or so)
	size_t i = 0;
	for (auto it = bodies.begin(); i < activeIndex; ++it, ++i)
	{
		assert(it != bodies.end());

		it->triggered = false;
		it->triggerInformation.triggerElementCollision.clear();
		it->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::NONE;

		size_t oI = 0;
		auto collisionIdIt = bodies.begin();
		for (; oI < it->index; ++collisionIdIt, ++oI)
		{
			assert(collisionIdIt != bodies.end());

			DEFINE_BODIES_COLLIDERS;
			
			handleCollision(itBody, collideElementBody, bodyRect, elementRect);
		}

		for (; oI < activeIndex; ++collisionIdIt, ++oI)
		{
			assert(collisionIdIt != bodies.end());

			DEFINE_BODIES_COLLIDERS;

			handleCollision(itBody, collideElementBody, bodyRect, elementRect);
		}

		for (; oI < inactiveIndex; ++collisionIdIt, ++oI)
		{
			assert(collisionIdIt != bodies.end());

			DEFINE_BODIES_COLLIDERS;

			for (auto collideElementPhysicsElementIt = collideElementBody.physicsElements.begin(); collideElementPhysicsElementIt != collideElementBody.physicsElements.end(); ++collideElementPhysicsElementIt)
			{
				elementRect = *collideElementPhysicsElementIt->getCollider();

				handleCollision(itBody, collideElementBody, bodyRect, elementRect);
			}
		}

		it->pos += it->vel * dt;
	}
}

void Physics::debugRenderBodies(RenderWindow & window) const
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (!it->isStatic)
		{
			Collider* collider = it->physicsElements[0].getCollider();

			RectangleShape body;

			switch (collider->type)
			{
				case Collider::Type::rect:
				{
					FloatRect colliderRect = collider->collider.rect;

					body.setSize(Vector2f(colliderRect.width, colliderRect.height));
					body.setPosition(Vector2f(colliderRect.left, colliderRect.top));
					body.setFillColor(Colors::Yellow);

					window.draw(body);

					break;
				}
				case Collider::Type::obb:
				{
					OBB collideOBB = collider->collider.obb;

					body.setPosition(collideOBB.pos);
					body.setSize(Vector2f{ collideOBB.width, collideOBB.height });
					body.setOrigin(collideOBB.origin);
					body.setRotation(utils::radiansToDegrees(collideOBB.angle));
					body.setFillColor(Colors::Yellow);
#if 0
					Vector2f points[4] = { { collideOBB.pos },{ collideOBB.pos.x + collideOBB.width, collideOBB.pos.y },
											 { collideOBB.pos.x + collideOBB.width, collideOBB.pos.y + collideOBB.height },
											 { collideOBB.pos.x, collideOBB.pos.y + collideOBB.height } };

					//Global origin
					Vector2f origin = collideOBB.pos + collideOBB.origin;

					for (int i = 0; i < 4; ++i)
					{
						points[i] = Vector2f(collideOBB.pos + (points[i].x - origin.x) * collideOBB.xAxis + (points[i].y - origin.y) * collideOBB.yAxis);
					}

					for (unsigned int i = 0; i < body.getPointCount(); ++i)
					{
						Vector2f myPoint = points[i];
						Vector2f point = body.getPoint(i);
						point = body.getTransform().transformPoint(point);
						sf::Transform transform = body.getTransform();

						std::cout << myPoint.x << " " << myPoint.y << "---" << point.x << " " << point.y << std::endl;
					}
#endif

					window.draw(body);

					break;
				}
				case Collider::Type::circle:
				{
					CircleShape body;

					FloatCircle circle = collider->collider.circle;

					body.setPosition(circle.center.x - circle.radius, circle.center.y - circle.radius);
					body.setRadius(circle.radius);
					body.setFillColor(Colors::Yellow);

					window.draw(body);

					break;
				}
			}
		}
	}
}

Physics::Body* Physics::addElementPointer(Body&& body)
{
	assert(body.physicsElements.size() == 1);

	Body* result = nullptr;

	if (body.isStatic || body.isTrigger)
	{
		bodies.push_back(std::move(body));
		result = &bodies.back();
		result->index = bodies.size() - 1;
	}
	else
	{
		bodies.insertPush_back(activeIndex, std::move(body));
		result = &bodies[activeIndex];
		result->index = activeIndex++;
	}

	return result;
}

void Physics::addElementValue(Body&& body)
{
	if (body.isStatic || body.isTrigger)
	{
		bodies.push_back(std::move(body));
	}
	else
	{
		bodies.insertPush_back(activeIndex++, std::move(body));
	}
}

void Physics::removeElementByIndex(int index)
{
	assert(index != -1);
	if ((!bodies[index].isStatic) && (!bodies[index].isTrigger))
		--activeIndex;
	bodies.erasePop_back(index);
}

void Physics::flipActive(Body & body)
{
	size_t sizeM1 = bodies.size() - 1;

	if (body.isActive)
	{
		if (body.isStatic || body.isTrigger)
		{
			bodies.swap(body.index, --inactiveIndex);
			bodies.swap(inactiveIndex, sizeM1);
		}
		else
		{
			bodies.swap(body.index, --activeIndex);
			bodies.swap(activeIndex, --inactiveIndex);
			bodies.swap(inactiveIndex, sizeM1);
		}

		body.index = sizeM1;
		body.isActive = false;
	}
	else
	{
		if (body.isStatic || body.isTrigger)
		{
			bodies.swap(body.index, inactiveIndex);
			body.index = inactiveIndex++;
		}
		else
		{
			bodies.swap(body.index, inactiveIndex);
			bodies.swap(inactiveIndex++, activeIndex);
			body.index = activeIndex++;
		}

		body.isActive = true;
	}
}

#if 0
void Physics::applySpriteToBoundingBox(const Sprite & sprite, Collider & boundingBox)
{
	assert(boundingBox.type == Collider::Type::rect);

	boundingBox.collider.rect.left = sprite.getGlobalBounds().left;
	boundingBox.collider.rect.top = sprite.getGlobalBounds().top;
	boundingBox.collider.rect.width = (float)sprite.getGlobalBounds().width;
	boundingBox.collider.rect.height = (float)sprite.getGlobalBounds().height;
}

Vector<ShortString> Physics::getAllCollisionIdsWhichContain(const ShortString & string)
{
	Vector<ShortString> result;

	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		size_t match = it->first.find(string, 0);
		if (match != String::npos)
		{
			bool onlyNumbers = true;
			ShortString substr = it->first;
			substr.erase(match, string.length());
			for (auto it = substr.begin(); it != substr.end(); ++it)
			{
				char c = *it;
				//NOTE: has to be ascii!
				if (c >= 48 && c <= 57)
					continue;
				else
				{
					onlyNumbers = false;
					break;
				}
			}
			if (onlyNumbers)
				result.push_back(it->first);
		}
	}

	return result;
}
#endif

Physics::Body::Body(Vector2f&& pos, const ShortString& name, Collider* collider, bool isTrigger, bool isStatic)
	: isStatic(isStatic), isTrigger(isTrigger), pos(std::move(pos)), id(name), physicsElements{}, index(-1)
{
	PhysicElement physicsElement = {};
	physicsElement.collidersInPointer = true;
	physicsElement.colliders.collidersPointer = collider;

	this->physicsElements.push_back(physicsElement);
}

Physics::Body::Body(const ShortString& name, Collider&& collider, bool isTrigger, bool isStatic)
	: isStatic(isStatic), isTrigger(isTrigger), pos(0.0f, 0.0f), id(name), physicsElements{}, index(-1)
{
	PhysicElement physicsElement = {};
	physicsElement.collidersInPointer = false;
	physicsElement.colliders.collidersValue = std::move(collider);

	this->physicsElements.push_back(physicsElement);
}

Physics::Body::Body(const ShortString& name, Vector<Collider>&& colliders, bool isTrigger) : isStatic(true), isTrigger(isTrigger), pos(0.0f, 0.0f),
																						 id(name), physicsElements{}, index(-1)
{
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		PhysicElement physicsElement = {};
		physicsElement.collidersInPointer = false;
		physicsElement.colliders.collidersValue = std::move(*it);

		this->physicsElements.push_back(physicsElement);
	}
}

bool Physics::Body::getIsTriggerd() const
{
	return triggered;
}

const Vector2f& Physics::Body::getPos() const
{
	assert(!isStatic);

	if (!isStatic)
		return pos;
	else
	{
		InvalidCodePath;
		return pos;
	}
}

void Physics::Body::setPos(Vector2f newPos)
{
	assert(!isStatic);

	if (!isStatic)
		pos = newPos;
	else
	{
		InvalidCodePath;
	}
}

const Physics::Body::TriggerInformation & Physics::Body::getTriggerInformation() const
{
	return triggerInformation;
}

const ShortString & Physics::Body::getId() const
{
	return id;
}

int Physics::Body::getIndex() const
{
	return index;
}

bool Physics::Body::getIsActive() const
{
	return isActive;
}

Physics::Collider * Physics::PhysicElement::getCollider() const
{
	if (collidersInPointer)
		return colliders.collidersPointer;
	else
		return (Collider *) &colliders.collidersValue;
}

Physics::Collider::Collider() : type(Type::rect), collider{ {} }
{
}

Physics::Collider::Collider(FloatRect & rect) : type(Type::rect), collider{ {rect.left, rect.top, rect.width, rect.height} }
{
}

Physics::Collider::Collider(OBB & obb) : type(Type::obb), collider{ {} }
{
	collider.obb = obb;
}

Physics::Collider::Collider(FloatCircle & circle) : type(Type::circle), collider{ {} }
{
	collider.circle = circle;
}

Physics::Collider::Collider(FloatRect && rect) : type(Type::rect), collider{ std::move(rect) }
{
}

Physics::Collider::Collider(OBB && obb) : type(Type::obb), collider{ {} }
{
	collider.obb = std::move(obb);
}

Physics::Collider::Collider(FloatCircle && circle) : type(Type::circle), collider{ {} }
{
	collider.circle = std::move(circle);
}

Physics::Collider::Type Physics::Collider::GetType() const
{
	return type;
}

bool Physics::Collider::intersects(const Collider & other) const
{
	if (other.type == Type::rect && type == Type::rect)
	{
		return collider.rect.intersects(other.collider.rect);
	}
	else if (other.type == Type::circle && type == Type::circle)
	{
		Vector2f vec = collider.circle.center - other.collider.circle.center;
		if (sqrtf(vec.x * vec.x + vec.y * vec.y) < collider.circle.radius + other.collider.circle.radius)
			return true;
		else
			return false;
	}
	else
	{
		Vector2f axis[4] = {};

		Vector2f s1Points[4] = {};
		Vector2f s2Points[4] = {};

		getPointsAxis(s1Points, axis);
		other.getPointsAxis(s2Points, axis + 2);

		for (int i = 0; i < 4; ++i)
		{
			Vector2f s1MinMax = getProjectionMinMax(s1Points, axis[i], i % 2 == 0);
			Vector2f s2MinMax = getProjectionMinMax(s2Points, axis[i], i % 2 == 0);

			if ((s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x))
				return false;
			else
			{
				continue;
			}
		}

		return true;
	}
}

bool Physics::Collider::collide(const Collider & other, Vector2f *minTransVec) const
{
	if (other.type == Type::rect && type == Type::rect)
	{
		bool result = collider.rect.intersects(other.collider.rect);
		
		if (result)
		{
			FloatRect rect = collider.rect;
			FloatRect otherRect = other.collider.rect;
			
			*minTransVec = { -(rect.left - (otherRect.left + otherRect.width)), -0 };
			Vector<Vector2f> corners;
			corners.push_back(Vector2f{ (rect.left + rect.width) - otherRect.left, 0 });
			corners.push_back(Vector2f{ 0, rect.top - (otherRect.top + otherRect.height) });
			corners.push_back(Vector2f{ 0, (rect.top + rect.height) - otherRect.top });

			for (auto it = corners.begin(); it != corners.end(); ++it)
			{
				if (fabsf(minTransVec->x * minTransVec->x + minTransVec->y * minTransVec->y) > fabsf(it->x * it->x + it->y * it->y))
				{
					*minTransVec = -*it;
				}
			}
		}

		return result;
	}
	else if (other.type == Type::circle && type == Type::circle)
	{
		Vector2f vec = collider.circle.center - other.collider.circle.center;
		float lenght = sqrtf(vec.x * vec.x + vec.y * vec.y);
		if (lenght < collider.circle.radius + other.collider.circle.radius)
		{
			Vector2f normalizedVec = { vec.x / lenght, vec.y / lenght };
			float overlap = lenght - (collider.circle.radius + other.collider.circle.radius);
			*minTransVec = -(normalizedVec * overlap);

			return true;
		}
		else
			return false;
	}
	else
	{
		//NOTE: xAxis goes first!!
		Vector2f axis[4] = {};

		Vector2f s1Points[4] = {};
		Vector2f s2Points[4] = {};

		float angle = 0.0f;

		float o = std::numeric_limits<float>::max();
		Vector2f minAxis = { 0.0f, 0.0f };

		//Get x and y axis, and the points of the collider
		getPointsAxis(s1Points, axis);
		other.getPointsAxis(s2Points, axis + 2);

		for (int i = 0; i < 4; ++i)
		{
			if (axis[i].x == 0.0f && axis[i].y == 0.0f)
				continue;

			//Project points on axis with dot-product
			Vector2f s1MinMax = getProjectionMinMax(s1Points, axis[i], i % 2 == 0);
			Vector2f s2MinMax = other.getProjectionMinMax(s2Points, axis[i], i % 2 == 0);

			//Check for 1d-intersection (all axis have to intersect if the colliders collide)
			if ((s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x))
				return false;
			else
			{
				float overlap = s1MinMax.y > s2MinMax.y ? -(s2MinMax.y - s1MinMax.x) : (s1MinMax.y - s2MinMax.x);
				if (fabsf(overlap) < fabsf(o))
				{
					o = overlap;
					minAxis = axis[i];
					if (i < 2)
					{
						if (type == Type::obb)
							angle = collider.obb.angle;
						else
							angle = 0.0f;
					}
					else
					{
						if (other.type == Type::obb)
							angle = other.collider.obb.angle;
						else
							angle = 0.0f;
					}
				}
			}
		}

		*minTransVec = -Vector2f(o * minAxis.x, o * minAxis.y);

		return true;
	}
}

//NOTE: angle from degrees in radians, because cosf uses radians, but in matrix of SFML in Shape it uses degrees, so you have to convert back and forth...
Physics::OBB::OBB(float left, float top, float width, float height, float angle) : angle(utils::degreesToRadians(angle)), 
																				   xAxis(cosf(this->angle), sinf(this->angle)), 
																				   yAxis((-sinf(this->angle)), cosf(this->angle)),
																				   width(width), height(height), pos(Vector2f{ left, top }), origin(0.0f, 0.0f)
{
}

Physics::OBB::OBB(Vector2f && topLeft, float width, float height, float angle) : angle(utils::degreesToRadians(angle)),
																				 xAxis(cosf(this->angle), sinf(this->angle)), 
																				 yAxis((-sinf(this->angle)), cosf(this->angle)),
																				 width(width), height(height), pos(std::move(topLeft)), origin(0.0f, 0.0f)
{
}

Physics::OBB::OBB(float left, float top, float width, float height, float angle, Vector2f&& origin) : angle(utils::degreesToRadians(angle)), 
																									xAxis(cosf(this->angle), sinf(this->angle)), 
																									yAxis((-sinf(this->angle)), cosf(this->angle)),
																									width(width), height(height), pos(Vector2f{ left, top }), 
																									origin(std::move(origin))
{
}

Physics::OBB::OBB(Vector2f && topLeft, float width, float height, float angle, Vector2f&& origin) : angle(utils::degreesToRadians(angle)),
																								 xAxis(cosf(this->angle), sinf(this->angle)), 
																								 yAxis((-sinf(this->angle)), cosf(this->angle)),
																								 width(width), height(height), pos(std::move(topLeft)), 
																								 origin(std::move(origin))
{
}

void Physics::OBB::setAngle(float newAngle)
{
	angle = utils::degreesToRadians(newAngle);
	xAxis = Vector2f(cosf(angle), sinf(angle));
	yAxis = Vector2f(-sinf(angle), cosf(angle));
}

float Physics::OBB::getAngle() const
{
	return utils::radiansToDegrees(angle);
}

Physics::FloatCircle::FloatCircle(Vector2f && center, float radius) : radius(radius), center(std::move(center))
{
}

Physics::FloatCircle::FloatCircle(float centerX, float centerY, float radius) : radius(radius), center(centerX, centerY)
{
}
