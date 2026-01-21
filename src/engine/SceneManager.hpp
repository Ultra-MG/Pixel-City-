#pragma once
#include <memory>

class Scene;

class SceneManager {
public:
  SceneManager() = default;
  ~SceneManager();

  void set(std::unique_ptr<Scene> scene);
  Scene& current();

private:
  std::unique_ptr<Scene> m_scene;
};
