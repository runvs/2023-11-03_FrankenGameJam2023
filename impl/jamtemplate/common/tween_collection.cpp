#include "tween_collection.hpp"
#include <algorithm>

void jt::TweenCollection::clear()
{
    m_tweens.clear();
    m_tweensToAdd.clear();
}

void jt::TweenCollection::add(std::shared_ptr<jt::TweenInterface> tween)
{
    m_tweensToAdd.push_back(tween);
}

void jt::TweenCollection::update(float elapsed)
{
    m_tweens.insert(m_tweens.end(), m_tweensToAdd.cbegin(), m_tweensToAdd.cend());
    m_tweensToAdd.clear();
    if (m_tweens.empty()) {
        return;
    }
    std::erase_if(
        m_tweens, [](std::shared_ptr<TweenInterface> const& tween) { return !(tween->isAlive()); });

    for (auto& tween : m_tweens) {
        tween->update(elapsed);
    }
}

std::size_t jt::TweenCollection::size() const { return m_tweens.size() + m_tweensToAdd.size(); }
