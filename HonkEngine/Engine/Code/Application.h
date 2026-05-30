#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <queue>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer/Shader.h"
#include "Scene/Scene.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"
#include "Audio/AudioManager.h"
#include "SaveSystem/JournalSaveSystem.h"
#include "stb_image.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

struct GlobalTimer
{
    std::function<void()> callback;
    std::chrono::steady_clock::time_point endTime;
    long long duration; // Duration in milliseconds
    bool repeat;

    // Comparator for priority queue (earliest time first)
    bool operator<(const GlobalTimer &other) const
    {
        return endTime > other.endTime;
    }
};

class Application
{
public:
    Application(int width, int height, const char *title);

    ~Application();

    void AddScene(const std::string &name, Scene *scene)
    {
        m_sceneMap.insert({ name, scene });
    }

    void SetScene(const std::string &name)
    {
        if (m_sceneMap.find(name) != m_sceneMap.end())
        {
            if (m_currentScene)
            {
                m_currentScene->OnExit();
            }
            m_currentScene = m_sceneMap[name]; // Assume existence for simplicity
            if (m_currentScene)
            {
                m_currentScene->OnEnter();
            }
        }
    }

    int SetTimer(long long duration, std::function<void()> callback, bool repeat);

    void CancelTimer(int timerId);

    int ResetTimer(int timerId, long long duration, std::function<void()> callback, bool repeat);

    void ClearAllTimers();

    void Run();

    static Application &Get()
    {
        return *s_instance;
    }

    // void DrawTexture(unsigned int textureId, const glm::mat4& transform);


    glm::vec2 CursorPos()
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);

        return { xpos, ypos };
    }


    bool IsMouseClick(int mouseButton)
    {
        return glfwGetMouseButton(m_window, mouseButton) == GLFW_PRESS;
    }

    std::string GetCurrentSceneName() const
    {
        for (const auto &pair : m_sceneMap)
        {
            if (pair.second == m_currentScene)
            {
                return pair.first; // Return the name of the current scene
            }
        }
        return "";
    }

    static glm::vec2 MousetoWorld()
    {
        Camera &camera = Application::GetCamera();
        float zoom = camera.GetZoom() - 0.155f;
        glm::vec2 cameraPos = glm::vec2(camera.GetPosX(), camera.GetPosY());
        glm::vec2 mousePos = Application::Get().CursorPos();
        float xpos = ((mousePos.x - (SCR_WIDTH / 2.0f)) / zoom) * (16.0 / SCR_WIDTH) + cameraPos.x;
        float ypos = ((mousePos.y - (SCR_HEIGHT / 2.0f)) / zoom) * (9.0 / SCR_HEIGHT) * -1 + cameraPos.y;

        return glm::vec2(xpos, ypos);
    }

    void exitGame()
    {
        if (m_window)
        {
            glfwSetWindowShouldClose(m_window, true);
        }
    }

    void SetFullScreen()
    {
        if (isFullscreen)
        {
            return;
        }

        isFullscreen = true;
        ToggleFullscreen(m_window);
    }

    void SetWindowedMode()
    {
        if (!isFullscreen)
        {
            return;
        }

        isFullscreen = false;
        ToggleFullscreen(m_window);
    }


    static Renderer &GetRenderer()
    {
        return s_instance->m_renderer;
    }
    static Input &GetInput()
    {
        return s_instance->m_input;
    }
    static Camera &GetCamera()
    {
        return s_instance->m_camera;
    }

    double GetDeltaTime() const
    {
        return deltaTime;
    }


    void ToggleFullscreen(GLFWwindow *window);

    // CURSOR FUNCTIONS

    GLFWcursor *LoadCursor(const char *imagePath, int xhot, int yhot)
    {
        int width, height, channels;
        unsigned char *data = stbi_load(imagePath, &width, &height, &channels, 4);
        if (!data)
        {
            std::cerr << "Cursor image load failed: " << imagePath << std::endl;
            return nullptr;
        }

        GLFWimage cursorImg;
        cursorImg.width = width;
        cursorImg.height = height;
        cursorImg.pixels = data;

        GLFWcursor *cursor = glfwCreateCursor(&cursorImg, xhot, yhot);
        stbi_image_free(data);
        return cursor;
    }


    void LoadCursors()
    {
        normalCursor = LoadCursor("Assets/Images/normal_cursor.png", 16, 16);
        clickedCursor = LoadCursor("Assets/Images/clicked_cursor.png", 16, 16);
    }

    void SetClickedCursor()
    {
        if (m_isCursorSettingEnabled)
        {
            glfwSetCursor(m_window, clickedCursor);
        }
    }

    void SetNormalCursor()
    {
        if (m_isCursorSettingEnabled)
        {
            glfwSetCursor(m_window, normalCursor);
        }
    }

    void ToggleCursorSetting(bool enabled)
    {
        m_isCursorSettingEnabled = enabled;
        if (m_isCursorSettingEnabled)
        {
            if (normalCursor)
            {
                glfwSetCursor(m_window, normalCursor);
            }
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
    }


private:
    void processTimers();
    std::priority_queue<GlobalTimer> timers;
    std::unordered_map<int, std::unique_ptr<GlobalTimer>> activeTimers;
    int nextTimerId = 0;
    std::string baseTitle;
    Shader m_shader;
    Camera m_camera;
    Renderer m_renderer;
    static Application *s_instance;
    GLFWwindow *m_window;
    Input m_input;
    bool m_isCursorSettingEnabled = false;
    Scene *m_currentScene = nullptr;
    std::map<std::string, Scene *> m_sceneMap;
    double deltaTime = 0.0;

    GLFWcursor *normalCursor = nullptr;
    GLFWcursor *clickedCursor = nullptr;

    bool isFullscreen = true;
};