# Component Manager

The component manager is a central store of components of a particular type. The component manager has ownership of the
components, meaning it handles the memory for the components and is responsible for freeing the memory when the
component is deleted/removed. The component manager keeps track of the entity that the component is assigned to
alongside the storage of the component itself.

Component managers work in conjunction with the entity manager, the entity manager handles creation and management
of the component managers, forwarding components to the relevant component manager as needed.

See the [Entity Manager] for more details.

[Entity Manager]:./entity_manager.md
