// TestCharacterData.cpp
// Tests score calculation logic in isolation.
// We replicate the scoring rules here to verify boundary conditions
// without pulling in the full engine header chain.
//
// Score ranges (from CharacterData.h):
//   LOW:         0 - 3
//   AVERAGE:     4 - 7
//   INFORMATIVE: 8+
//
// Late serve penalty: -2

#include "catch2/catch_amalgamated.hpp"

// Replicate just the enums and logic we need — no engine headers required
enum ScoreLevel { LOW, AVERAGE, INFORMATIVE };

/// @brief Mirrors CharacterData::getServeScoreLevel boundary logic.
/// @pre   score can be any int (negative possible with late penalty)
/// @post  returns correct level for the given score
static ScoreLevel ClassifyScore(int score) {
    if (score <= 3) return LOW;
    if (score >= 4 && score <= 7) return AVERAGE;
    return INFORMATIVE;
}

/// @brief Mirrors CharacterData::calculateTimeServe penalty.
/// @pre   remainingTime >= 0
/// @post  returns 0 if on time, -2 if late
static int TimeServePenalty(int remainingTime) {
    if (remainingTime == 0) return -2;
    return 0;
}

/// @brief Combines meal score and time penalty.
/// @pre   mealScore >= 0, remainingTime >= 0
static int TotalScore(int mealScore, int remainingTime) {
    return mealScore + TimeServePenalty(remainingTime);
}

// =============================================================
// NORMAL CASES
// =============================================================

TEST_CASE("ScoreLevel - score of 0 gives LOW", "[CharacterData][normal]") {
    REQUIRE(ClassifyScore(0) == LOW);
}

TEST_CASE("ScoreLevel - score of 2 gives LOW", "[CharacterData][normal]") {
    REQUIRE(ClassifyScore(2) == LOW);
}

TEST_CASE("ScoreLevel - score of 5 gives AVERAGE", "[CharacterData][normal]") {
    REQUIRE(ClassifyScore(5) == AVERAGE);
}

TEST_CASE("ScoreLevel - score of 10 gives INFORMATIVE", "[CharacterData][normal]") {
    REQUIRE(ClassifyScore(10) == INFORMATIVE);
}

TEST_CASE("TimeServePenalty - on time gives 0 penalty", "[CharacterData][normal]") {
    REQUIRE(TimeServePenalty(30) == 0);
    REQUIRE(TimeServePenalty(1) == 0);
}

TEST_CASE("TimeServePenalty - late gives -2 penalty", "[CharacterData][normal]") {
    REQUIRE(TimeServePenalty(0) == -2);
}

TEST_CASE("TotalScore - good food on time stays INFORMATIVE",
    "[CharacterData][normal]") {
    // mealScore=10, on time → total=10 → INFORMATIVE
    REQUIRE(ClassifyScore(TotalScore(10, 30)) == INFORMATIVE);
}

TEST_CASE("TotalScore - average food on time stays AVERAGE",
    "[CharacterData][normal]") {
    // mealScore=6, on time → total=6 → AVERAGE
    REQUIRE(ClassifyScore(TotalScore(6, 30)) == AVERAGE);
}

TEST_CASE("TotalScore - low food on time stays LOW",
    "[CharacterData][normal]") {
    // mealScore=2, on time → total=2 → LOW
    REQUIRE(ClassifyScore(TotalScore(2, 30)) == LOW);
}

// =============================================================
// EDGE CASES — boundary values
// =============================================================

TEST_CASE("ScoreLevel - boundary: score 3 gives LOW", "[CharacterData][edge]") {
    REQUIRE(ClassifyScore(3) == LOW);
}

TEST_CASE("ScoreLevel - boundary: score 4 gives AVERAGE", "[CharacterData][edge]") {
    REQUIRE(ClassifyScore(4) == AVERAGE);
}

TEST_CASE("ScoreLevel - boundary: score 7 gives AVERAGE", "[CharacterData][edge]") {
    REQUIRE(ClassifyScore(7) == AVERAGE);
}

TEST_CASE("ScoreLevel - boundary: score 8 gives INFORMATIVE",
    "[CharacterData][edge]") {
    REQUIRE(ClassifyScore(8) == INFORMATIVE);
}

TEST_CASE("TotalScore - late penalty pushes boundary score down one level",
    "[CharacterData][edge]") {
    // mealScore=4 (lowest AVERAGE) - 2 penalty = 2 → LOW
    REQUIRE(ClassifyScore(TotalScore(4, 0)) == LOW);
}

TEST_CASE("TotalScore - late penalty pushes boundary INFORMATIVE to AVERAGE",
    "[CharacterData][edge]") {
    // mealScore=8 (lowest INFORMATIVE) - 2 = 6 → AVERAGE
    REQUIRE(ClassifyScore(TotalScore(8, 0)) == AVERAGE);
}

TEST_CASE("TotalScore - late penalty on already LOW stays LOW",
    "[CharacterData][edge]") {
    // mealScore=2 - 2 = 0 → still LOW
    REQUIRE(ClassifyScore(TotalScore(2, 0)) == LOW);
}

// =============================================================
// FAILURE CASES
// =============================================================

TEST_CASE("ScoreLevel - negative score gives LOW", "[CharacterData][failure]") {
    // Can happen when late penalty applied to score of 0
    REQUIRE(ClassifyScore(-2) == LOW);
}

TEST_CASE("TotalScore - worst food late gives lowest possible score",
    "[CharacterData][failure]") {
    // mealScore=0, late → total=-2 → LOW
    int total = TotalScore(0, 0);
    REQUIRE(total == -2);
    REQUIRE(ClassifyScore(total) == LOW);
}

TEST_CASE("TotalScore - good food but late can lose INFORMATIVE",
    "[CharacterData][failure]") {
    // mealScore=9, late → total=7 → AVERAGE, not INFORMATIVE
    REQUIRE(ClassifyScore(TotalScore(9, 0)) == AVERAGE);
}