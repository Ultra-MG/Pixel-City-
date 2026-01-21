#include "engine/SceneManager.hpp"
#include "engine/Scene.hpp"
#include <stdexcept>

SceneManager::~SceneManager() = default;

void SceneManager::set(std::unique_ptr<Scene> scene) {
  m_scene = std::move(scene);
}

Scene& SceneManager::current() {
  if (!m_scene) throw std::runtime_error("No scene set.");
  return *m_scene;
}
