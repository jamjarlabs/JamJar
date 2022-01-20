#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/primitive/primitive_system.hpp"
#include <utility>

JamJar::Standard::_2D::Primitive::Primitive(Polygon polygon, Material material, DrawMode drawMode)
    : Component(KEY), polygon(std::move(polygon)), material(std::move(material)), drawMode(drawMode) {
    if (!this->material.shaders.has_value()) {
        this->material.shaders.emplace(JamJar::ShaderGroup({
            .vertexShader = PrimitiveSystem::DEFAULT_PRIMITIVE_VERTEX_SHADER_NAME,
            .fragmentShader = PrimitiveSystem::DEFAULT_PRIMITIVE_FRAGMENT_SHADER_NAME,
        }));
    }
}

JamJar::Standard::_2D::Primitive::Primitive(Polygon polygon, Material material)
    : Primitive(std::move(polygon), std::move(material), DrawMode::LINE_STRIP) {}
