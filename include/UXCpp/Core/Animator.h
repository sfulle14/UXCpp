/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#pragma once

#include <UXCpp/Core/Animation.h>
#include <UXCpp/Core/Property.h>
#include <vector>
#include <memory>
#include <functional>

namespace uxcpp::core {

/**
 * @brief Manages active animations and updates them every frame.
 */
class Animator {
public:
    static Animator& getInstance() {
        static Animator instance;
        return instance;
    }

    /**
     * @brief Animates a property from start to end value.
     */
    template <typename T>
    void animateProperty(Property<T>* prop, T start, T end, float duration, Easing easing = Easing::Linear) {
        auto anim = std::make_shared<PropertyAnimation<T>>(start, end, duration, easing);
        
        // Create a wrapper to update the property
        auto updateFunc = [prop, anim](float dt) {
            anim->update(dt);
            prop->set(anim->getValue());
        };

        m_activeAnimations.push_back(updateFunc);
    }

    void update(float deltaTime) {
        m_activeAnimations.erase(
            std::remove_if(m_activeAnimations.begin(), m_activeAnimations.end(),
                [deltaTime](auto& animFunc) {
                    animFunc(deltaTime);
                    // Note: In a full implementation, we'd track the PropertyAnimation object 
                    // to know when it's finished. For this simplified version, 
                    // we rely on the property value reaching its target or a timer.
                    return false; // Simplified: animations persist until manually cleared or logic added
                }),
            m_activeAnimations.end()
        );
    }

    void clear() { m_activeAnimations.clear(); }

private:
    Animator() = default;
    std::vector<std::function<void(float)>> m_activeAnimations;
};

} // namespace uxcpp::core
