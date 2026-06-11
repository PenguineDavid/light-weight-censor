#ifndef CENSORER_HPP
#define CENSORER_HPP

#include <vector>
#include <string>

namespace cen
{
    void set_filter(std::string key, std::vector<std::string> content);
    void awf(std::string word, std::string key);
    void clear_filter(std::string key);
    void delete_filter(std::string key);
    std::string censor(std::string text, std::string key);
    std::string censor_case_insensitive(std::string text, std::string key);
}

#endif
