# Stateful Systems

A stateful system is a system that keeps track of entities and components. The stateful system is able to filter
out which entities and components it is tracking by including some criteria that they must match to be tracked, this
is referred to as an *evaluator*.

Stateful systems group an entity and their components together by using a *system entity* which is a wrapper around
the entity and components attached to it. The system entity includes some useful helper methods to allow easy
addition, retrieval, update, and deletion of entities and components.

The StatefulSystem class is abstract, meaning that there are derived classes that provide specific ways to handle
storing the system entities, for example in a vector, or a map.

## Evaluator

A stateful system must include an evaluator, this is a function that filters which entities and components the
stateful system should track. The evaluator takes as arguments an entity and a vector of all of its components, and
must return `true` if it should be tracked as a system entity, or `false` if it should not be tracked.

An example of an evaluator looks like this:

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

## System Entity

A system entity is a wrapper around an entity and any components that are attached to it. The system entity allows
for easier management of an entity and its components, with addition, retrieval, updating, and removal of components,
while also allowing easy access to the entity itself.

## Implementations

The core stateful systems that are provided are:

- MapSystem - stores the system entities in a [`std::map`](https://www.cplusplus.com/reference/map/map/) that maps an
entity ID to a system entity.
