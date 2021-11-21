#ifndef STANDARD_2D_BOX2D_BOX2D_BODY_HPP
#define STANDARD_2D_BOX2D_BOX2D_BODY_HPP

#include "box2d/box2d.h"
#include "component/component.hpp"
#include "geometry/polygon.hpp"
#include "geometry/vector_2d.hpp"
#include <unordered_map>
namespace JamJar::Standard::_2D {
enum Box2DBodyType { STATIC = 0, KINEMATIC, DYNAMIC };
struct Box2DBodyProperties {
    Box2DBodyType bodyType = DYNAMIC;
    float friction = 0.2f;
    float restitution = 0.0f;
    float restitutionThreshold = 1.0f * b2_lengthUnitsPerMeter;
    float density = 0.0f;
    bool isSensor = false;
    Vector2D linearVelocity = Vector2D(0, 0);
    float angularVelocity = 0.0f;
    float linearDamping = 0.0f;
    float angularDamping = 0.0f;
    float gravityScale = 1.0f;
    bool allowSleep = true;
    bool awake = true;
    bool fixedRotation = false;
    bool bullet = false;
    bool enabled = true;
};
class Box2DBody : public Component {
  public:
    constexpr static uint32_t KEY = JamJar::hash("jamjar_2d_box2d_body");
    static std::unordered_map<JamJar::Standard::_2D::Box2DBodyType, b2BodyType> BODY_TYPES;
    static std::unordered_map<b2BodyType, JamJar::Standard::_2D::Box2DBodyType> REVERSE_BODY_TYPES;

    explicit Box2DBody(JamJar::Polygon polygon);
    explicit Box2DBody(JamJar::Polygon polygon, Box2DBodyProperties properties);

    JamJar::Polygon polygon;
    Box2DBodyProperties initializationProperties;

    /**
     * Do not use this method unless you know what you are doing! This function should really only be used by the
     * Box2DPhysicsSystem.
     */
    void SetBody(b2Body *body);

    Vector2D GetPosition() const;

    float GetAngle() const;

    void SetLinearVelocity(Vector2D v);

    Vector2D GetLinearVelocity() const;

    void SetAngularVelocity(float omega);

    float GetAngularVelocity() const;

    void ApplyForce(Vector2D force, Vector2D point, bool wake);

    void ApplyForceToCenter(Vector2D force, bool wake);

    void ApplyTorque(float torque, bool wake);

    void ApplyLinearImpulse(Vector2D impulse, Vector2D point, bool wake);

    void ApplyLinearImpulseToCenter(Vector2D impulse, bool wake);

    void ApplyAngularImpulse(float impulse, bool wake);

    float GetMass() const;

    float GetInertia() const;

    float GetLinearDamping() const;

    void SetLinearDamping(float linearDamping);

    float GetAngularDamping() const;

    void SetAngularDamping(float angularDamping);

    float GetGravityScale() const;

    void SetGravityScale(float scale);

    void SetType(Box2DBodyType type);

    Box2DBodyType GetType() const;

    void SetBullet(bool flag);

    bool IsBullet() const;

    void SetSleepingAllowed(bool flag);

    bool IsSleepingAllowed() const;

    void SetAwake(bool flag);

    bool IsAwake() const;

    void SetEnabled(bool flag);

    bool IsEnabled() const;

    void SetFixedRotation(bool flag);

    bool IsFixedRotation() const;

  private:
    b2Body *body;
};
}; // namespace JamJar::Standard::_2D

#endif
