#pragma once

#include <string>
#include <vector>
#include "../linking.hpp"

/// Functionality to interact with the file system.
namespace FileSystem {
    /// Delimiter, '\' on Windows, '/' elsewhere.
    ENGINE_EXPORT extern const char DELIMITER;
    
    /// A file.
    ENGINE_EXPORT extern const unsigned int FILE;
    
    /// A directory.
    ENGINE_EXPORT extern const unsigned int DIRECTORY;
    
    /// Check if a file exists.
    /**
     * Works for directories as well.
     * @param filename Filename (either relative or absolute) to check.
     * @return Whether the file exists
     */
    ENGINE_EXPORT bool FileExists(const char* filename);
    
    /// Copy a file.
    /**
     * @param source Source to copy.
     * @param destination Destination to copy to.
     */
    ENGINE_EXPORT void Copy(const char* source, const char* destination);
    
    /// Create a directory if it does not already exist.
    /**
     * Permission for new directory is 0777 on Unix.
     * @param filename Filename (either absolute or relative) for the directory to create.
     */
    ENGINE_EXPORT void CreateDirectory(const char* filename);
    
    /// Get all the contents of a directory.
    /**
     * @param directoryName Path to the directory to scan.
     * @param type Type of content to get, FILE, DIRECTORY or both (use | to combine).
     * @return A list of all the files/directories in the directory.
     */
    ENGINE_EXPORT std::vector<std::string> DirectoryContents(const std::string& directoryName, unsigned int type = FILE | DIRECTORY);
    
    /// Get save path for application data folder.
    /**
     * The application data folder is created if it does not already exist.
     * On Windows, this is \<User>\\AppData\\Roaming\\\<AppName>.
     * On MacOS, this is ~/Library/Application Support/\<AppName>.
     * On Linux, this is ~/.local/share/\<AppName>.
     * @param appName The name of the app/game.
     * @return The path
     */
    ENGINE_EXPORT std::string DataPath(const char* appName);
    
    /// Get save path for application data.
    /**
     * The application data folder is created if it does not already exist.
     * On Windows, this is \<User>\\AppData\\Roaming\\\<AppName>\\\<Filename>.
     * On MacOS, this is ~/Library/Application Support/\<AppName>/\<Filename>.
     * On Linux, this is ~/.local/share/\<AppName>/\<Filename>.
     * @param appName The name of the app/game.
     * @param filename Filename, eg. "settings.ini".
     * @return The path
     */
    ENGINE_EXPORT std::string DataPath(const char* appName, const char* filename);
    
    /// Get the parent directory of a path.
    /**
     * @param path Absolute path to get the parent directory of.
     * @return Path of the parent directory.
     */
    ENGINE_EXPORT std::string GetParentDirectory(const std::string& path);
    
    /// Get the extension part of a filename.
    /**
     * @param filename The filename to check.
     * @return The extension part of the filename, converted to lower case.
     */
    ENGINE_EXPORT std::string GetExtension(const std::string& filename);

    /// Get the name of the file without the extension.
    /**
     * @param The datapath to the file.
     * @return The name of the file without extension.
     */
    ENGINE_EXPORT std::string GetName(const std::string& filepath);
    
    /// Get the directory part for a path (including the final delimiter).
    /**
     * @param path The path to get the directory part of.
     * @return The directory path of the path.
     */
    ENGINE_EXPORT std::string GetDirectory(const std::string& path);

    /// Rename an existing file.
    /**
     * @param filepath The path of the original file.
     * @param name New name of the file.
     * @return Path to the renamed file.
     */
    ENGINE_EXPORT std::string Rename(const std::string& filepath, const std::string& name);
    
    /// Execute a program asynchronously.
    /**
     * @param path The path to the program to execute.
     * @param arguments The arguments to the program.
     */
    ENGINE_EXPORT void ExecuteProgram(const std::string& path, const std::string& arguments);
}
