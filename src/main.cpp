#include "PCH.h"
#include "Settings.h"
#include <algorithm>
#include <cmath>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    void SetupLog()
    {
        auto logsFolder = SKSE::log::log_directory();
        if (!logsFolder)
        {
            SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
            return;
        }

        auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
        auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
        auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
        auto logger = std::make_shared<spdlog::logger>("log", std::move(fileLogger));

        spdlog::set_default_logger(std::move(logger));
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::info);
    }

    bool HasExcludedKeyword(const RE::TESForm* form, const std::vector<std::string>& keywords)
    {
        if (keywords.empty())
        {
            return false;
        }
        const auto* kwForm = form->As<RE::BGSKeywordForm>();
        if (!kwForm)
        {
            return false;
        }
        return std::ranges::any_of(keywords, [kwForm](const std::string& kw)
            {
                return kwForm->HasKeywordString(kw);
            });
    }

    float ComputeWeight(float original, const CategorySettings& cfg, const Settings& settings)
    {
        if (!cfg.enabled)
        {
            return original;
        }
        if (settings.preserveZeroWeight && original <= 0.0f)
        {
            return original;
        }

        float w = cfg.useFlatWeight ? cfg.flatWeight : (original * cfg.multiplier);

        if (cfg.minWeight >= 0.0f)
        {
            w = (std::max)(w, cfg.minWeight);
        }
        if (cfg.maxWeight >= 0.0f)
        {
            w = (std::min)(w, cfg.maxWeight);
        }
        if (settings.roundToTwoDecimals)
        {
            w = std::round(w * 100.0f) / 100.0f;
        }

        return (std::max)(w, 0.0f);
    }

    void ApplyDynamicWeights()
    {
        auto& settings = Settings::GetSingleton();
        settings.Load();

        auto* dataHandler = RE::TESDataHandler::GetSingleton();
        if (!dataHandler)
        {
            return;
        }

        std::size_t ingrCount = 0;
        if (settings.ingredients.enabled)
        {
            for (auto* ingr : dataHandler->GetFormArray<RE::IngredientItem>())
            {
                if (!ingr || HasExcludedKeyword(ingr, settings.excludedKeywords))
                {
                    continue;
                }
                ingr->weight = ComputeWeight(ingr->weight, settings.ingredients, settings);
                ++ingrCount;
            }
        }

        std::size_t potionCount = 0;
        std::size_t poisonCount = 0;
        std::size_t foodCount = 0;

        for (auto* alch : dataHandler->GetFormArray<RE::AlchemyItem>())
        {
            if (!alch || HasExcludedKeyword(alch, settings.excludedKeywords))
            {
                continue;
            }

            if (alch->IsFood())
            {
                if (settings.food.enabled)
                {
                    alch->weight = ComputeWeight(alch->weight, settings.food, settings);
                    ++foodCount;
                }
            }
            else if (alch->IsPoison())
            {
                if (settings.poisons.enabled)
                {
                    alch->weight = ComputeWeight(alch->weight, settings.poisons, settings);
                    ++poisonCount;
                }
            }
            else
            {
                if (settings.potions.enabled)
                {
                    alch->weight = ComputeWeight(alch->weight, settings.potions, settings);
                    ++potionCount;
                }
            }
        }

        SKSE::log::info("Updated {} ingredients, {} potions, {} poisons, {} food items.",
            ingrCount, potionCount, poisonCount, foodCount);
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);
    SetupLog();

    SKSE::log::info("AlchemyWeightReducer initializing...");

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* msg)
        {
            if (msg->type == SKSE::MessagingInterface::kDataLoaded)
            {
                ApplyDynamicWeights();
            }
        });

    return true;
}