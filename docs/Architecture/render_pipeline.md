# Render Pipeline

A render pipeline is used to keep a consistent and logical structure when rendering anything, with rendering split
into three stages - pre-render, render, and post render.

The render pipeline follows this flow:

1. Pre-render stage, sprites/primitives/stuff to be drawn is parsed and renderables (see the Renderable class) are
generated and sent out as messages for the render systems to load. Any messages generated by this point are dispatched.
2. Render stage, the renderables are taken and used to actually do the rendering, e.g. drawing to a WebGL2 canvas.
3. Post-render stage, any logic that has to be executed after rendering, e.g. updating positions for interpolation. Any
messages generated by this point are dispatched.
