# Texture Loading

Texture loading can be done through the file system (either actual filesystem, or [a virtual file system provided
by Emscripten](https://emscripten.org/docs/porting/files/index.html)).

The `FileTextureSystem` handles loading images from a filesystem by parsing any `FileTextureRequest` sent to the system
with the appropriate message. The system will then respond with a `FileTextureResponse` with the status of the request
(success or failure) and the loaded image data. Some systems will automatically link into this (rendering systems) to
generate textures once an image has been loaded.

## Setting up the File Texture System

To set up a FileTextureSystem, you simply need to provide a message bus.

```c++
new JamJar::Standard::FileTextureSystem(messageBus);
```

## Data File Packaging

If targeting the browser a game can use [Emscripten's data file
packaging](https://emscripten.org/docs/porting/files/packaging_files.html#packaging-files) to create a virtual file
system that allows images and assets to be loaded using standard file system calls.

## Loading an Image

An image can be loaded by using the helper `LoadTexture` function:

```c++
JamJar::Standard::LoadTexture(new JamJar::Standard::FileTextureRequest({JamJar::hash("smiley"), "/assets/texture.png"}));
```

This requests that the `/assets/texture.png` image is loaded, and once it has loaded it will be referrable using the
`smiley` hash.

### Behind the scenes

This helper function simply wraps up the `FileTextureRequest` in a `unique_ptr` to manage its memory, then publishes
it with the message type `MESSAGE_REQUEST_FILE_TEXTURE_LOAD`.

### Advanced Options

The `FileTextureRequest` takes a set of `TextureProperties` which configure how the image should be parsed as a texture,
with options around mapping, filtering, and if mipmaps should be generated:

```c++
new JamJar::Standard::FileTextureRequest({
        .key = JamJar::hash("smiley"),
        .path = "/assets/texture.png",
        .properties = JamJar::TextureProperties({
            .minFilter = JamJar::TextureFilter::NEAREST,
            .magFilter = JamJar::TextureFilter::NEAREST
        })
});
```

This will load the texture and apply the filtering option of `NEAREST` when it is processed by the rendering system.
