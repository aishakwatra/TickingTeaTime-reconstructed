#pragma once

#include "../GameObjects/GameObject.h"
#include "../Engine.h"
#include "../Input/Input.h"
#include <iostream>
#include "../Application.h"

#include "../UI/UIButton.h"

class Interface : public GameObject
{

public:
    Interface(const std::string &name) : GameObject(name), isVisible(false)
    {
    }

    // Show the interface
    virtual void Show()
    {
        isVisible = true;
        setActiveStatus(true);
    }

    // Hide the interface
    virtual void Hide()
    {
        isVisible = false;
        setActiveStatus(false);
    }

    // Check if the interface is currently visible
    bool IsVisible() const
    {
        return isVisible;
    }

    virtual void Render() override
    {
        if (!isVisible)
            return;

        for (auto &object : m_gameObjects)
        {
            if (object->getActiveStatus())
            {
                object->Render();
            }
        }
    }

    virtual void Update(float dt, long frame) override
    {
        if (!isVisible)
            return;

        for (auto it = m_gameObjects.rbegin(); it != m_gameObjects.rend(); ++it)
        {
            GameObject *object = *it;
            if (object->getActiveStatus())
            { // CHECK ACTIVE STATUS
                object->Update(dt, frame);
            }
        }
    }



protected:
    bool isVisible;

    std::vector<GameObject *> m_gameObjects;

    virtual void setActiveStatus(bool status) override
    {
        for (auto &object : m_gameObjects)
        {
            object->setActiveStatus(status);
        }
    }
};
