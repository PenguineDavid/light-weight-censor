/*
 * Copyright (C) 2026 David S
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html#SEC1>.
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
