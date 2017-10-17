#pragma once

#include <SimpleIni.h>
#include <string>
#include <map>
#include "../linking.hpp"

/// Handles settings.
class Settings {
    public:
        /// Get a bool setting.
        /**
         * @param settingName Setting name.
         * @return The value of the setting
         */
        ENGINE_EXPORT bool GetBool(const std::string& settingName);
        
        /// Set a bool setting.
        /**
         * @param settingName Setting name.
         * @param value Value to set it to.
         */
        ENGINE_EXPORT void SetBool(const std::string& settingName, bool value);
        
        /// Get a long setting.
        /**
         * @param settingName Setting name.
         * @return The value of the setting
         */
        ENGINE_EXPORT long GetLong(const std::string& settingName);
        
        /// Set a long setting.
        /**
         * @param settingName Setting name.
         * @param value Value to set it to.
         */
        ENGINE_EXPORT void SetLong(const std::string& settingName, long value);
        
        /// Get a double setting.
        /**
         * @param settingName Setting name.
         * @return The value of the setting
         */
        ENGINE_EXPORT double GetDouble(const std::string& settingName);
        
        /// Set a double setting.
        /**
         * @param settingName Setting name.
         * @param value Value to set it to.
         */
        ENGINE_EXPORT void SetDouble(const std::string& settingName, double value);
        
        /// Get a string setting.
        /**
         * @param settingName Setting name.
         * @return The value of the setting
         */
        ENGINE_EXPORT std::string GetString(const std::string& settingName);
        
        /// Set a string setting.
        /**
         * @param settingName Setting name.
         * @param value Value to set it to.
         */
        ENGINE_EXPORT void SetString(const std::string& settingName, const std::string& value);
        
        /// Save the settings to an INI file.
        virtual void Save() = 0;
        
    protected:
        /// Ini file.
        CSimpleIni ini;
        
        /// Add a bool setting.
        /**
         * @param settingName Setting name.
         * @param section INI section.
         * @param key INI key.
         * @param defaultValue Default value.
         */
        ENGINE_EXPORT void AddBoolSetting(const std::string& settingName, const std::string& section, const std::string& key, bool defaultValue);
        
        /// Add a long setting.
        /**
         * @param settingName Setting name.
         * @param section INI section.
         * @param key INI key.
         * @param defaultValue Default value.
         */
        ENGINE_EXPORT void AddLongSetting(const std::string& settingName, const std::string& section, const std::string& key, long defaultValue);
        
        /// Add a double setting.
        /**
         * @param settingName Setting name.
         * @param section INI section.
         * @param key INI key.
         * @param defaultValue Default value.
         */
        ENGINE_EXPORT void AddDoubleSetting(const std::string& settingName, const std::string& section, const std::string& key, double defaultValue);
        
        /// Add a string setting.
        /**
         * @param settingName Setting name.
         * @param section INI section.
         * @param key INI key.
         * @param defaultValue Default value.
         */
        ENGINE_EXPORT void AddStringSetting(const std::string& settingName, const std::string& section, const std::string& key, const std::string& defaultValue);
        
    private:
        struct BoolSetting {
            std::string section;
            std::string key;
            bool defaultValue;
        };
        
        struct LongSetting {
            std::string section;
            std::string key;
            long defaultValue;
        };
        
        struct DoubleSetting {
            std::string section;
            std::string key;
            double defaultValue;
        };
        
        struct StringSetting {
            std::string section;
            std::string key;
            std::string defaultValue;
        };
        
        std::map<std::string, BoolSetting> boolSettings;
        std::map<std::string, LongSetting> longSettings;
        std::map<std::string, DoubleSetting> doubleSettings;
        std::map<std::string, StringSetting> stringSettings;
};
