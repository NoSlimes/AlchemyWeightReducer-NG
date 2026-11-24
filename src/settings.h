#pragma once

#include <string>
#include <vector>

struct CategorySettings
{
    bool enabled{ true };
    bool useFlatWeight{ true };   // false = Multiply, true = Flat
    float multiplier{ 0.5f };
    float flatWeight{ 0.1f };
    float minWeight{ 0.01f };
    float maxWeight{ -1.0f };     // < 0 = no upper limit
};

struct Settings
{
    bool preserveZeroWeight{ true };
    bool roundToTwoDecimals{ true };

    CategorySettings ingredients{ .enabled = true, .useFlatWeight = true, .multiplier = 0.5f, .flatWeight = 0.05f };
    CategorySettings potions{ .enabled = true, .useFlatWeight = true, .multiplier = 0.5f, .flatWeight = 0.10f };
    CategorySettings poisons{ .enabled = true, .useFlatWeight = true, .multiplier = 0.5f, .flatWeight = 0.05f };
    CategorySettings food{ .enabled = false, .useFlatWeight = true, .multiplier = 0.5f, .flatWeight = 0.10f };

    std::vector<std::string> excludedKeywords;

    static Settings& GetSingleton()
    {
        static Settings instance;
        return instance;
    }

    void Load();
};