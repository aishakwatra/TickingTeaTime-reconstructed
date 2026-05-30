#pragma once

#include "AnimateGameObject.h"
#include "../Engine.h"
#include <glm/glm.hpp>
#include "../Application.h"
#include "../Scene/Scene.h"
#include "../Input/Input.h"
#include <iostream>
#include "GameObject.h"
#include "../Animation/Animator.h"
#include "../Audio/AudioManager.h"
#include "../Scene/Hallway.h"
#include "../GameObjects/Book.h"


class Player : public AnimateGameObject
{
    bool canMove = true;
    Book *m_journal;

    enum class Direction
    {
        Left,
        Right,
        None
    };
    Direction lastDirection = Direction::None;

public:
    Player(const std::string &name, const std::string &texturePath, int p_row, int p_col, Book *journal, float speed)
        : AnimateGameObject(name, texturePath, p_row, p_col), m_journal(journal), speed(speed),
          audioManager(AudioManager::GetInstance())
    {
        m_scale = glm::vec3(6.1f, 6.1f, 0.0f);
        m_position = glm::vec3(-5.0f, -0.8f, 0.0f);
        m_animator.AddAnimation("walk_left", 4, 8, 8.0f, Animator::LoopType::Loop, []() {});
        m_animator.AddAnimation("walk_right", 3, 8, 8.0f, Animator::LoopType::Loop, []() {});
        m_animator.AddAnimation("idle_left", 2, 1, 8.0f, Animator::LoopType::Loop, []() {});
        m_animator.AddAnimation("idle_right", 1, 1, 8.0f, Animator::LoopType::Loop, []() {});
        m_animator.SetAnimation("idle_left");
        audioManager.LoadSound("Player_footsteps", "Assets/Sounds/footstep.mp3", SFX, 0.5f);
    }

    void StopMovement()
    {
        canMove = false;
        audioManager.StopSound("Player_footsteps");
    }

    void ResumeMovement()
    {
        canMove = true;
    }

    void LoadPosition()
    {
        // Call the system to grab the position from the XML.
        // If it fails or the file doesn't exist, it safely returns the default -5.0f!
        float savedX = JournalSaveSystem::GetInstance().GetSavedPlayerX();

        m_position.x = savedX;
    }

    void Update(float dt, long frame) override
    {
        Input &input = Application::GetInput();
        AnimateGameObject::Update(dt, frame);

        // Initialize to a default or idle animation
        std::string currentAnimation = "idle";
        bool isWalking = false;

        const float leftBound = -20.52f;
        const float rightBound = 21.0f;

        if (input.Get().GetKey(GLFW_KEY_A) && canMove)
        {
            isWalking = true;
            float newPos = m_position.x - speed * dt;
            m_position.x = std::max(newPos, leftBound); // Ensure player doesn't move past left bound
            currentAnimation = "walk_left";
            lastDirection = Direction::Left;
        }
        if (input.Get().GetKey(GLFW_KEY_D) && canMove)
        {
            isWalking = true;
            float newPos = m_position.x + speed * dt;
            m_position.x = std::min(newPos, rightBound); // Ensure player doesn't move past right bound
            currentAnimation = "walk_right";
            lastDirection = Direction::Right;
        }


        if (input.Get().GetKeyDown(GLFW_KEY_J))
        {
            Application::Get().SetScene("JournalEntry");
        }

        // If not walking, set idle animation based on last direction
        if (!isWalking)
        {
            if (lastDirection == Direction::Left)
            {
                currentAnimation = "idle_left";
            }
            else if (lastDirection == Direction::Right)
            {
                currentAnimation = "idle_right";
            }
        }

        // Set the animation
        m_animator.SetAnimation(currentAnimation);

        // Then update the animator
        m_animator.Update(dt);

        // Now get the current frame and row
        int currentFrame = m_animator.GetCurrentFrame();
        int currentRow = m_animator.GetCurrentRow();

        // Use the current frame and row for your sprite sheet
        animY = static_cast<float>(currentRow);
        animX = static_cast<float>(currentFrame);

        if (isWalking && Application::Get().GetCurrentSceneName() == ("Hallway"))
        {
            if (!audioManager.IsSoundPlaying("Player_footsteps"))
            {
                audioManager.PlaySound("Player_footsteps", true);
            }
        }
        else
        {
            if (audioManager.IsSoundPlaying("Player_footsteps"))
            {
                audioManager.StopSound("Player_footsteps");
            }
        }

        glm::vec2 mosePos = Application::Get().MousetoWorld();
    }



private:
    float speed = 6.0f;
    glm::vec2 mousePos;
    Animator m_animator;
    AudioManager &audioManager;
};