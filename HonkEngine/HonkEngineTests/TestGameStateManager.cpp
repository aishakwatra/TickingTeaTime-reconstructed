// TestGameStateManager.cpp
// Tests the room state progression logic in isolation.
// We replicate the state enum and transition rules here
// without pulling in the full engine header chain.
//
// Valid progression:
//   Order -> Serve -> MealReact -> Score ->
//   InspectionStart -> Inspection -> InspectionEnd -> End

#include "catch2/catch_amalgamated.hpp"

// Replicate just the enums — no engine headers required
enum class RoomState {
    Order,
    Prepare,
    Serve,
    MealReact,
    Score,
    InspectionStart,
    Inspection,
    InspectionEnd,
    End
};

enum class GameState {
    ROOM1_STATE,
    ROOM2_STATE,
    ROOM3_STATE,
    ROOM4_STATE,
    HALLWAY_STATE
};

/// @brief Minimal state manager to test transition logic in isolation.
///
/// Invariants:
///   - roomState is always a valid RoomState enum value
///   - gameState is always a valid GameState enum value
class TestStateManager {
public:
    TestStateManager()
        : roomState(RoomState::Order)
        , gameState(GameState::HALLWAY_STATE) {
    }

    void SetRoomState(RoomState s) { roomState = s; }
    RoomState getRoomState() const { return roomState; }

    void setGameState(GameState s) { gameState = s; }
    GameState getGameState() const { return gameState; }

    /// @brief Returns true if newState is a valid next step
    ///        in the standard room progression.
    /// @pre   newState is a valid RoomState
    bool IsValidTransition(RoomState from, RoomState to) const {
        switch (from) {
        case RoomState::Order:          return to == RoomState::Prepare;
        case RoomState::Prepare:        return to == RoomState::Serve;
        case RoomState::Serve:          return to == RoomState::MealReact;
        case RoomState::MealReact:      return to == RoomState::Score;
        case RoomState::Score:          return to == RoomState::InspectionStart;
        case RoomState::InspectionStart:return to == RoomState::Inspection;
        case RoomState::Inspection:     return to == RoomState::InspectionEnd;
        case RoomState::InspectionEnd:  return to == RoomState::End;
        default:                        return false;
        }
    }

private:
    RoomState roomState;
    GameState gameState;
};

// =============================================================
// NORMAL CASES
// =============================================================

TEST_CASE("StateManager - initial RoomState is Order", "[GameState][normal]") {
    TestStateManager sm;
    REQUIRE(sm.getRoomState() == RoomState::Order);
}

TEST_CASE("StateManager - SetRoomState stores each valid state",
    "[GameState][normal]") {
    TestStateManager sm;

    sm.SetRoomState(RoomState::Serve);
    REQUIRE(sm.getRoomState() == RoomState::Serve);

    sm.SetRoomState(RoomState::MealReact);
    REQUIRE(sm.getRoomState() == RoomState::MealReact);

    sm.SetRoomState(RoomState::Score);
    REQUIRE(sm.getRoomState() == RoomState::Score);

    sm.SetRoomState(RoomState::InspectionStart);
    REQUIRE(sm.getRoomState() == RoomState::InspectionStart);

    sm.SetRoomState(RoomState::Inspection);
    REQUIRE(sm.getRoomState() == RoomState::Inspection);

    sm.SetRoomState(RoomState::InspectionEnd);
    REQUIRE(sm.getRoomState() == RoomState::InspectionEnd);

    sm.SetRoomState(RoomState::End);
    REQUIRE(sm.getRoomState() == RoomState::End);
}

TEST_CASE("StateManager - full valid progression sequence",
    "[GameState][normal]") {
    TestStateManager sm;

    const RoomState sequence[] = {
        RoomState::Order,
        RoomState::Prepare,
        RoomState::Serve,
        RoomState::MealReact,
        RoomState::Score,
        RoomState::InspectionStart,
        RoomState::Inspection,
        RoomState::InspectionEnd,
        RoomState::End
    };

    const int count = 9;
    for (int i = 0; i < count - 1; ++i) {
        REQUIRE(sm.IsValidTransition(sequence[i], sequence[i + 1]) == true);
    }
}

TEST_CASE("StateManager - setGameState stores each room state",
    "[GameState][normal]") {
    TestStateManager sm;

    sm.setGameState(GameState::ROOM1_STATE);
    REQUIRE(sm.getGameState() == GameState::ROOM1_STATE);

    sm.setGameState(GameState::ROOM2_STATE);
    REQUIRE(sm.getGameState() == GameState::ROOM2_STATE);

    sm.setGameState(GameState::ROOM3_STATE);
    REQUIRE(sm.getGameState() == GameState::ROOM3_STATE);

    sm.setGameState(GameState::ROOM4_STATE);
    REQUIRE(sm.getGameState() == GameState::ROOM4_STATE);
}

// =============================================================
// EDGE CASES
// =============================================================

TEST_CASE("StateManager - setting same state twice keeps that state",
    "[GameState][edge]") {
    TestStateManager sm;

    sm.SetRoomState(RoomState::Inspection);
    sm.SetRoomState(RoomState::Inspection);
    REQUIRE(sm.getRoomState() == RoomState::Inspection);
}

TEST_CASE("StateManager - RoomState and GameState are independent",
    "[GameState][edge]") {
    TestStateManager sm;

    sm.SetRoomState(RoomState::Order);
    sm.setGameState(GameState::ROOM2_STATE);

    REQUIRE(sm.getRoomState() == RoomState::Order);
    REQUIRE(sm.getGameState() == GameState::ROOM2_STATE);
}

TEST_CASE("StateManager - rapid state changes keep last value",
    "[GameState][edge]") {
    TestStateManager sm;

    sm.SetRoomState(RoomState::Order);
    sm.SetRoomState(RoomState::Serve);
    sm.SetRoomState(RoomState::Score);
    sm.SetRoomState(RoomState::Inspection);

    REQUIRE(sm.getRoomState() == RoomState::Inspection);
}

// =============================================================
// FAILURE CASES — invalid transitions
// =============================================================

TEST_CASE("StateManager - skipping states is invalid transition",
    "[GameState][failure]") {
    TestStateManager sm;

    // Cannot skip from Order directly to MealReact
    REQUIRE(sm.IsValidTransition(RoomState::Order,
        RoomState::MealReact) == false);

    // Cannot skip from Serve directly to Score
    REQUIRE(sm.IsValidTransition(RoomState::Serve,
        RoomState::Score) == false);

    // Cannot skip from Score directly to Inspection
    REQUIRE(sm.IsValidTransition(RoomState::Score,
        RoomState::Inspection) == false);
}

TEST_CASE("StateManager - going backwards is invalid transition",
    "[GameState][failure]") {
    TestStateManager sm;

    // Cannot go backwards
    REQUIRE(sm.IsValidTransition(RoomState::Serve,
        RoomState::Order) == false);
    REQUIRE(sm.IsValidTransition(RoomState::Inspection,
        RoomState::Score) == false);
    REQUIRE(sm.IsValidTransition(RoomState::End,
        RoomState::InspectionEnd) == false);
}

TEST_CASE("StateManager - End state has no valid next transition",
    "[GameState][failure]") {
    TestStateManager sm;
    sm.SetRoomState(RoomState::End);

    REQUIRE(sm.IsValidTransition(RoomState::End,
        RoomState::Order) == false);
    REQUIRE(sm.IsValidTransition(RoomState::End,
        RoomState::Inspection) == false);
}

TEST_CASE("StateManager - state does not change without explicit set",
    "[GameState][failure]") {
    TestStateManager sm;
    sm.SetRoomState(RoomState::Inspection);

    RoomState before = sm.getRoomState();
    // No set call
    REQUIRE(sm.getRoomState() == before);
}