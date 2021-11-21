#include "standard/2d/box2d/box2d_body.hpp"

std::unordered_map<JamJar::Standard::_2D::Box2DBodyType, b2BodyType> JamJar::Standard::_2D::Box2DBody::BODY_TYPES = {
    {Box2DBodyType::STATIC, b2_staticBody},
    {Box2DBodyType::KINEMATIC, b2_kinematicBody},
    {Box2DBodyType::DYNAMIC, b2_dynamicBody}};

std::unordered_map<b2BodyType, JamJar::Standard::_2D::Box2DBodyType>
    JamJar::Standard::_2D::Box2DBody::REVERSE_BODY_TYPES = {{b2_staticBody, Box2DBodyType::STATIC},
                                                            {b2_kinematicBody, Box2DBodyType::KINEMATIC},
                                                            {b2_dynamicBody, Box2DBodyType::DYNAMIC}};

JamJar::Standard::_2D::Box2DBody::Box2DBody(Polygon polygon, Box2DBodyProperties properties)
    : Component(JamJar::Standard::_2D::Box2DBody::KEY), polygon(polygon), initializationProperties(properties),
      body(nullptr) {}

JamJar::Standard::_2D::Box2DBody::Box2DBody(Polygon polygon) : Box2DBody(polygon, Box2DBodyProperties()) {}

void JamJar::Standard::_2D::Box2DBody::SetBody(b2Body *body) { this->body = body; }

JamJar::Vector2D JamJar::Standard::_2D::Box2DBody::GetPosition() const {
    auto pos = this->body->GetPosition();
    return Vector2D(pos.x, pos.y);
}

float JamJar::Standard::_2D::Box2DBody::GetAngle() const { return this->body->GetAngle(); }

void JamJar::Standard::_2D::Box2DBody::SetLinearVelocity(JamJar::Vector2D v) {
    this->body->SetLinearVelocity(b2Vec2(v.x, v.y));
}

JamJar::Vector2D JamJar::Standard::_2D::Box2DBody::GetLinearVelocity() const {
    auto vel = this->body->GetLinearVelocity();
    return Vector2D(vel.x, vel.y);
}

void JamJar::Standard::_2D::Box2DBody::SetAngularVelocity(float omega) { this->body->SetAngularVelocity(omega); }

float JamJar::Standard::_2D::Box2DBody::GetAngularVelocity() const { return this->body->GetAngularVelocity(); }

void JamJar::Standard::_2D::Box2DBody::ApplyForce(Vector2D force, Vector2D point, bool wake) {
    this->body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
}

void JamJar::Standard::_2D::Box2DBody::ApplyForceToCenter(Vector2D force, bool wake) {
    this->body->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void JamJar::Standard::_2D::Box2DBody::ApplyTorque(float torque, bool wake) { this->body->ApplyTorque(torque, wake); }

void JamJar::Standard::_2D::Box2DBody::ApplyLinearImpulse(Vector2D impulse, Vector2D point, bool wake) {
    this->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.y, point.y), wake);
}

void JamJar::Standard::_2D::Box2DBody::ApplyLinearImpulseToCenter(Vector2D impulse, bool wake) {
    this->body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), wake);
}

void JamJar::Standard::_2D::Box2DBody::ApplyAngularImpulse(float impulse, bool wake) {
    this->body->ApplyAngularImpulse(impulse, wake);
}

float JamJar::Standard::_2D::Box2DBody::GetMass() const { return this->body->GetMass(); }

float JamJar::Standard::_2D::Box2DBody::GetInertia() const { return this->body->GetInertia(); }

float JamJar::Standard::_2D::Box2DBody::GetLinearDamping() const { return this->body->GetLinearDamping(); }

void JamJar::Standard::_2D::Box2DBody::SetLinearDamping(float linearDamping) {
    this->body->SetLinearDamping(linearDamping);
}

float JamJar::Standard::_2D::Box2DBody::GetAngularDamping() const { return this->body->GetAngularDamping(); }

void JamJar::Standard::_2D::Box2DBody::SetAngularDamping(float angularDamping) {
    this->body->SetAngularDamping(angularDamping);
}

float JamJar::Standard::_2D::Box2DBody::GetGravityScale() const { return this->body->GetGravityScale(); }

void JamJar::Standard::_2D::Box2DBody::SetGravityScale(float scale) { this->body->SetGravityScale(scale); }

void JamJar::Standard::_2D::Box2DBody::SetType(Box2DBodyType type) {
    this->body->SetType(Box2DBody::BODY_TYPES.at(type));
}

JamJar::Standard::_2D::Box2DBodyType JamJar::Standard::_2D::Box2DBody::GetType() const {
    return Box2DBody::REVERSE_BODY_TYPES.at(this->body->GetType());
}

void JamJar::Standard::_2D::Box2DBody::SetBullet(bool flag) { this->body->SetBullet(flag); }

bool JamJar::Standard::_2D::Box2DBody::IsBullet() const { return this->body->IsBullet(); }

void JamJar::Standard::_2D::Box2DBody::SetSleepingAllowed(bool flag) { this->body->SetSleepingAllowed(flag); }

bool JamJar::Standard::_2D::Box2DBody::IsSleepingAllowed() const { return this->body->IsSleepingAllowed(); }

void JamJar::Standard::_2D::Box2DBody::SetAwake(bool flag) { this->body->SetAwake(flag); }

bool JamJar::Standard::_2D::Box2DBody::IsAwake() const { return this->body->IsAwake(); }

void JamJar::Standard::_2D::Box2DBody::SetEnabled(bool flag) { this->body->SetEnabled(flag); }

bool JamJar::Standard::_2D::Box2DBody::IsEnabled() const { return this->body->IsEnabled(); }

void JamJar::Standard::_2D::Box2DBody::SetFixedRotation(bool flag) { this->body->SetFixedRotation(flag); }

bool JamJar::Standard::_2D::Box2DBody::IsFixedRotation() const { return this->body->IsFixedRotation(); }
