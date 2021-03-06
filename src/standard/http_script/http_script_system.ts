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
import ScriptAsset from "../../scripting/script_asset";
import ScriptRequest from "../../scripting/script_request";
import MapSystem from "../../system/map_system";

/**
 * HTTPScriptSystem handles loading script assets over HTTP and making them
 * available to the engine for execution.
 */
class HTTPScriptSystem extends MapSystem {
    /**
     * Message to send out all loaded script messages that are currently loaded.
     */
    public static readonly MESSAGE_REQUEST_FLUSH = "request_script_flush";
    /**
     * Message to clear out loaded assets.
     */
    public static readonly MESSAGE_REQUEST_CLEAR = "request_script_clear";

    private static readonly DEFAULT_ROOT_PATH = "/";

    private rootPath: string;
    private assets: ScriptAsset[];

    constructor(
        messageBus: IMessageBus,
        scene?: IScene,
        entities?: Map<number, SystemEntity>,
        subscriberID?: number,
        assets: ScriptAsset[] = []
    ) {
        super(messageBus, scene, undefined, entities, subscriberID);
        this.assets = assets;
        this.messageBus.Subscribe(this, [
            ScriptRequest.MESSAGE_REQUEST_LOAD,
            HTTPScriptSystem.MESSAGE_REQUEST_FLUSH,
            HTTPScriptSystem.MESSAGE_REQUEST_CLEAR,
        ]);

        if (window.JamJar && window.JamJar.RootPath) {
            this.rootPath = window.JamJar.RootPath;
        } else {
            this.rootPath = HTTPScriptSystem.DEFAULT_ROOT_PATH;
        }
    }

    public OnMessage(message: IMessage): void {
        super.OnMessage(message);
        switch (message.type) {
            case ScriptRequest.MESSAGE_REQUEST_LOAD: {
                const loadMessage = message as Message<ScriptRequest>;
                if (loadMessage.payload === undefined) {
                    return;
                }
                this.load(loadMessage.payload);
                break;
            }
            case HTTPScriptSystem.MESSAGE_REQUEST_FLUSH: {
                this.flush();
                break;
            }
            case HTTPScriptSystem.MESSAGE_REQUEST_CLEAR: {
                this.clear();
                break;
            }
        }
    }

    protected httpSuccess(code: string, request: ScriptRequest): void {
        const asset = new ScriptAsset(request.name, code);
        this.messageBus.Publish(Message.New<ScriptAsset>(ScriptAsset.MESSAGE_FINISH_LOAD, asset));
        this.assets.push(asset);
    }

    protected httpError(request: ScriptRequest, error: Error): void {
        const asset = new ScriptAsset(request.name, "", error);
        this.messageBus.Publish(Message.New<ScriptAsset>(ScriptAsset.MESSAGE_FINISH_LOAD, asset));
        this.assets.push(asset);
    }

    protected handleResponse(response: Response): Promise<string> {
        if (!response.ok) {
            // Non 404 response code
            throw Error(response.statusText);
        }
        return response.text();
    }

    private load(request: ScriptRequest): void {
        let source = request.source;
        if (source.indexOf("http://") !== 0 && source.indexOf("https://") !== 0) {
            // If relative, prepend with root path
            source = `${this.rootPath}${source}`;
        }
        // HTTP fetch request to source
        fetch(source)
            .then(this.handleResponse)
            .then((buffer) => this.httpSuccess(buffer, request))
            .catch((error) => this.httpError(request, error));
    }

    private flush(): void {
        for (let i = 0; i < this.assets.length; i++) {
            this.messageBus.Publish(Message.New<ScriptAsset>(ScriptAsset.MESSAGE_FINISH_LOAD, this.assets[i]));
        }
    }

    private clear(): void {
        this.assets = [];
    }
}

export default HTTPScriptSystem;
