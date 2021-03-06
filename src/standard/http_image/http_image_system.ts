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

import IMessageBus from "../../message/imessage_bus";
import IScene from "../../scene/iscene";
import SystemEntity from "../../system/system_entity";
import IMessage from "../../message/imessage";
import Message from "../../message/message";
import ImageAsset from "../../rendering/image/image_asset";
import ImageRequest from "../../rendering/image/image_request";
import MapSystem from "../../system/map_system";

/**
 * HTTPImageSystem handles loading image assets over HTTP and making them
 * available to the engine for rendering.
 */
class HTTPImageSystem extends MapSystem {
    public static readonly MESSAGE_REQUEST_FLUSH = "request_image_flush";
    public static readonly MESSAGE_REQUEST_CLEAR = "request_image_clear";

    private static readonly DEFAULT_ROOT_PATH = "/";

    private images: ImageAsset[];
    private rootPath: string;

    constructor(
        messageBus: IMessageBus,
        scene?: IScene,
        entities?: Map<number, SystemEntity>,
        subscriberID?: number,
        images: ImageAsset[] = []
    ) {
        super(messageBus, scene, undefined, entities, subscriberID);
        this.images = images;
        this.messageBus.Subscribe(this, [
            ImageRequest.MESSAGE_REQUEST_LOAD,
            HTTPImageSystem.MESSAGE_REQUEST_FLUSH,
            HTTPImageSystem.MESSAGE_REQUEST_CLEAR,
        ]);

        if (window.JamJar && window.JamJar.RootPath) {
            this.rootPath = window.JamJar.RootPath;
        } else {
            this.rootPath = HTTPImageSystem.DEFAULT_ROOT_PATH;
        }
    }

    public OnMessage(message: IMessage): void {
        super.OnMessage(message);
        switch (message.type) {
            case ImageRequest.MESSAGE_REQUEST_LOAD: {
                const loadMessage = message as Message<ImageRequest>;
                if (loadMessage.payload === undefined) {
                    return;
                }
                this.load(loadMessage.payload);
                break;
            }
            case HTTPImageSystem.MESSAGE_REQUEST_FLUSH: {
                this.flush();
                break;
            }
            case HTTPImageSystem.MESSAGE_REQUEST_CLEAR: {
                this.clear();
                break;
            }
        }
    }

    protected onLoad(event: Event | undefined, image: HTMLImageElement, request: ImageRequest): void {
        const asset = new ImageAsset(
            request.name,
            image,
            true,
            request.xWrap,
            request.yWrap,
            request.magFilter,
            request.minFilter,
            request.generateMipmaps,
            request.mirror
        );
        this.messageBus.Publish(Message.New<ImageAsset>(ImageAsset.MESSAGE_FINISH_LOAD, asset));
        this.images.push(asset);
    }

    protected onError(event: Event | undefined, image: HTMLImageElement, request: ImageRequest): void {
        const asset = new ImageAsset(
            request.name,
            image,
            false,
            request.xWrap,
            request.yWrap,
            request.magFilter,
            request.minFilter,
            request.generateMipmaps,
            request.mirror,
            new Error(`Failed to load image ${request.name} from source ${request.source}`)
        );
        this.messageBus.Publish(Message.New<ImageAsset>(ImageAsset.MESSAGE_FINISH_LOAD, asset));
        this.images.push(asset);
    }

    private load(request: ImageRequest): void {
        const img = new Image();
        img.crossOrigin = "*";
        img.addEventListener("error", this.onError.bind(this, event, img, request));
        img.addEventListener("load", this.onLoad.bind(this, event, img, request));
        if (request.source.indexOf("http://") === 0 || request.source.indexOf("https://") === 0) {
            // Handle absolute URLs
            img.src = request.source;
        } else {
            // If relative, prepend with root path
            img.src = `${this.rootPath}${request.source}`;
        }
    }

    private flush(): void {
        for (let i = 0; i < this.images.length; i++) {
            this.messageBus.Publish(Message.New<ImageAsset>(ImageAsset.MESSAGE_FINISH_LOAD, this.images[i]));
        }
    }

    private clear(): void {
        this.images.length = 0;
    }
}

export default HTTPImageSystem;
