#pragma once

enum Sandwich
{
    SALMON,
    EGG,
    CUCUMBER,
    BEEF,
    SANDWICH_EMPTY
};

enum Dessert
{
    ECLAIR,
    MACARON,
    TART,
    SCONE,
    DESSERT_EMPTY
};

enum Tea
{
    EARLGREYTEA,
    ASSAMTEA,
    GREENTEA,
    CHAMOMILETEA,
    TEA_EMPTY
};

enum Optional
{
    MILK,
    OPTIONAL_EMPTY
};

class KitchenData
{

public:
    static KitchenData *GetInstance()
    {

        static KitchenData instance;
        return &instance;
    }

    void clearPlate()
    {

        sandwichChoice = SANDWICH_EMPTY;
        dessertChoice = DESSERT_EMPTY;
        teaChoice = TEA_EMPTY;
        optionalChoice = OPTIONAL_EMPTY;
    }

    void setSandwich(Sandwich choice)
    {
        sandwichChoice = choice;
    }
    void setDessert(Dessert choice)
    {
        dessertChoice = choice;
    }
    void setTea(Tea choice)
    {
        teaChoice = choice;
    }
    void setOptional(Optional choice)
    {
        optionalChoice = choice;
    }

    Sandwich getSandwich()
    {
        return sandwichChoice;
    }
    Tea getTea()
    {
        return teaChoice;
    }
    Dessert getDessert()
    {
        return dessertChoice;
    }
    Optional getOptional()
    {
        return optionalChoice;
    }

    bool checkCompletePlate()
    { // RETURNS TRUE IF 3 CATEGORIES ON PLATE

        if ((sandwichChoice != SANDWICH_EMPTY) && (teaChoice != TEA_EMPTY) && (dessertChoice != DESSERT_EMPTY))
        {
            return true;
        }
        return false;
    }

private:
    KitchenData()
    {

        sandwichChoice = SANDWICH_EMPTY;
        dessertChoice = DESSERT_EMPTY;
        teaChoice = TEA_EMPTY;
        optionalChoice = OPTIONAL_EMPTY;
    }

    Sandwich sandwichChoice;
    Dessert dessertChoice;
    Tea teaChoice;
    Optional optionalChoice;
};
