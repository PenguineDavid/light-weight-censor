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
using include;

public class Program
{
    public static void Main(string[] args)
    {
        Console.WriteLine("=== Testing C# Censorer Library ===\n");

        // 1. Initialize
        Censorer.SetFilter("badwords", new List<string> { "apple", "banana" });

        // 2. Append extra word
        Censorer.Awf("orange", "badwords");

        // 3. Scenario A: Standard Case-Sensitive test
        string rawTextA = "I hate apple and orange!";
        string cleanTextA = Censorer.Censor(rawTextA, "badwords");
        Console.WriteLine("[Standard Censor]");
        Console.WriteLine("Original: " + rawTextA);
        Console.WriteLine("Censored: " + cleanTextA);
        Console.WriteLine();

        // 4. Scenario B: Case-Insensitive evaluation test
        string rawTextB = "I hate ApPlE, BANANA, and an ORANGE tree!";
        string cleanTextB = Censorer.CensorCaseInsensitive(rawTextB, "badwords");
        Console.WriteLine("[Case-Insensitive Censor]");
        Console.WriteLine("Original: " + rawTextB);
        Console.WriteLine("Censored: " + cleanTextB);
        Console.WriteLine();

        // 5. Scenario C: Verify standard method fails on uppercase mutations
        string cleanTextC = Censorer.Censor(rawTextB, "badwords");
        Console.WriteLine("[Standard Censor running on mutated text]");
        Console.WriteLine("Censored: " + cleanTextC);
        Console.WriteLine();

        // 6. Scenario D: Clear & Delete
        Console.WriteLine("--- Clearing Filters ---");
        Censorer.ClearFilter("badwords");
        Console.WriteLine("--- Deleting Filter Key Completely ---");
        Censorer.DeleteFilter("badwords");
    }
}
