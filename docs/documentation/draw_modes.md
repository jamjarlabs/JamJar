# Draw Modes

A `DrawMode` defines how a `Renderable` is drawn by a `RenderSystem`. These are roughly mapped to
[OpenGL Primitives](https://www.khronos.org/opengl/wiki/Primitive).

JamJarNative has the following supported draw modes:

- `POINTS` - Each vertex is a single point.
- `LINES` - Every two vertices is a line.
- `LINE_STRIP` - Every vertex is connected in a continous line from start to finish.
- `TRIANGLES` - Every three vertices is a triangle.
- `TRIANGLE_STRIP` - Every group of three adjacent vertices forms a triangle.
