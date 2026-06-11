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


using System;
using System.Collections.Generic;
using System.Text;

namespace include
{
    class Censorer
    {
        // ========================
        // private vars and helpers
        // ========================

        private static readonly Dictionary<string, List<string>> filters = new();

        private static bool IsSpace(char c)
        {
            return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        }

        private static bool IsPunct(char c) => char.IsPunctuation(c);

        private static bool WordViolatesFilter(string word, string key)
        {
            if (!filters.ContainsKey(key))
                return false;

            return filters[key].Contains(word);
        }

        private static bool WordViolatesFilterInsensitive(string word, string key)
        {
            if (!filters.ContainsKey(key))
                return false;

            // Convert the target word to lowercase
            string lowerWord = word.ToLowerInvariant();

            // Look for any match in the filter list converted to lowercase
            return filters[key].Exists(w => w.ToLowerInvariant() == lowerWord);
        }

        // ================
        // public functions
        // ================

        public static void SetFilter(string key, List<string> content)
        {
            filters[key] = content;
        }

        // append word, filter
        public static void Awf(string word, string key)
        {
            if (!filters.TryGetValue(key, out List<string>? wordList))
            {
                wordList = new List<string>();
                filters[key] = wordList;
            }

            wordList.Add(word);
        }

        public static void ClearFilter(string key)
        {
            filters[key].Clear();
        }

        public static void DeleteFilter(string key)
        {
            filters.Remove(key);
        }

        public static string Censor(string text, string key)
        {
            StringBuilder word = new();
            StringBuilder result = new();

            for (int i = 0; i < text.Length; i++)
            {
                char c = text[i];

                if (IsSpace(c) || IsPunct(c))
                {
                    if (word.Length > 0) 
                    {
                        string currentWordStr = word.ToString();

                        if (WordViolatesFilter(currentWordStr, key))
                        {
                            result.Append(new string('*', currentWordStr.Length));
                        }
                        else
                        {
                            result.Append(currentWordStr);
                        }
                        word.Length = 0;
                    }

                    result.Append(c);
                }
                else
                {
                    word.Append(c); 
                }
            }

            if (word.Length > 0)
            {
                string currentWordStr = word.ToString();
                if (WordViolatesFilter(currentWordStr, key))
                {
                    for (int i = 0; i < currentWordStr.Length; ++i)
                    {
                        result.Append('*');
                    }
                }
                else
                {
                    result.Append(currentWordStr);
                }
            }

            return result.ToString();
        }

        public static string CensorCaseInsensitive(string text, string key)
        {
            StringBuilder word = new();
            StringBuilder result = new();

            for (int i = 0; i < text.Length; i++)
            {
                char c = text[i];

                if (IsSpace(c) || IsPunct(c))
                {
                    if (word.Length > 0)
                    {
                        string currentWordStr = word.ToString();

                        if (WordViolatesFilterInsensitive(currentWordStr, key))
                        {
                            result.Append(new string('*', currentWordStr.Length));
                        }
                        else
                        {
                            result.Append(currentWordStr);
                        }
                        word.Length = 0;
                    }
                    result.Append(c);
                }
                else
                {
                    word.Append(c);
                }
            }

            if (word.Length > 0)
            {
                string currentWordStr = word.ToString();
                if (WordViolatesFilterInsensitive(currentWordStr, key))
                {
                    result.Append(new string('*', currentWordStr.Length));
                }
                else
                {
                    result.Append(currentWordStr);
                }
            }

            return result.ToString();
        }
    }
}
