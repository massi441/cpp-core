#pragma once

#ifndef NN_SWITCH

#include <filesystem>
#include <fstream>
#include <unordered_set>

#include "Core/Result/ReturnStatus.h"
#include "Core/Result/ReturnValue.h"

namespace ml {

template <typename ... Args>
std::filesystem::path fromCurrentPath(Args... args) {
    std::filesystem::path path = std::filesystem::current_path();
    (path.append(std::forward<Args>(args)), ...);
    return path;
}

template <typename ...Args>
std::filesystem::path makePath(Args&& ...args) {
    std::filesystem::path path;
    (path.append(std::forward<Args>(args)), ...);
    return path;
}

template <typename ... Args>
std::filesystem::path fromParentPath(const std::filesystem::path& basePath, Args&& ...args) {
    std::filesystem::path path = basePath.parent_path();
    (path.append(std::forward<Args>(args)), ...);
    return path;
}

ml::ReturnStatus ensureDirCreated(const std::filesystem::path& path);
ml::ReturnStatus clearDirectory(const std::filesystem::path& path);
ml::ReturnStatus removeDirectory(const std::filesystem::path& path);
ml::ReturnStatus isExistPath(const std::filesystem::path& path);
ml::ReturnStatus isExistParentPath(const std::filesystem::path& path);
ml::ReturnStatus createDirectory(const std::filesystem::path& path);
ml::ReturnStatus copyRecursiveOverwrite(const std::filesystem::path& from, const std::filesystem::path& to);

/**
 * Backups a directory by copying numbered versions into a destination directory,
 * in ascending order of recency.
 * @param source The folder to back-up
 * @param backupsDest The folder where numbered backups are stores
 * @param depth The amount of backups that should be stored in the destination folder
 * @return True if the full operation was successful, false otherwise.
 */
ml::ReturnStatus backupDirNumbered(const std::filesystem::path& source, const std::filesystem::path& backupsDest, uint32_t depth);

ml::ReturnValue<uintmax_t> getFileSize(const std::filesystem::path& path);

ml::ReturnValue<std::vector<std::string>> readLines(const std::filesystem::path& path, int maxLines = -1);
ml::ReturnValue<std::unordered_set<std::string>> readUniqueLines(const std::filesystem::path& path, int maxLines = -1);

template <typename T>
ml::ReturnStatus writeLines(const std::filesystem::path& path, const T& lines) {
    std::ofstream file(path);
    if (!file) {
        return ml::ReturnStatus::Failure("Failed to open file " + path.string());
    }

    for (const std::string& line : lines) {
        file << line << std::endl;
    }

    return ml::ReturnStatus::Success();
}

/**
 * Returns the stem of a given path (the file name without the last extension)
 */
std::string fileName(const std::filesystem::path& path);

}

#endif
