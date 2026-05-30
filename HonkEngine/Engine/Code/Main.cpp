#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080
#define NAME "Ticking Tea Time"

#include <windows.h>
#include <tchar.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Shader.h"
#include "Application.h"

#include "Scene/Hallway.h"
#include "Scene/Room1.h"
#include "Scene/Room2.h"
#include "Scene/Room3.h"
#include "Scene/Room4.h"
#include "Scene/Kitchen.h"
#include "Scene/LoadScene.h"
#include "Scene/MainMenu.h"
#include "Scene/JournalEntry.h"
#include "Scene/EndScene.h"
#include "Scene/OpenScene.h"
#include "Scene/EndCredit.h"
#include "Scene/TutorialScene.h"
#include "Scene/BeforeEndCutscene.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// int main()
{

    HANDLE hMutex = CreateMutex(NULL, TRUE, L"Global\\TickingTeaTime");

    if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(
            NULL, _T("Another instance of the application is already running."), _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    Application game(WIN_WIDTH, WIN_HEIGHT, NAME);

    // Add scene
    game.AddScene("LoadScene", new LoadScene());
    // Set start scene
    game.AddScene("Hallway", new Hallway());
    game.AddScene("Room1", new Room1());
    game.AddScene("Room2", new Room2());
    game.AddScene("Room3", new Room3());
    game.AddScene("Room4", new Room4());
    game.AddScene("Kitchen", new Kitchen());
    game.AddScene("MainMenu", new MainMenu());
    game.AddScene("OpenScene", new OpenScene());
    game.AddScene("JournalEntry", new JournalEntry());
    game.AddScene("EndScene", new EndScene());
    game.AddScene("EndCredit", new EndCredit());
    game.AddScene("TutorialScene", new TutorialScene());
    game.AddScene("BeforeEndCutscene", new BeforeEndCutscene());
    game.SetScene("LoadScene");

    // Run game
    game.Run();

    if (hMutex)
    {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    return 0;
}
