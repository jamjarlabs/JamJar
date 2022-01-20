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
    JamJar::Vector2D scale;
    bool regenerate;

    /**
     * Do not use this method unless you know what you are doing! This function should really only be used by the
     * Box2DPhysicsSystem.
     */
    void SetBody(b2Body *body);

    void SetScale(const JamJar::Vector2D &scale);

    void SetPosition(const JamJar::Vector2D &position);

    void SetAngle(float angle);

    [[nodiscard]] Vector2D GetPosition() const;

    [[nodiscard]] float GetAngle() const;

    void SetLinearVelocity(const Vector2D &v);

    [[nodiscard]] Vector2D GetLinearVelocity() const;

    void SetAngularVelocity(float omega);

    [[nodiscard]] float GetAngularVelocity() const;

    void ApplyForce(const Vector2D &force, const Vector2D &point, bool wake);

    void ApplyForceToCenter(const Vector2D &force, bool wake);

    void ApplyTorque(float torque, bool wake);

    void ApplyLinearImpulse(const Vector2D &impulse, const Vector2D &point, bool wake);

    void ApplyLinearImpulseToCenter(const Vector2D &impulse, bool wake);

    void ApplyAngularImpulse(float impulse, bool wake);

    [[nodiscard]] float GetMass() const;

    [[nodiscard]] float GetInertia() const;

    [[nodiscard]] float GetLinearDamping() const;

    void SetLinearDamping(float linearDamping);

    [[nodiscard]] float GetAngularDamping() const;

    void SetAngularDamping(float angularDamping);

    [[nodiscard]] float GetGravityScale() const;

    void SetGravityScale(float scale);

    void SetType(Box2DBodyType type);

    [[nodiscard]] Box2DBodyType GetType() const;

    void SetBullet(bool flag);

    [[nodiscard]] bool IsBullet() const;

    void SetSleepingAllowed(bool flag);

    [[nodiscard]] bool IsSleepingAllowed() const;

    void SetAwake(bool flag);

    [[nodiscard]] bool IsAwake() const;

    void SetEnabled(bool flag);

    [[nodiscard]] bool IsEnabled() const;

    void SetFixedRotation(bool flag);

    [[nodiscard]] bool IsFixedRotation() const;

  private:
    b2Body *body;
};
}; // namespace JamJar::Standard::_2D

#endif
