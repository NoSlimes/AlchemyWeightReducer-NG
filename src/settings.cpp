#include "PCH.h"
#include "Settings.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <format>
#include <SimpleIni.h>
#include <sstream>

namespace
{
    std::string Trim(std::string s)
    {
        auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
        s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
        return s;
    }

    std::vector<std::string> SplitCSV(const char* raw)
    {
        std::vector<std::string> out;
        if (!raw || *raw == '\0')
        {
            return out;
        }
        std::stringstream ss(raw);
        std::string item;
        while (std::getline(ss, item, ','))
        {
            item = Trim(std::move(item));
            if (!item.empty())
            {
                out.push_back(std::move(item));
            }
        }
        return out;
    }

    bool EqualsIgnoreCase(std::string_view a, std::string_view b)
    {
        return std::ranges::equal(a, b, [](char c1, char c2)
            {
                return std::tolower(static_cast<unsigned char>(c1)) ==
                    std::tolower(static_cast<unsigned char>(c2));
            });
    }

    std::string FormatFloat(float v)
    {
        double clean = std::round(static_cast<double>(v) * 10000.0) / 10000.0;
        if (clean == 0.0)
        {
            clean = 0.0;
        }
        return std::format("{}", clean);
    }

    void SyncCategory(CSimpleIniA& ini, const char* section, CategorySettings& cfg)
    {
        cfg.enabled = ini.GetBoolValue(section, "bEnabled", cfg.enabled);

        const char* modeDefault = cfg.useFlatWeight ? "Flat" : "Multiply";
        const char* modeStr = ini.GetValue(section, "sMode", modeDefault);
        cfg.useFlatWeight = EqualsIgnoreCase(modeStr ? modeStr : "", "Flat");

        cfg.multiplier = static_cast<float>(ini.GetDoubleValue(section, "fMultiplier", cfg.multiplier));
        cfg.flatWeight = static_cast<float>(ini.GetDoubleValue(section, "fFlatWeight", cfg.flatWeight));
        cfg.minWeight = static_cast<float>(ini.GetDoubleValue(section, "fMinWeight", cfg.minWeight));
        cfg.maxWeight = static_cast<float>(ini.GetDoubleValue(section, "fMaxWeight", cfg.maxWeight));

        // Write defaults back if any key was missing
        ini.SetBoolValue(section, "bEnabled", cfg.enabled);
        ini.SetValue(section, "sMode", cfg.useFlatWeight ? "Flat" : "Multiply", "; Multiply or Flat");
        ini.SetValue(section, "fMultiplier", FormatFloat(cfg.multiplier).c_str());
        ini.SetValue(section, "fFlatWeight", FormatFloat(cfg.flatWeight).c_str());
        ini.SetValue(section, "fMinWeight", FormatFloat(cfg.minWeight).c_str());
        ini.SetValue(section, "fMaxWeight", FormatFloat(cfg.maxWeight).c_str(), "; -1.0 = no upper limit");
    }
}

void Settings::Load()
{
    const std::filesystem::path iniPath = "Data/SKSE/Plugins/AlchemyWeightReducer.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    const bool fileExists = std::filesystem::exists(iniPath);
    if (fileExists)
    {
        if (ini.LoadFile(iniPath.string().c_str()) < 0)
        {
            SKSE::log::error("Failed to parse {}, using defaults.", iniPath.string());
            return;
        }
    }

    preserveZeroWeight = ini.GetBoolValue("General", "bPreserveZeroWeight", preserveZeroWeight);
    roundToTwoDecimals = ini.GetBoolValue("General", "bRoundToTwoDecimals", roundToTwoDecimals);

    ini.SetBoolValue("General", "bPreserveZeroWeight", preserveZeroWeight, "; Keep 0.0 weight items at 0.0");
    ini.SetBoolValue("General", "bRoundToTwoDecimals", roundToTwoDecimals);

    SyncCategory(ini, "Ingredients", ingredients);
    SyncCategory(ini, "Potions", potions);
    SyncCategory(ini, "Poisons", poisons);
    SyncCategory(ini, "Food", food);

    const char* rawKeywords = ini.GetValue("Exclusions", "sExcludedKeywords", "");
    excludedKeywords = SplitCSV(rawKeywords);
    ini.SetValue("Exclusions", "sExcludedKeywords", rawKeywords ? rawKeywords : "",
        "; Comma-separated keyword EditorIDs, e.g. VendorItemDrink");

    if (!fileExists)
    {
        std::filesystem::create_directories(iniPath.parent_path());
        ini.SaveFile(iniPath.string().c_str());
        SKSE::log::info("Generated default config at {}", iniPath.string());
    }
    else
    {
        SKSE::log::info("Loaded config from {}", iniPath.string());
    }
}