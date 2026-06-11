/*
 * Copyright (C) 2026 David S
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "censorer.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace cen
{

    // ========================
    // private vars and helpers
    // ========================

    static std::unordered_map<std::string, std::vector<std::string>> filters;

    static bool is_space(char c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    static bool is_punct(char c)
    {
        return (c >= 33 && c <= 47) || // ! " # $ % & ' ( ) * + , - . /
               (c >= 58 && c <= 64) || // : ; < = > ? @
               (c >= 91 && c <= 96) || // [ \ ] ^ _ `
               (c >= 123 && c <= 126); // { | } ~
    }

    static std::string to_lower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return s;
    }

    // ================
    // public functions
    // ================

    void set_filter(std::string key, std::vector<std::string> content)
    {
        filters[key] = content;
    }

    // append word, filter
    void awf(std::string word, std::string key)
    {
        filters[key].push_back(word);
    }

    void clear_filter(std::string key)
    {
        filters[key].clear();
    }

    void delete_filter(std::string key)
    {
        filters.erase(key);
    }

    std::string censor(std::string text, std::string key)
    {
        std::string word = "";
        std::string result = "";

        for (char c : text)
        {
            // If we hit a space OR a punctuation mark, the current word is finished
            if (is_space(c) || is_punct(c))
            {
                if (!word.empty())
                {
                    // Check if the finished word is a bad word
                    if (std::find(filters[key].begin(), filters[key].end(), word) != filters[key].end())
                    {
                        result.append(word.length(), '*');
                    }
                    else
                    {
                        result += word;
                    }
                    word = ""; // Clear the buffer for the next word
                }

                // Now safely append the space or punctuation mark in its correct order
                result.push_back(c);
            }
            else
            {
                // It's a normal letter, keep building the current word
                word += c;
            }
        }

        // Cleanup: Handle the very last word if the string didn't end in a space/punctuation
        if (!word.empty())
        {
            if (std::find(filters[key].begin(), filters[key].end(), word) != filters[key].end())
            {
                for (size_t i = 0; i < word.length(); ++i)
                {
                    result.push_back('*');
                }
            }
            else
            {
                result += word;
            }
        }

        return result;
    }

    std::string censor_case_insensitive(std::string text, std::string key)
    {
        std::string word = "";
        std::string result = "";

        // Build a temporary list of completely lowercased bad words for lookup
        std::vector<std::string> lower_filters;
        for (const auto &w : filters[key])
        {
            lower_filters.push_back(to_lower(w));
        }

        for (char c : text)
        {
            if (is_space(c) || is_punct(c))
            {
                if (!word.empty())
                {
                    std::string lower_word = to_lower(word);
                    if (std::find(lower_filters.begin(), lower_filters.end(), lower_word) != lower_filters.end())
                    {
                        result.append(word.length(), '*');
                    }
                    else
                    {
                        result += word;
                    }
                    word = "";
                }
                result.push_back(c);
            }
            else
            {
                word += c;
            }
        }

        if (!word.empty())
        {
            std::string lower_word = to_lower(word);
            if (std::find(lower_filters.begin(), lower_filters.end(), lower_word) != lower_filters.end())
            {
                result.append(word.length(), '*');
            }
            else
            {
                result += word;
            }
        }

        return result;
    }
}