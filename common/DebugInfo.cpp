#pragma once
#include <string>

/**
 * @brief Get section string
 * @param title     Section title(name)
 * @param breakLine Break line to show the title
 * @return Section string
 */
std::string section(const std::string& title, bool breakLine = true) {
    std::string str;
    if (breakLine) {
        str += "\n";
    }
    if (title.empty()) {
        return str + std::string(120, '=');
    }

    std::string fillStr((120 - title.size()) / 2, '=');
    return str + fillStr + " " + title + " " + fillStr;
}

/**
 * @brief Get sub section string
 * @param title     Sub section title(name)
 * @param breakLine Break line to show the title
 * @return Sub section string
 */
std::string subSection(const std::string& title, bool breakLine = true) {
    std::string str;
    if (breakLine) {
        str += "\n";
    }
    if (title.empty()) {
        return str + std::string(120, '-');
    }

    std::string fillStr((120 - title.size()) / 2, '-');
    return str + fillStr + " " + title + " " + fillStr;
}
