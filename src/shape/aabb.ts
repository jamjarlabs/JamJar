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

import IShape from "./ishape";
import Vector from "../geometry/vector";
import Transform from "../standard/transform/transform";

/**
 * AABB is an Axis-Aligned Bounding-Box, this is a rectangle that has no
 * rotation; it is aligned with the X and Y axis.
 * The AABB is defined by a centre and width and height dimensions, it can
 * be used for faster collision detection as it is a more simple shape,
 * requiring less calculations than a fully defined polygon.
 */
class AABB implements IShape {
    public center: Vector;
    public size: Vector;

    constructor(size: Vector, center: Vector = Vector.New(0, 0)) {
        this.center = center;
        this.size = size;
    }

    public Center(): Vector {
        return this.center.Copy();
    }

    public Transform(transform: Transform): IShape {
        this.size.Multiply(transform.scale);
        this.center.Add(transform.position);
        return this;
    }

    public FarthestPointInDirection(direction: Vector): Vector {
        if (direction.x >= 0) {
            const right = this.center.x + this.size.x / 2;
            if (direction.y >= 0) {
                return Vector.New(right, this.center.y + this.size.y / 2); // top right
            } else {
                return Vector.New(right, this.center.y - this.size.y / 2); // bottom right
            }
        } else {
            const left = this.center.x - this.size.x / 2;
            if (direction.y >= 0) {
                return Vector.New(left, this.center.y + this.size.y / 2); // top left
            } else {
                return Vector.New(left, this.center.y - this.size.y / 2); // bottom left
            }
        }
    }

    public PointInside(point: Vector): boolean {
        const left = this.center.x - this.size.x / 2;
        const right = this.center.x + this.size.x / 2;
        const top = this.center.y + this.size.y / 2;
        const bottom = this.center.y - this.size.y / 2;
        return point.x < right && point.x > left && point.y < top && point.y > bottom;
    }

    public Copy(): AABB {
        return new AABB(this.size.Copy(), this.center.Copy());
    }

    public Free(): void {
        this.center.Free();
        this.size.Free();
    }
}

export default AABB;
