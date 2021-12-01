# Bodies

A Box2D body is represented in JamJar by a `Box2DBody`.

## Using a Box2D Body

A Box2D body can be set up by attaching `Box2DBody` and `Transform` components to an entity.

```c++
#include "entity/entity.hpp"
#include "geometry/vector_2d.hpp"
#include "standard/2d/transform/transform.hpp"
#include "standard/2d/box2d/box2d_body.hpp"
#include "geometry/polygon.hpp"
...
auto body = new JamJar::Entity(messageBus);
body->Add(std::move(
    std::make_unique<JamJar::Standard::_2D::Transform>(JamJar::Vector2D(0, 30), JamJar::Vector2D(10, 10))));
body->Add(std::move(std::make_unique<JamJar::Standard::_2D::Box2DBody>(
    JamJar::Polygon({-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5}),
    JamJar::Standard::_2D::Box2DBodyProperties({.density = 1.0f, .angularVelocity = 1.0f}))));
```

This creates a body at position `x: 0, y: 30` with a scale of `x: 10, y: 10`, the shape of the body uses the coordinates
`-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5` (this is a square) and some additional properties of the body are set using
the `Box2DBodyProperties` (density is `1.0f`, initial angular velocity is `1.0f`).

### Changing Box2D Body Position/Angle

Using a `Box2DBody` and a `Transform` the `Box2DBody` component takes precdence over the transform for determining
the position, the position in the `Transform` should be treated as read only in this scenario, if you need to update
the position of the entity use the `Box2DBody.SetPosition` function.

```c++
bodyComp->SetPosition(JamJar::Vector2D(3, 2));
```

This sets the entity's position to `x: 3, y:2`.

The same applies to changing the entities angle, use the `Box2DBody.SetAngle` function:

```c++
bodyComp->SetAngle(2);
```

This sets the entity's angle to `2`.

### Scaling a Box2D Body

Due to the way Box2D handles shapes and scaling, you must adjust the entity's scale using the `Box2DBody` component by
using the `Box2DBody.SetScale` function:

```c++
bodyComp->SetScale(JamJar::Vector2D(3, 2));
```

This will adjust the scale to `x: 3, y: 2`.

#### Scaling Behind the Scenes

Box2D does not support scaling an existing body, so when you call `SetScale` this adjusts an extra scaling value, and
then marks the body as ready for regeneration using the `regenerate` property. Then in the next update the
`Box2DPhysicsSystem` will create a copy of the body's fixture before deleting the old fixture and replacing it.

For more information around how Box2D works, check out the [Box2D
documentation](https://box2d.org/documentation/index.html).
