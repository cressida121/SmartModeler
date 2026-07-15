#ifndef SCENENODE_H
#define SCENENODE_H

#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class SceneNode {
public:
	SceneNode(SceneNode *parent = nullptr, std::string name = "") : m_parent(parent), m_name(name) {}

	// Rule of Five - deleted to prevent copying/moving scene hierarchy

	virtual ~SceneNode() = default;
	SceneNode(const SceneNode &) = delete;
	SceneNode &operator=(const SceneNode &) = delete;
	SceneNode(SceneNode &&) = delete;
	SceneNode &operator=(SceneNode &&) = delete;

	void SetParent(SceneNode *parent) { m_parent = parent; }
	SceneNode *GetParent() const { return m_parent; };
	std::vector<SceneNode *> GetChildren() const { return m_children; };
	virtual glm::mat4 GetTransform() const = 0;
	std::string GetName() const { return m_name; }

protected:
	SceneNode *m_parent;
	std::vector<SceneNode *> m_children;
	std::string m_name;
};

#endif // SCENENODE_H