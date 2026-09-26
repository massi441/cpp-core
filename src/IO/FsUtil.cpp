#ifndef NN_SWITCH

#include "Core/IO/FsUtil.h"

#include <fstream>

namespace fs = std::filesystem;

namespace ml {

ml::ReturnStatus ensureDirCreated(const fs::path& path) {
    std::error_code ec;
    if (fs::exists(path, ec)) {
        return ml::ReturnStatus::Success();
    }

    fs::create_directory(path, ec);

    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnStatus clearDirectory(const fs::path& path) {
    std::error_code ec;

    std::vector<fs::path> removeableEntries;
    for (const fs::directory_entry& entry : fs::directory_iterator(path, ec)) {
        if (ec) {
            return ml::ReturnStatus::Failure("Error while clearing \"", path.string().c_str(), "\" directory: ", ec.message().c_str());
        }

        removeableEntries.push_back(entry.path());
    }

    for (const fs::path& removeableEntry : removeableEntries) {
        fs::remove_all(removeableEntry, ec);
        if (ec) {
            return ml::ReturnStatus::Failure("Error while clearing \"", path.string().c_str(), "\" directory: ", ec.message().c_str());
        }
    }

    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnStatus removeDirectory(const std::filesystem::path& path) {
    std::error_code ec;
    fs::remove_all(path, ec);

    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnStatus isExistPath(const fs::path& path) {
    std::error_code ec;
    if (!fs::exists(path, ec)) {
        if (ec) {
            return ml::ReturnStatus::Failure("Error while looking up \"", path.string().c_str(), "\" path: ", ec.message().c_str());
        }

        return ml::ReturnStatus::Failure();
    }

    return fs::exists(path, ec)
        ? ml::ReturnStatus::Success()
        : ml::ReturnStatus::Failure(ec.message());
}

ml::ReturnStatus isExistParentPath(const std::filesystem::path& path) {
    fs::path parentPath = path.parent_path();

    return !parentPath.empty()
        ? ml::ReturnStatus::Success()
        : ml::isExistPath(parentPath);
}

ml::ReturnStatus createDirectory(const fs::path& path) {
    std::error_code ec;
    fs::create_directory(path, ec);
    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnStatus copyRecursiveOverwrite(const std::filesystem::path& from, const std::filesystem::path& to) {
    std::error_code ec;
    fs::copy_options options = fs::copy_options::recursive | fs::copy_options::overwrite_existing;

    ml::ReturnStatus clearStatus = ml::clearDirectory(to);
    if (!clearStatus) {
        return ml::ReturnStatus::Failure("Failed to clear \"", to.string().c_str(), "\" directory before copy overwrite operation: ", clearStatus.message());
    }

    fs::copy(from, to, options, ec);

    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnStatus backupDirNumbered(const std::filesystem::path& source, const std::filesystem::path& backupsDest, uint32_t depth) {
    std::error_code ec;
    if (ml::isExistPath(backupsDest)) {
        // remove oldest back up (if present)
        fs::path maxBackupPath = backupsDest / std::to_string(depth);

        if (ml::isExistPath(maxBackupPath)) {
            fs::remove_all(maxBackupPath, ec);

            if (ec) {
                return ml::ReturnStatus::Failure("Failed to remove oldest backup \"", maxBackupPath.string().c_str(), "\" during backup numbered operation: ", ec.message().c_str());
            }
        }

        // shift all back up folder numbers up by one (starting at the second to last backup)
        for (uint32_t i = depth - 1; i > 0; i--) {
            fs::path backupPath = backupsDest / std::to_string(i);
            fs::path newBackupPath = backupsDest / std::to_string(i + 1);

            if (!ml::isExistPath(backupPath)) {
                continue;
            }

            fs::rename(backupPath, newBackupPath, ec);

            if (ec) {
                return ml::ReturnStatus::Failure("Failed to rename to \"", newBackupPath.string().c_str(), "\" during backup numbered operation: ", ec.message().c_str());
            }
        }
    } else if (!fs::create_directory(backupsDest, ec)) {
        return ml::ReturnStatus::Failure("Failed to create \"", backupsDest.string().c_str(), "\" during backup numbered operation: ", ec.message().c_str());
    }

    fs::path newestBackupPath = backupsDest / std::to_string(1);
    fs::copy_options options = fs::copy_options::recursive | fs::copy_options::overwrite_existing;

    fs::copy(source, newestBackupPath, options, ec);

    return ml::ReturnStatus::FromErrorCode(ec);
}

ml::ReturnValue<uintmax_t> getFileSize(const std::filesystem::path& path) {
    std::error_code ec;
    uintmax_t size = fs::file_size(path, ec);

    return ml::ReturnValue<uintmax_t>::FromErrorCode(ec, size);
}

ml::ReturnValue<std::vector<std::string>> readLines(const std::filesystem::path& path, int maxLines) {
    std::ifstream file(path);
    if (!file) {
        return ml::ReturnValue<std::vector<std::string>>::Failure("Failed to read lines of: " + path.string());
    }

    int count = 0;
    std::string line;
    std::vector<std::string> lines;
    bool isValidLine = maxLines == -1 || (count < maxLines);
    while (isValidLine && std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        count++;
        lines.push_back(std::move(line));
    }

    return ml::ReturnValue<std::vector<std::string>>::Success(lines);
}

ml::ReturnStatus writeLines(const std::filesystem::path& path, const std::vector<std::string>& lines) {
    std::ofstream file(path);
    if (!file) {
        return ml::ReturnStatus::Failure("Failed to open file " + path.string());
    }

    for (const std::string& line : lines) {
        file << line << std::endl;
    }

    return ml::ReturnStatus::Success();
}

}

#endif
