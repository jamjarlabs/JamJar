# Stateful Systems

A stateful system is a system that keeps track of entities and components. The stateful system is able to filter
out which entities and components it is tracking by including some criteria that they must match to be tracked, this
is referred to as an *evaluator*.

Stateful systems group an entity and their components together by using a *system entity* which is a wrapper around
the entity and components attached to it. The system entity includes some useful helper methods to allow easy
addition, retrieval, update, and deletion of entities and components.

The StatefulSystem class is abstract, meaning that there are derived classes that provide specific ways to handle
storing the system entities, for example in a vector, or a map.

## System Entity

A system entity is a wrapper around an entity and any components that are attached to it. The system entity allows
for easier management of an entity and its components, with addition, retrieval, updating, and removal of components,
while also allowing easy access to the entity itself.

## Implementations

JamJar provides a couple of standard stateful system implementations that you can use.

### Vector System

The VectorSystem stores the system entities in a [`std::vector`](https://www.cplusplus.com/reference/vector/vector/).

#### Accessing Entities

Vector system entities can be accessed by using the `entities`
[`std::vector`](https://www.cplusplus.com/reference/vector/vector/).


```c++
for (auto &entity : this->entities) {
    ...
}
```

#### Evaluator

A vector system must include an evaluator, this is a function that filters which entities and components the
vector system should track. The evaluator takes as arguments an entity and a vector of all of its components, and
must return `true` if it should be tracked as a system entity, or `false` if it should not be tracked.

An example of a vector system evaluator looks like this:

```c++
bool JamJar::Standard::_2D::SpriteSystem::evaluator(Entity *entity,
                                                    const std::vector<JamJar::Component *> &components) {
    bool hasSprite = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Sprite::KEY) {
            hasSprite = true;
        }
        if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }
        if (hasSprite && hasTransform) {
            return true;
        }
    }
    return false;
}
```

This evaluator is used in the SpriteSystem to make sure that only components that have a Sprite component and a
Transform component are tracked.

### Map System

The MapSystem stores the system entities in a [`std::map`](https://www.cplusplus.com/reference/map/map/) that maps an
entity ID to a system entity.

#### Accessing Entities

Map system entities can be accessed by using the `entities` [`std::map`](https://www.cplusplus.com/reference/map/map/),
with the key being the ID of the entity.

```c++
auto a = this->entities.at(entityID);
```

#### Evaluator

The MapSystem uses the same evaluator as the VectorSystem.

### Bucket System

The BucketSystem stores the system entities in a [`std::map`](https://www.cplusplus.com/reference/map/map/) of
[`std::vector`](https://www.cplusplus.com/reference/vector/vector/) - with the map keys being any arbitrary `uint32_t`
that the developer can choose, for example keeping track of entities with a camera and player entities in two separate
buckets.

#### Accessing Entities

Bucket system entities can be accessed by using the bucket key to look up the bucket, then the entities in the bucket
can be iterated over since they are in a [`std::vector`](https://www.cplusplus.com/reference/vector/vector/).

```c++
auto players = this->entities.at(PLAYER_BUCKET);

for (auto& player : players) {
    ...
```

#### Evaluator

A bucket system must include an evaluator, this is a function that filters which entities and components the
bucket system should track. The evaluator takes as arguments an entity and a vector of all of its components, and
must return either a bucket key as a `unit32_t` or an empty optional value to mark that the entity should not be
tracked.

An example of a bucket system evaluator looks like this:

```c++
std::optional<uint32_t> CustomSystem::evaluator(JamJar::Entity *entity, const std::vector<JamJar::Component *> &components) {
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Camera::KEY) {
            return std::optional<uint32_t>(CustomSystem::CAMERA_BUCKET);
        }
        if (component->key == Player::KEY) {
            return std::optional<uint32_t>(CustomSystem::PLAYER_BUCKET);
        }
    }
    return std::optional<uint32_t>(std::nullopt);
}
```

This evaluator tracks entities that have either a Camera or a Player component, splitting them into two buckets.

### Bucket Map System

The BucketMapSystem stores the system entities in a [`std::map`](https://www.cplusplus.com/reference/map/map/) of
[`std::map`](https://www.cplusplus.com/reference/map/map/) - with the outer map keys being any arbitrary `uint32_t`
that the developer can choose, for example keeping track of entities with a camera and player entities in two separate
buckets, and the inner map keys being entity IDs.

#### Accessing Entities

Bucket system entities can be accessed by using the bucket key to look up the bucket, then the entities in the bucket
can be accessed using the inner [`std::map`](https://www.cplusplus.com/reference/map/map/) which is mapped to entity
IDs.

```c++
auto projectiles = this->entities.at(PROJECTILE_BUCKET);

auto collisionProjectile = projectiles.at(collision.aID);
```

#### Evaluator

The BucketMapSystem uses the same evaluator as the BucketSystem.
