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

import IMessageBus from "../../message/imessage_bus";
import IScene from "../../scene/iscene";
import SystemEntity from "../../system/system_entity";
import Evaluator from "../../system/evaluator";
import IMessage from "../../message/imessage";
import Message from "../../message/message";
import IRenderable from "../../rendering/irenderable";
import ArraySystem from "../../system/array_system";

/**
 * RenderSystem is an abstract class representing a generic rendering system,
 * has logic for handling loading renderables.
 * Contains the message constant for loading renderables.
 */
abstract class RenderSystem extends ArraySystem {
    /**
     * Message used to add new renderables into the render system's render list.
     */
    public static readonly MESSAGE_LOAD_RENDERABLES = "load_renderables";

    /**
     * A list of things to be rendered.
     */
    protected renderables: IRenderable[];

    constructor(
        messageBus: IMessageBus,
        scene?: IScene,
        evaluator?: Evaluator,
        renderables: IRenderable[] = [],
        entities?: SystemEntity[],
        subscriberID?: number
    ) {
        super(messageBus, scene, evaluator, entities, subscriberID);
        this.renderables = renderables;
    }

    public OnMessage(message: IMessage): void {
        super.OnMessage(message);
        switch (message.type) {
            case RenderSystem.MESSAGE_LOAD_RENDERABLES: {
                const renderMessage = message as Message<IRenderable[]>;
                if (renderMessage.payload === undefined) {
                    return;
                }
                this.renderables.push(...renderMessage.payload);
                break;
            }
        }
    }
}

export default RenderSystem;
