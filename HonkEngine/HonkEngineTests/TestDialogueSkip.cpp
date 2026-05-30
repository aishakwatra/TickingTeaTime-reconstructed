// TestDialogueSkip.cpp
// Tests for dialogue fast forward logic.
//
// The fast forward feature allows the player to hold Left Ctrl
// to advance dialogue automatically, stopping at choice prompts.
//
// Invariants:
//   - Fast forward never fires during a choice dialogue
//   - Fast forward never fires when pause menu is visible
//   - Fast forward only fires when Left Ctrl is held
//
// Normal cases:  fast forward advances during normal dialogue
// Edge cases:    fast forward stops at choices, resumes after
// Failure cases: fast forward blocked when paused or Ctrl not held

#include "catch2/catch_amalgamated.hpp"

/// @brief Mirrors the fast forward eligibility logic from RoomScene.
/// @param ctrlHeld        True if Left Ctrl is currently held.
/// @param isPaused        True if pause menu is visible.
/// @param isChoiceDialogue True if current dialogue is a choice prompt.
/// @pre   none
/// @post  returns true only when all three conditions allow fast forward
static bool ShouldFastForward(bool ctrlHeld, bool isPaused, bool isChoiceDialogue)
{
    if (!ctrlHeld)
        return false;
    if (isPaused)
        return false;
    if (isChoiceDialogue)
        return false;
    return true;
}

/// @brief Mirrors the fast forward timer logic from RoomScene.
/// @param currentTimer    Current countdown timer value in seconds.
/// @param dt              Delta time in seconds.
/// @param interval        How often to advance dialogue in seconds.
/// @pre   dt >= 0, interval > 0
/// @post  returns true when timer has expired and dialogue should advance
static bool ShouldAdvanceThisFrame(float currentTimer, float dt, float interval)
{
    return (currentTimer - dt) <= 0.0f;
}

/// @brief Mirrors the timer reset logic from RoomScene.
/// @param currentTimer    Current countdown timer value.
/// @param dt              Delta time in seconds.
/// @param interval        Reset value when timer expires.
/// @pre   dt >= 0, interval > 0
/// @post  returns reset interval if expired, otherwise decremented timer
static float UpdateTimer(float currentTimer, float dt, float interval)
{
    float updated = currentTimer - dt;
    if (updated <= 0.0f)
        return interval;
    return updated;
}

// =============================================================
// NORMAL CASES
// =============================================================

TEST_CASE("FastForward - fires when Ctrl held during normal dialogue", "[DialogueSkip][normal]")
{
    REQUIRE(ShouldFastForward(true, false, false) == true);
}

TEST_CASE("FastForward - does not fire when Ctrl not held", "[DialogueSkip][normal]")
{
    REQUIRE(ShouldFastForward(false, false, false) == false);
}

TEST_CASE("FastForward - does not fire during choice dialogue", "[DialogueSkip][normal]")
{
    REQUIRE(ShouldFastForward(true, false, true) == false);
}

TEST_CASE("FastForward - does not fire when paused", "[DialogueSkip][normal]")
{
    REQUIRE(ShouldFastForward(true, true, false) == false);
}

TEST_CASE("FastForward - timer advances dialogue when expired", "[DialogueSkip][normal]")
{
    // Timer at 0.05s, dt = 0.05s -> should advance
    REQUIRE(ShouldAdvanceThisFrame(0.05f, 0.05f, 0.05f) == true);
}

TEST_CASE("FastForward - timer does not advance before expiry", "[DialogueSkip][normal]")
{
    // Timer at 0.05s, dt = 0.02s -> should not advance yet
    REQUIRE(ShouldAdvanceThisFrame(0.05f, 0.02f, 0.05f) == false);
}

TEST_CASE("FastForward - timer resets after expiry", "[DialogueSkip][normal]")
{
    // Timer at 0.05s, dt = 0.05s -> resets to interval
    float result = UpdateTimer(0.05f, 0.05f, 0.05f);
    REQUIRE(result == 0.05f);
}

TEST_CASE("FastForward - timer decrements correctly", "[DialogueSkip][normal]")
{
    // Timer at 0.05s, dt = 0.02s -> decrements to 0.03s
    float result = UpdateTimer(0.05f, 0.02f, 0.05f);
    REQUIRE(result == Catch::Approx(0.03f));
}

// =============================================================
// EDGE CASES
// =============================================================

TEST_CASE("FastForward - stops exactly at choice dialogue", "[DialogueSkip][edge]")
{
    // Normal dialogue — should fast forward
    REQUIRE(ShouldFastForward(true, false, false) == true);

    // Hit a choice — should stop
    REQUIRE(ShouldFastForward(true, false, true) == false);

    // Choice resolved, back to normal — should resume
    REQUIRE(ShouldFastForward(true, false, false) == true);
}

TEST_CASE("FastForward - paused and choice both block simultaneously", "[DialogueSkip][edge]")
{
    REQUIRE(ShouldFastForward(true, true, true) == false);
}

TEST_CASE("FastForward - releasing Ctrl mid dialogue stops fast forward", "[DialogueSkip][edge]")
{
    // Ctrl held
    REQUIRE(ShouldFastForward(true, false, false) == true);

    // Ctrl released
    REQUIRE(ShouldFastForward(false, false, false) == false);
}

TEST_CASE("FastForward - timer advance on exact expiry boundary", "[DialogueSkip][edge]")
{
    // Timer exactly equals dt — should advance
    REQUIRE(ShouldAdvanceThisFrame(0.016f, 0.016f, 0.05f) == true);
}

TEST_CASE("FastForward - timer resets to full interval after advance", "[DialogueSkip][edge]")
{
    float interval = 0.05f;
    float result = UpdateTimer(0.05f, 0.05f, interval);
    REQUIRE(result == Catch::Approx(interval));
}

TEST_CASE("FastForward - very small dt does not trigger advance", "[DialogueSkip][edge]")
{
    // dt almost zero — timer should not expire
    REQUIRE(ShouldAdvanceThisFrame(0.05f, 0.001f, 0.05f) == false);
}

TEST_CASE("FastForward - large dt triggers advance", "[DialogueSkip][edge]")
{
    // dt larger than timer — should advance
    REQUIRE(ShouldAdvanceThisFrame(0.05f, 0.1f, 0.05f) == true);
}

// =============================================================
// FAILURE CASES
// =============================================================

TEST_CASE("FastForward - all conditions blocking returns false", "[DialogueSkip][failure]")
{
    REQUIRE(ShouldFastForward(false, true, true) == false);
}

TEST_CASE("FastForward - Ctrl held but paused does not fast forward", "[DialogueSkip][failure]")
{
    REQUIRE(ShouldFastForward(true, true, false) == false);
}

TEST_CASE("FastForward - Ctrl held but choice blocks regardless of pause", "[DialogueSkip][failure]")
{
    REQUIRE(ShouldFastForward(true, false, true) == false);
    REQUIRE(ShouldFastForward(true, true, true) == false);
}

TEST_CASE("FastForward - timer never goes negative after update", "[DialogueSkip][failure]")
{
    // Even with large dt timer resets to interval not negative
    float result = UpdateTimer(0.05f, 10.0f, 0.05f);
    REQUIRE(result > 0.0f);
}

TEST_CASE("FastForward - zero dt does not advance timer", "[DialogueSkip][failure]")
{
    float result = UpdateTimer(0.05f, 0.0f, 0.05f);
    REQUIRE(result == Catch::Approx(0.05f));
}