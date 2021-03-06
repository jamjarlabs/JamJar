/*
Copyright 2021 JamJar Authors

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

import {
    Game,
    IMessageBus,
    EntityManager,
    MessageBus,
    WebGLSystem,
    Entity,
    Transform,
    Camera,
    Material,
    Vector,
    Motion,
    Renderable,
    Message,
    Matrix3D,
    Matrix4D,
    CanvasResizeSystem,
    KeyboardSystem,
    IMessage,
    IEntity,
    Component,
    ArraySystem,
    SpriteSystem,
    HTTPImageSystem,
    TextureFiltering,
    ImageRequest,
    Sprite,
    Texture,
    TextSystem,
    FontRequest,
    Text,
    TextAlignment,
    Color,
    FullscreenSystem,
    CanvasResize,
} from "jamjar";

class AspectControlSystem extends ArraySystem {
    public static readonly RESOLUTION_AND_ASPECT: [[number, number], [number, number]][] = [
        [
            [1, 1],
            [1000, 1000],
        ],
        [
            [4, 3],
            [640, 480],
        ],
        [
            [4, 3],
            [800, 600],
        ],
        [
            [16, 10],
            [1440, 900],
        ],
        [
            [16, 10],
            [640, 400],
        ],
        [
            [16, 9],
            [3840, 2160],
        ],
        [
            [16, 9],
            [1920, 1080],
        ],
        [
            [16, 9],
            [1366, 768],
        ],
        [
            [16, 9],
            [1024, 576],
        ],
        [
            [16, 9],
            [640, 360],
        ],
        [
            [9, 16],
            [360, 640],
        ],
        [
            [9, 16],
            [576, 1024],
        ],
        [
            [9, 16],
            [768, 1366],
        ],
    ];
    public static readonly VIRTUAL_HEIGHT = 100;

    private static readonly EVALUATOR = (entity: IEntity, components: Component[]): boolean => {
        return (
            components.some((component) => component.key === Camera.KEY) ||
            components.some((component) => component.key === Text.KEY)
        );
    };

    private index: number;
    private maxResSet: boolean;

    constructor(messageBus: IMessageBus, index = 0) {
        super(messageBus, undefined, AspectControlSystem.EVALUATOR);
        this.index = index;
        this.maxResSet = true;
        this.messageBus.Subscribe(this, ["keydown"]);
        this.initialiseAspectRatio();
    }

    public OnMessage(message: IMessage): void {
        super.OnMessage(message);
        switch (message.type) {
            case "keydown": {
                const keyMessage = message as Message<string>;
                if (keyMessage.payload === undefined) {
                    return;
                }
                switch (keyMessage.payload) {
                    case "ArrowUp": {
                        this.index++;
                        if (this.index >= AspectControlSystem.RESOLUTION_AND_ASPECT.length) {
                            this.index = 0;
                        }
                        this.changeAspectRatio();
                        break;
                    }
                    case "ArrowDown": {
                        this.index--;
                        if (this.index < 0) {
                            this.index = AspectControlSystem.RESOLUTION_AND_ASPECT.length - 1;
                        }
                        this.changeAspectRatio();
                        break;
                    }
                    case "KeyF": {
                        this.messageBus.Publish(new Message(FullscreenSystem.MESSAGE_REQUEST_ENTER_FULLSCREEN));
                        break;
                    }
                    case "KeyX": {
                        this.maxResSet = !this.maxResSet;
                        this.changeAspectRatio();
                        break;
                    }
                }
                break;
            }
        }
    }

    private initialiseAspectRatio() {
        const ratios: [number, number][] = [];
        for (const resolutionAndAspect of AspectControlSystem.RESOLUTION_AND_ASPECT) {
            ratios.push(resolutionAndAspect[0]);
        }

        const initialAspect = CanvasResize.GetClosestAspectRatioToResolution(screen.width, screen.height, ratios);

        for (let i = 0; i < AspectControlSystem.RESOLUTION_AND_ASPECT.length; i++) {
            if (AspectControlSystem.RESOLUTION_AND_ASPECT[i][0] === initialAspect) {
                this.index = i;
            }
        }
        this.changeAspectRatio();
    }

    private changeAspectRatio() {
        const aspect = AspectControlSystem.RESOLUTION_AND_ASPECT[this.index][0];
        const resolution = AspectControlSystem.RESOLUTION_AND_ASPECT[this.index][1];
        this.messageBus.Publish(new Message(CanvasResizeSystem.MESSAGE_SET_ASPECT_RATIO, aspect[0] / aspect[1]));
        if (this.maxResSet) {
            this.messageBus.Publish(
                new Message<[number, number]>(CanvasResizeSystem.MESSAGE_SET_MAX_RESOLUTION, resolution)
            );
        } else {
            this.messageBus.Publish(new Message(CanvasResizeSystem.MESSAGE_SET_MAX_RESOLUTION, null));
        }
        // Delete any existing cameras
        for (const entity of this.entities) {
            if (entity.Get(Camera.KEY) === undefined) {
                continue;
            }
            entity.Destroy();
        }
        this.resolutionChange();
    }

    private resolutionChange(): void {
        this.createCamera();
        this.updateText();
    }

    private createCamera(): void {
        const aspect = AspectControlSystem.RESOLUTION_AND_ASPECT[this.index][0];
        const cameraEntity = new Entity(this.messageBus);
        cameraEntity.Add(
            new Camera(
                new Color(0.23921568627, 0.23921568627, 0.23921568627),
                undefined,
                undefined,
                new Vector(
                    AspectControlSystem.VIRTUAL_HEIGHT * (aspect[0] / aspect[1]),
                    AspectControlSystem.VIRTUAL_HEIGHT
                )
            )
        );
        cameraEntity.Add(new Transform());
    }

    private updateText(): void {
        const aspect = AspectControlSystem.RESOLUTION_AND_ASPECT[this.index][0];
        const resolution = AspectControlSystem.RESOLUTION_AND_ASPECT[this.index][1];
        let numOfText = 0;
        for (const entity of this.entities) {
            const text = entity.Get(Text.KEY) as Text | undefined;
            if (text === undefined) {
                continue;
            }
            numOfText++;
            if (entity.entity.tags.includes("aspect_ratio")) {
                text.value = `aspect ratio: ${aspect[0]}:${aspect[1]}`;
            }
            if (entity.entity.tags.includes("resolution")) {
                if (this.maxResSet) {
                    text.value = `max resolution: ${resolution[0]}x${resolution[1]}`;
                } else {
                    text.value = `max resolution: not set`;
                }
            }
        }

        if (numOfText <= 0) {
            const aspectRatioText = new Entity(this.messageBus, ["aspect_ratio"]);
            aspectRatioText.Add(new Transform(Vector.New(0, 22), Vector.New(3, 3)));
            aspectRatioText.Add(new Motion(undefined, undefined, 1));
            aspectRatioText.Add(
                new Text(
                    0,
                    `aspect ratio: ${aspect[0]}:${aspect[1]}`,
                    "arial",
                    TextAlignment.Center,
                    undefined,
                    undefined,
                    new Color(1, 0, 0)
                )
            );

            const resolutionText = new Entity(this.messageBus, ["resolution"]);
            resolutionText.Add(new Transform(Vector.New(0, 18), Vector.New(3, 3)));
            resolutionText.Add(new Motion(undefined, undefined, 1));
            resolutionText.Add(
                new Text(
                    0,
                    `max resolution: ${resolution[0]}x${resolution[1]}`,
                    "arial",
                    TextAlignment.Center,
                    undefined,
                    undefined,
                    new Color(1, 0, 0)
                )
            );
        }
    }
}

class ResizableGame extends Game {
    private document: HTMLDocument;
    private gl: WebGL2RenderingContext;
    private canvas: HTMLCanvasElement;
    private wrapper: HTMLElement;

    constructor(
        messageBus: IMessageBus,
        document: HTMLDocument,
        gl: WebGL2RenderingContext,
        canvas: HTMLCanvasElement,
        wrapper: HTMLElement
    ) {
        super(messageBus, "resizable-game");
        this.document = document;
        this.gl = gl;
        this.canvas = canvas;
        this.wrapper = wrapper;
    }

    OnStart(): void {
        new WebGLSystem(messageBus, this.gl);
        new SpriteSystem(messageBus);
        new HTTPImageSystem(messageBus);
        new KeyboardSystem(messageBus, this.document);
        new TextSystem(messageBus);
        new CanvasResizeSystem(messageBus, this.canvas, this.wrapper);
        new FullscreenSystem(messageBus, this.canvas, this.document);
        new AspectControlSystem(messageBus);

        this.messageBus.Publish(
            Message.New<FontRequest>(FontRequest.MESSAGE_REQUEST_LOAD, new FontRequest("arial", "arial", "normal", 100))
        );

        this.messageBus.Publish(
            Message.New<ImageRequest>(
                ImageRequest.MESSAGE_REQUEST_LOAD,
                new ImageRequest("example", "assets/example.png", {
                    minFilter: TextureFiltering.NEAREST,
                    magFilter: TextureFiltering.NEAREST,
                })
            )
        );

        const picture = new Entity(this.messageBus);
        picture.Add(new Transform(Vector.New(0, 0), Vector.New(100, 56.25)));
        picture.Add(new Motion(undefined, undefined, 1));
        picture.Add(
            new Sprite(
                new Material({
                    texture: new Texture("example"),
                })
            )
        );
    }
}

if (window.JamJar === undefined) {
    throw "Missing required JamJar configuration options";
}

if (window.JamJar.CanvasID === undefined) {
    throw "Missing required CanvasID";
}

if (window.JamJar.CanvasWrapperID === undefined) {
    throw "Missing required CanvasWrapperID";
}

// Get canvas
const canvas = document.getElementById(window.JamJar.CanvasID) as HTMLCanvasElement;

// Get WebGL2 context
const gl = canvas.getContext("webgl2", { alpha: false });
if (!gl) {
    throw "WebGL2 not supported in this browser";
}

const wrapper = document.getElementById(window.JamJar.CanvasWrapperID);

if (wrapper === null) {
    throw `Invalid CanvasWrapperID provided, no wrapper found with ID ${window.JamJar.CanvasWrapperID}`;
}

// Set up vector and renderable pooling
Vector.Init(200);
Renderable.Init(100);
Message.Init(500);
Matrix4D.Init(30);
Matrix3D.Init(30);

// Create message bus and entity manager
const messageBus = new MessageBus();
new EntityManager(messageBus);

// Create and start game
new ResizableGame(messageBus, document, gl, canvas, wrapper).Start();
