/*
Copyright 2020 JamJar Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

import Component from "../../component/component";
import Vector from "../../geometry/vector";

/**
 * Motion is a component that represents an entity with motion values.
 * Holds info such as velocity, acceleration, angular velocity and angular acceleration.
 */
class Motion extends Component {
    /**
     * Key of the motion component.
     */
    public static readonly KEY = "motion";
    /**
     * Velocity of the entity.
     */
    public velocity: Vector;
    /**
     * Acceleration of the entity.
     */
    public acceleration: Vector;
    /**
     * Angular velocity of the entity around the Z axis, scalar value.
     */
    public angularVelocity: number;
    /**
     * Angular acceleration of the entity around the Z axis, scalar value.
     */
    public angularAcceleration: number;

    constructor(
        velocity = Vector.New(0, 0),
        acceleration = Vector.New(0, 0),
        angularVelocity = 0,
        angularAcceleration = 0
    ) {
        super(Motion.KEY);
        this.velocity = velocity;
        this.acceleration = acceleration;
        this.angularVelocity = angularVelocity;
        this.angularAcceleration = angularAcceleration;
    }

    public Free(): void {
        this.velocity.Free();
        this.acceleration.Free();
    }
}

export default Motion;
