// TestKitchenData.cpp
// Tests for KitchenData singleton — food selection, plate completion.
//
// Normal cases:  valid food selections and plate checks
// Edge cases:    setting the same item twice, optional milk
// Failure cases: empty plate, incomplete plate

#include "catch2/catch_amalgamated.hpp"
#include "Scene/KitchenData.h"

// Helper to reset KitchenData between tests
static void ResetKitchen()
{
    KitchenData::GetInstance()->clearPlate();
}

// =============================================================
// NORMAL CASES
// =============================================================

TEST_CASE("KitchenData - setSandwich stores correct value", "[KitchenData][normal]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    REQUIRE(kitchen->getSandwich() == SALMON);

    kitchen->setSandwich(BEEF);
    REQUIRE(kitchen->getSandwich() == BEEF);

    kitchen->setSandwich(EGG);
    REQUIRE(kitchen->getSandwich() == EGG);

    kitchen->setSandwich(CUCUMBER);
    REQUIRE(kitchen->getSandwich() == CUCUMBER);
}

TEST_CASE("KitchenData - setTea stores correct value", "[KitchenData][normal]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setTea(EARLGREYTEA);
    REQUIRE(kitchen->getTea() == EARLGREYTEA);

    kitchen->setTea(ASSAMTEA);
    REQUIRE(kitchen->getTea() == ASSAMTEA);

    kitchen->setTea(CHAMOMILETEA);
    REQUIRE(kitchen->getTea() == CHAMOMILETEA);

    kitchen->setTea(GREENTEA);
    REQUIRE(kitchen->getTea() == GREENTEA);
}

TEST_CASE("KitchenData - setDessert stores correct value", "[KitchenData][normal]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setDessert(ECLAIR);
    REQUIRE(kitchen->getDessert() == ECLAIR);

    kitchen->setDessert(MACARON);
    REQUIRE(kitchen->getDessert() == MACARON);

    kitchen->setDessert(TART);
    REQUIRE(kitchen->getDessert() == TART);

    kitchen->setDessert(SCONE);
    REQUIRE(kitchen->getDessert() == SCONE);
}

TEST_CASE("KitchenData - setOptional stores milk correctly", "[KitchenData][normal]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setOptional(MILK);
    REQUIRE(kitchen->getOptional() == MILK);
}

TEST_CASE("KitchenData - checkCompletePlate returns true when all three set", "[KitchenData][normal]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    kitchen->setTea(EARLGREYTEA);
    kitchen->setDessert(ECLAIR);

    REQUIRE(kitchen->checkCompletePlate() == true);
}

TEST_CASE("KitchenData - clearPlate resets all choices to empty", "[KitchenData][normal]")
{
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    kitchen->setTea(EARLGREYTEA);
    kitchen->setDessert(ECLAIR);
    kitchen->setOptional(MILK);

    kitchen->clearPlate();

    REQUIRE(kitchen->getSandwich() == SANDWICH_EMPTY);
    REQUIRE(kitchen->getTea() == TEA_EMPTY);
    REQUIRE(kitchen->getDessert() == DESSERT_EMPTY);
    REQUIRE(kitchen->getOptional() == OPTIONAL_EMPTY);
}

// =============================================================
// EDGE CASES
// =============================================================

TEST_CASE("KitchenData - setting same item twice keeps latest value", "[KitchenData][edge]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    kitchen->setSandwich(BEEF);
    REQUIRE(kitchen->getSandwich() == BEEF);

    kitchen->setTea(EARLGREYTEA);
    kitchen->setTea(GREENTEA);
    REQUIRE(kitchen->getTea() == GREENTEA);

    kitchen->setDessert(ECLAIR);
    kitchen->setDessert(SCONE);
    REQUIRE(kitchen->getDessert() == SCONE);
}

TEST_CASE("KitchenData - checkCompletePlate ignores optional milk", "[KitchenData][edge]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    // Plate is complete without optional
    kitchen->setSandwich(EGG);
    kitchen->setTea(CHAMOMILETEA);
    kitchen->setDessert(SCONE);
    REQUIRE(kitchen->checkCompletePlate() == true);

    // Still complete with optional added
    kitchen->setOptional(MILK);
    REQUIRE(kitchen->checkCompletePlate() == true);
}

TEST_CASE("KitchenData - clearPlate then checkCompletePlate returns false", "[KitchenData][edge]")
{
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    kitchen->setTea(EARLGREYTEA);
    kitchen->setDessert(ECLAIR);
    kitchen->clearPlate();

    REQUIRE(kitchen->checkCompletePlate() == false);
}

// =============================================================
// FAILURE CASES
// =============================================================

TEST_CASE("KitchenData - checkCompletePlate false when only sandwich set", "[KitchenData][failure]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    REQUIRE(kitchen->checkCompletePlate() == false);
}

TEST_CASE("KitchenData - checkCompletePlate false when only tea set", "[KitchenData][failure]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setTea(EARLGREYTEA);
    REQUIRE(kitchen->checkCompletePlate() == false);
}

TEST_CASE("KitchenData - checkCompletePlate false when only dessert set", "[KitchenData][failure]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setDessert(ECLAIR);
    REQUIRE(kitchen->checkCompletePlate() == false);
}

TEST_CASE("KitchenData - checkCompletePlate false when sandwich and tea but no dessert", "[KitchenData][failure]")
{
    ResetKitchen();
    KitchenData *kitchen = KitchenData::GetInstance();

    kitchen->setSandwich(SALMON);
    kitchen->setTea(EARLGREYTEA);
    REQUIRE(kitchen->checkCompletePlate() == false);
}

TEST_CASE("KitchenData - checkCompletePlate false on fresh instance", "[KitchenData][failure]")
{
    ResetKitchen();
    REQUIRE(KitchenData::GetInstance()->checkCompletePlate() == false);
}