#include "input_component_impl.hpp"
#include <game_properties.hpp>

InputComponentImpl::InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void InputComponentImpl::updateMovement(InputTargetInterface& target)
{
    auto const force = 150.0f;

    if (m_keyboard->pressed(jt::KeyCode::D) || m_keyboard->pressed(jt::KeyCode::Right)) {
        target.addForceToCenter(jt::Vector2f { force, 0.0f });
    }
    if (m_keyboard->pressed(jt::KeyCode::A) || m_keyboard->pressed(jt::KeyCode::Left)) {
        target.addForceToCenter(jt::Vector2f { -force, 0.0f });
    }

    if (m_keyboard->pressed(jt::KeyCode::W) || m_keyboard->pressed(jt::KeyCode::Up)) {
        target.addForceToCenter(jt::Vector2f { 0.0f, -force });
    }
    if (m_keyboard->pressed(jt::KeyCode::S) || m_keyboard->pressed(jt::KeyCode::Down)) {
        target.addForceToCenter(jt::Vector2f { 0.0f, force });
    }
}
