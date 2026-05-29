#pragma once
#include <string>

class JournalSaveSystem
{
public:
    // Get the singleton instance
    static JournalSaveSystem &GetInstance()
    {
        static JournalSaveSystem instance;
        return instance;
    }

    // Single call to save all journal details and player positions to default file
    bool SaveJournalData(const std::string &filepath = "savegame.xml");

    // Reads back the tag hierarchy and restores state
    bool LoadJournalData(const std::string &filepath = "savegame.xml");

    float GetSavedPlayerX(const std::string &filepath = "savegame.xml");

private:
    // Private constructor/destructor for Singleton design pattern
    JournalSaveSystem() = default;
    ~JournalSaveSystem() = default;
    JournalSaveSystem(const JournalSaveSystem &) = delete;
    JournalSaveSystem &operator=(const JournalSaveSystem &) = delete;
};
#pragma once
