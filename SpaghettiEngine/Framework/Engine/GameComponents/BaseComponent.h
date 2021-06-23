#pragma once
#include "Scene.h"
#include <list>

CLASS_FORWARD_DECLARATION(BaseComponent);

class BaseComponent
{
    friend class Scene;
public:
    // Used to convert local id to public id.
    enum class Type
    {
        gameObj = 1,
        script = 2,
        invalid = 3
    };
public:
    BaseComponent(PScene owner, bool isDisabled = false);

    BaseComponent(const BaseComponent&) = delete;
    BaseComponent(const BaseComponent&&) = delete;
    BaseComponent& operator=(const BaseComponent&) = delete;
    BaseComponent& operator=(const BaseComponent&&) = delete;

#pragma region Events
    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnFixedUpdate() {}
    virtual void OnEnd() {}

    virtual void OnDisabled() {}
    virtual void OnEnabled() {}

    virtual void Disable();
    virtual void Enable();

    virtual void OnCollide(CollideEvent& e) {};
    virtual void OnCollideEnter(CollideEvent& e) {};
    virtual void OnCollideExit(CollideEvent& e) {};
#pragma endregion

    virtual bool [[nodiscard]] IsDisabled();
    virtual void Load(nlohmann::json& input) = 0;

    bool CallDestroy();
    virtual Type GetType() = 0;

    /// Get the shared_ptr of the component which is owned by a scene
    [[nodiscard]] std::shared_ptr<BaseComponent> GetSharedPtr() const;
    void AssignPtr(std::list<SBaseComponent>::iterator iterator);

    [[nodiscard]] PScene GetOwner() const;
protected:
    virtual ~BaseComponent() = default;
    virtual void Destroy();
private: // Can only be used by scene
    std::list<SBaseComponent>::iterator GetIterator() const;

    void SetName(const std::string& name);
protected:
    PScene _owner = nullptr;
    bool _isDisabled = false;

    std::string _name;

    // the pointer to it self in the scene container.
    std::list<SBaseComponent>::iterator _iterator;
    std::weak_ptr<BaseComponent> _this;
};