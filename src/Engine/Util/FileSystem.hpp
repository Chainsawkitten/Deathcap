#pragma once

#include <string>
#include <vector>

/// Functionality to interact with the file system.
namespace FileSystem {
    /// Delimiter, '\' on Windows, '/' elsewhere.
    extern const char DELIMITER;
    
    /// A file.
    extern const unsigned int FILE;
    
    /// A directory.
    extern const unsigned int DIRECTORY;
    
    /// Check if a file exists.
    /**
     * Works for directories as well.
     * @param filename Filename (either relative or absolute) to check.
     * @return Whether the file exists
     */
    bool FileExists(const char* filename);
    
    /// Create a directory if it does not already exist.
    /**
     * Permission for new directory is 0777 on Unix.
     * @param filename Filename (either absolute or relative) for the directory to create.
     */
    void CreateDirectory(const char* filename);
    
    /// Get all the contents of a directory.
    /**
     * @param directoryName Path to the directory to scan.
     * @param type Type of content to get, FILE, DIRECTORY or both (use | to combine).
     * @return A list of all the files/directories in the directory.
     */
    std::vector<std::string> DirectoryContents(const std::string& directoryName, unsigned int type = FILE | DIRECTORY);
    
    /// Get save path for application data folder.
    /**
     * The application data folder is created if it does not already exist.
     * On Windows, this is \<User>\\AppData\\Roaming\\\<AppName>.
     * On MacOS, this is ~/Library/Application Support/\<AppName>.
     * On Linux, this is ~/.local/\<AppName>.
     * @param appName The name of the app/game.
     * @return The path
     */
    std::string DataPath(const char* appName);
    
    /// Get save path for application data.
    /**
     * The application data folder is created if it does not already exist.
     * On Windows, this is \<User>\\AppData\\Roaming\\\<AppName>\\\<Filename>.
     * On MacOS, this is ~/Library/Application Support/\<AppName>/\<Filename>.
     * On Linux, this is ~/.local/\<AppName>/\<Filename>.
     * @param appName The name of the app/game.
     * @param filename Filename, eg. "settings.ini".
     * @return The path
     */
    std::string DataPath(const char* appName, const char* filename);
    
    /// Get the parent directory of a path.
    /**
     * @param path Absolute path to get the parent directory of.
     * @return Path of the parent directory.
     */
    std::string GetParentDirectory(const std::string& path);
}
