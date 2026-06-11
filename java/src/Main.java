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

import include.censorer;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        System.out.println("=== Testing Java Censorer Library ===\n");

        // 1. Initialize
        censorer.setFilter("badwords", new ArrayList<>(List.of("apple", "banana")));

        // 2. Append extra word
        censorer.awf("orange", "badwords");

        // 3. Scenario A: Standard Case-Sensitive test
        String rawTextA = "I hate apple and orange!";
        String cleanTextA = censorer.censor(rawTextA, "badwords");
        System.out.println("[Standard Censor]");
        System.out.println("Original: " + rawTextA);
        System.out.println("Censored: " + cleanTextA);
        System.out.println();

        // 4. Scenario B: Case-Insensitive evaluation test
        String rawTextB = "I hate ApPlE, BANANA, and an ORANGE tree!";
        String cleanTextB = censorer.censorCaseInsensitive(rawTextB, "badwords");
        System.out.println("[Case-Insensitive Censor]");
        System.out.println("Original: " + rawTextB);
        System.out.println("Censored: " + cleanTextB);
        System.out.println();

        // 5. Scenario C: Verify standard method fails on uppercase mutations
        String cleanTextC = censorer.censor(rawTextB, "badwords");
        System.out.println("[Standard Censor running on mutated text]");
        System.out.println("Censored: " + cleanTextC);
        System.out.println();

        // 6. Scenario D: Clear & Delete
        System.out.println("--- Clearing Filters ---");
        censorer.clearFilter("badwords");
        System.out.println("--- Deleting Filter Key Completely ---");
        censorer.deleteFilter("badwords");
    }
}
