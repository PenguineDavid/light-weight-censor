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
