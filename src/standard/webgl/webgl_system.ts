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

import Transform from "../transform/transform";
import IMessage from "../../message/imessage";
import Camera from "../camera/camera";
import Message from "../../message/message";
import IMessageBus from "../../message/imessage_bus";
import Game from "../../game";
import IEntity from "../../entity/ientity";
import Component from "../../component/component";
import IScene from "../../scene/iscene";
import SystemEntity from "../../system/system_entity";
import ImageAsset from "../../rendering/image/image_asset";
import ShaderAsset from "../../rendering/shader/shader_asset";
import GLSLShader from "../glsl/glsl_shader";
import GLSLContext from "../glsl/glsl_context";
import DefaultTextureVertexShader from "./default_texture_vertex_shader";
import DefaultTextureFragmentShader from "./default_texture_fragment_shader";
import RenderSystem from "../render/render_system";
import DefaultTextFragmentShader from "./default_text_fragment_shader";
import IRenderable from "../../rendering/irenderable";
import TextureFiltering from "../../rendering/texture/texture_filtering";
import TextureWrapping from "../../rendering/texture/texture_wrapping";
import DrawMode from "../../rendering/draw_mode";
import DefaultPrimitiveFragmentShader from "./default_primitive_fragment_shader";
import DefaultPrimitiveVertexShader from "./default_primitive_vertex_shader";

/**
 * WebGLSystem handles rendering to an HTML5 canvas using WebGL.
 * Takes in renderables created by pre rendering steps and
 * renders them onto a canvas.
 */
class WebGLSystem extends RenderSystem {
    private static readonly EVALUATOR = (entity: IEntity, components: Component[]): boolean => {
        return [Transform.KEY, Camera.KEY].every((type) => components.some((component) => component.key === type));
    };

    private static readonly FILTER_MODES = new Map<TextureFiltering, number>(
        new Map([
            [TextureFiltering.NEAREST, WebGL2RenderingContext.NEAREST],
            [TextureFiltering.BILINEAR, WebGL2RenderingContext.LINEAR],
            [TextureFiltering.TRILINEAR, WebGL2RenderingContext.LINEAR_MIPMAP_LINEAR],
        ])
    );

    private static readonly WRAP_MODES = new Map<TextureWrapping, number>(
        new Map([
            [TextureWrapping.REPEAT, WebGL2RenderingContext.REPEAT],
            [TextureWrapping.MIRRORED_REPEAT, WebGL2RenderingContext.MIRRORED_REPEAT],
            [TextureWrapping.CLAMP_TO_EDGE, WebGL2RenderingContext.CLAMP_TO_EDGE],
        ])
    );

    private static readonly DRAW_MODES = new Map<DrawMode, number>(
        new Map([
            [DrawMode.POINTS, WebGL2RenderingContext.POINTS],
            [DrawMode.LINES, WebGL2RenderingContext.LINES],
            [DrawMode.LINE_STRIP, WebGL2RenderingContext.LINE_STRIP],
            [DrawMode.TRIANGLES, WebGL2RenderingContext.TRIANGLES],
            [DrawMode.TRIANGLE_STRIP, WebGL2RenderingContext.TRIANGLE_STRIP],
        ])
    );

    private gl: WebGL2RenderingContext;
    private textures: Map<string, WebGLTexture>;
    private shaders: Map<string, [WebGLShader, GLSLShader]>;
    private programs: Map<string, WebGLProgram>;
    private zOrderGroups: Map<number, IRenderable[]>;
    private programGroups: Map<string, IRenderable[]>;
    private textureGroups: Map<string, IRenderable[]>;
    private cameraRenderables: IRenderable[];

    constructor(
        messageBus: IMessageBus,
        gl: WebGL2RenderingContext,
        scene?: IScene,
        renderables?: IRenderable[],
        defaultShaderAssets: ShaderAsset[] = [
            new ShaderAsset(ShaderAsset.DEFAULT_TEXTURE_FRAGMENT_SHADER_NAME, new DefaultTextureFragmentShader()),
            new ShaderAsset(ShaderAsset.DEFAULT_TEXTURE_VERTEX_SHADER_NAME, new DefaultTextureVertexShader()),
            new ShaderAsset(ShaderAsset.DEFAULT_TEXT_FRAGMENT_SHADER_NAME, new DefaultTextFragmentShader()),
            new ShaderAsset(ShaderAsset.DEFAULT_PRIMITIVE_FRAGMENT_SHADER_NAME, new DefaultPrimitiveFragmentShader()),
            new ShaderAsset(ShaderAsset.DEFAULT_PRIMITIVE_VERTEX_SHADER_NAME, new DefaultPrimitiveVertexShader()),
        ],
        shaders: Map<string, [WebGLShader, GLSLShader]> = new Map(),
        textures: Map<string, WebGLTexture> = new Map(),
        programs: Map<string, WebGLProgram> = new Map(),
        entities?: SystemEntity[],
        subscriberID?: number
    ) {
        super(messageBus, scene, WebGLSystem.EVALUATOR, renderables, entities, subscriberID);

        this.gl = gl;
        this.textures = textures;
        this.shaders = shaders;
        this.programs = programs;
        this.zOrderGroups = new Map();
        this.programGroups = new Map();
        this.textureGroups = new Map();
        this.cameraRenderables = [];

        // Subscribe to messages
        this.messageBus.Subscribe(this, [
            Game.MESSAGE_RENDER,
            RenderSystem.MESSAGE_LOAD_RENDERABLES,
            ImageAsset.MESSAGE_FINISH_LOAD,
            ShaderAsset.MESSAGE_REQUEST_LOAD,
        ]);

        // Load default shaders
        for (const asset of defaultShaderAssets) {
            this.messageBus.Publish(Message.New<ShaderAsset>(ShaderAsset.MESSAGE_REQUEST_LOAD, asset));
        }
    }

    public OnMessage(message: IMessage): void {
        super.OnMessage(message);
        switch (message.type) {
            case Game.MESSAGE_RENDER: {
                const renderMessage = message as Message<number>;
                if (renderMessage.payload === undefined) {
                    return;
                }
                this.render(renderMessage.payload);
                break;
            }
            case ImageAsset.MESSAGE_FINISH_LOAD: {
                const loadMessage = message as Message<ImageAsset>;
                const asset = loadMessage.payload;
                if (asset === undefined) {
                    return;
                }
                this.loadTexture(asset);
                break;
            }
            case ShaderAsset.MESSAGE_REQUEST_LOAD: {
                const loadMessage = message as Message<ShaderAsset>;
                const asset = loadMessage.payload;
                if (asset === undefined) {
                    return;
                }
                this.loadShader(asset);
                break;
            }
        }
    }

    private loadShader(asset: ShaderAsset): void {
        if (!(asset.shader instanceof GLSLShader)) {
            return;
        }

        const gl = this.gl;
        let type: number;
        switch (asset.shader.type) {
            case ShaderAsset.VERTEX_TYPE: {
                type = gl.VERTEX_SHADER;
                break;
            }
            case ShaderAsset.FRAGMENT_TYPE: {
                type = gl.FRAGMENT_SHADER;
                break;
            }
            default: {
                return;
            }
        }

        const shader = gl.createShader(type);
        if (shader === null) {
            throw `Error creating shader for asset '${asset.name}'`;
        }
        gl.shaderSource(shader, asset.shader.source);
        gl.compileShader(shader);
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            const err = gl.getShaderInfoLog(shader);
            gl.deleteShader(shader);
            throw `Error compiling shader for asset '${asset.name}', error: ${err}`;
        }
        this.shaders.set(asset.name, [shader, asset.shader]);
        this.messageBus.Publish(Message.New(ShaderAsset.MESSAGE_FINISH_LOAD));
    }

    private loadTexture(asset: ImageAsset): void {
        if (!asset.success) {
            return;
        }

        // Find GL modes
        const xWrap = WebGLSystem.WRAP_MODES.get(asset.xWrap);
        if (xWrap === undefined) {
            console.warn(`Unsupported x wrapping mode: ${asset.xWrap}`);
            return;
        }
        const yWrap = WebGLSystem.WRAP_MODES.get(asset.yWrap);
        if (yWrap === undefined) {
            console.warn(`Unsupported y wrapping mode: ${asset.xWrap}`);
            return;
        }
        const magFilter = WebGLSystem.FILTER_MODES.get(asset.magFilter);
        if (magFilter === undefined) {
            console.warn(`Unsupported mag filtering mode: ${asset.magFilter}`);
            return;
        }
        const minFilter = WebGLSystem.FILTER_MODES.get(asset.minFilter);
        if (minFilter === undefined) {
            console.warn(`Unsupported min filtering mode: ${asset.minFilter}`);
            return;
        }

        const gl = this.gl;
        const level = 0;
        const internalFormat = gl.RGBA;
        const srcFormat = gl.RGBA;
        const srcType = gl.UNSIGNED_BYTE;
        const border = 0;
        const glTexture = gl.createTexture();

        gl.bindTexture(gl.TEXTURE_2D, glTexture);
        if (!asset.mirror) {
            gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
        }
        if (asset.image instanceof HTMLImageElement) {
            gl.texImage2D(gl.TEXTURE_2D, level, internalFormat, srcFormat, srcType, asset.image);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, xWrap);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, yWrap);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, minFilter);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, magFilter);
            if (asset.generateMipmaps) {
                gl.generateMipmap(gl.TEXTURE_2D);
            }
        } else {
            gl.texImage2D(
                gl.TEXTURE_2D,
                level,
                internalFormat,
                asset.image.width,
                asset.image.height,
                border,
                srcFormat,
                srcType,
                asset.image
            );
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, xWrap);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, yWrap);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, minFilter);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, magFilter);
            if (asset.generateMipmaps) {
                gl.generateMipmap(gl.TEXTURE_2D);
            }
        }
        gl.bindTexture(gl.TEXTURE_2D, null);
        if (glTexture === null) {
            throw `Failed to create texture for image asset '${asset.name}'`;
        }
        this.textures.set(asset.name, glTexture);
    }

    private render(alpha: number): void {
        try {
            const gl = this.gl;

            const canvasWidth = gl.canvas.width;
            const canvasHeight = gl.canvas.height;

            for (let i = 0; i < this.entities.length; i++) {
                const cameraEntity = this.entities[i];

                const camera = cameraEntity.Get(Camera.KEY) as Camera;
                const transform = cameraEntity.Get(Transform.KEY) as Transform;

                // realWidth and realHeight are the width and height of the viewport
                // relative to the canvas with and height, rather than the normalised
                // scale of viewportScale
                const realWidth = canvasWidth * camera.viewportScale.x;
                const realHeight = canvasHeight * camera.viewportScale.y;

                // realPosition is the center position of the camera viewport in relation to
                // the canvas converted from the -1 to +1 coordinates of the viewportPosition
                // combined with the real width and height to make sure it is in the center
                // of the viewport.
                const realPositionX = canvasWidth / 2 + (camera.viewportPosition.x / 2) * canvasWidth - realWidth / 2;
                const realPositionY =
                    canvasHeight / 2 + (camera.viewportPosition.y / 2) * canvasHeight - realHeight / 2;

                // Define the viewport position of the camera
                gl.viewport(realPositionX, realPositionY, realWidth, realHeight);

                // Define scissor around camera viewport, ensures that nothing is rendered outside
                // of the viewport defined for this camera
                gl.scissor(realPositionX, realPositionY, realWidth, realHeight);

                gl.enable(gl.SCISSOR_TEST);

                // Clear the screen
                gl.clearDepth(1.0);
                gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

                // Set the background color
                gl.clearColor(
                    camera.backgroundColor.red,
                    camera.backgroundColor.green,
                    camera.backgroundColor.blue,
                    camera.backgroundColor.alpha
                );

                // Enable depth testing (objects can appear behind/infront of eachother)
                gl.enable(gl.DEPTH_TEST);
                gl.depthFunc(gl.LEQUAL);

                // Enable alpha blending
                gl.enable(gl.BLEND);
                gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

                for (let j = 0; j < this.renderables.length; j++) {
                    const renderable = this.renderables[j];
                    if (renderable.camera.id === cameraEntity.entity.id) {
                        this.cameraRenderables.push(this.renderables[j]);
                    }
                }

                if (this.cameraRenderables.length === 0) {
                    // No renderables prepared for camera
                    continue;
                }

                // Group by z order
                for (let j = 0; j < this.cameraRenderables.length; j++) {
                    const renderable = this.cameraRenderables[j];
                    const zOrderGroup = this.zOrderGroups.get(renderable.zOrder);
                    if (zOrderGroup === undefined) {
                        this.zOrderGroups.set(renderable.zOrder, [renderable]);
                    } else {
                        zOrderGroup.push(renderable);
                    }
                }
                for (const zOrderGroupKeyValue of [...this.zOrderGroups].sort()) {
                    const zOrderGroup = zOrderGroupKeyValue[1];
                    // Group by program
                    for (let j = 0; j < zOrderGroup.length; j++) {
                        const renderable = zOrderGroup[j];
                        let key = "";
                        for (const shaderName of renderable.material.shaders) {
                            key = `${key}_${shaderName}`;
                        }
                        const programRenderables = this.programGroups.get(key);
                        if (programRenderables === undefined) {
                            this.programGroups.set(key, [renderable]);
                        } else {
                            programRenderables.push(renderable);
                            this.programGroups.set(key, programRenderables);
                        }
                    }

                    for (const programGroup of this.programGroups.values()) {
                        const programCanary = programGroup[0];
                        const shaders: [WebGLShader, GLSLShader][] = [];
                        let programKey = "";
                        let loaded = true;

                        for (const shaderName of programCanary.material.shaders) {
                            const shader = this.shaders.get(shaderName);
                            if (shader === undefined) {
                                // Shader not loaded
                                loaded = false;
                                break;
                            }
                            shaders.push(shader);
                            programKey = `${programKey}_${shaderName}`;
                        }

                        if (!loaded) {
                            // Shader not loaded skip renderables for
                            // this program
                            continue;
                        }

                        // Set program variables
                        let program = this.programs.get(programKey);
                        // Program not created yet
                        if (program === undefined) {
                            const loadProgram = gl.createProgram();
                            if (loadProgram === null) {
                                throw `Error creating program '${programKey}'`;
                            }
                            for (const shader of shaders) {
                                gl.attachShader(loadProgram, shader[0]);
                            }
                            gl.linkProgram(loadProgram);
                            if (!gl.getProgramParameter(loadProgram, gl.LINK_STATUS)) {
                                const linkErr = gl.getProgramInfoLog(loadProgram);
                                gl.deleteProgram(loadProgram);
                                throw `Error linking program '${programKey}', error: ${linkErr}`;
                            }
                            this.programs.set(programKey, loadProgram);
                            program = loadProgram;
                        }

                        gl.useProgram(program);
                        // Per shader
                        const glslContext = new GLSLContext(gl, program, camera, transform);
                        for (const shader of shaders) {
                            if (shader[1].perShader !== undefined) {
                                shader[1].perShader(glslContext);
                            }
                        }

                        // Group by texture
                        for (let k = 0; k < programGroup.length; k++) {
                            const renderable = programGroup[k];
                            let textureName = `no_texture`;
                            if (renderable.material.texture !== undefined) {
                                textureName = `texture_${renderable.material.texture.image}`;
                            }
                            const textureRenderables = this.textureGroups.get(textureName);
                            if (textureRenderables === undefined) {
                                this.textureGroups.set(textureName, [renderable]);
                            } else {
                                textureRenderables.push(renderable);
                                this.textureGroups.set(textureName, textureRenderables);
                            }
                        }

                        for (const textureGroup of this.textureGroups.values()) {
                            const textureCanary = textureGroup[0];
                            let texture: WebGLTexture | undefined = undefined;
                            if (textureCanary.material.texture !== undefined) {
                                texture = this.textures.get(textureCanary.material.texture.image);
                                if (texture === undefined) {
                                    // Texture not loaded, skip rendering for this texture
                                    continue;
                                }

                                for (const shader of shaders) {
                                    if (shader[1].perTexture !== undefined) {
                                        shader[1].perTexture(glslContext, texture);
                                    }
                                }
                            }

                            for (const renderable of textureGroup) {
                                const drawMode = WebGLSystem.DRAW_MODES.get(renderable.drawMode);
                                if (drawMode === undefined) {
                                    console.warn(`Unsupported draw mode ${renderable.drawMode}`);
                                    continue;
                                }

                                for (const shader of shaders) {
                                    if (shader[1].perRenderable !== undefined) {
                                        shader[1].perRenderable(glslContext, renderable, texture);
                                    }
                                }

                                gl.drawArrays(drawMode, 0, renderable.vertices.GetFloat32Array().length / 2);
                            }
                        }
                        this.textureGroups.clear();
                    }
                    this.programGroups.clear();
                }
                this.zOrderGroups.clear();
                this.cameraRenderables.length = 0;
            }
        } finally {
            this.textureGroups.clear();
            this.programGroups.clear();
            this.zOrderGroups.clear();
            this.cameraRenderables.length = 0;
            this.renderables.length = 0;
        }
    }
}

export default WebGLSystem;
