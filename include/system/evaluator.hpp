#ifndef SYSTEM_EVALUATOR_HPP
#define SYSTEM_EVALUATOR_HPP

#include "entity/entity.hpp"
#include <functional>

namespace JamJar {
using Evaluator = std::function<bool(Entity *, const std::vector<JamJar::Component *> &)>;
}; // namespace JamJar

#endif
