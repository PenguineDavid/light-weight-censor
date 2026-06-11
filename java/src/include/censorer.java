package include;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class censorer {
    private static final Map<String, List<String>> filters = new HashMap<>();

    private static boolean isSpace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    private static boolean isPunct(char c) {
        return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) ||
                (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
    }

    private static boolean wordViolatesFilter(String word, String key) {
        if (!filters.containsKey(key))
            return false;
        return filters.get(key).contains(word);
    }

    private static boolean wordViolatesFilterInsensitive(String word, String key) {
        if (!filters.containsKey(key))
            return false;

        // Check if any element in the list matches, ignoring case mutations
        return filters.get(key).stream()
                .anyMatch(filterWord -> filterWord.equalsIgnoreCase(word));
    }

    public static void setFilter(String key, List<String> content) {
        filters.put(key, content);
    }

    public static void awf(String word, String key) {
        filters.computeIfAbsent(key, k -> new ArrayList<>()).add(word);
    }

    public static void clearFilter(String key) {
        if (filters.containsKey(key)) {
            filters.get(key).clear();
        }
    }

    public static void deleteFilter(String key) {
        filters.remove(key);
    }

    public static String censor(String text, String key) {
        StringBuilder word = new StringBuilder();
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);

            if (isSpace(c) || isPunct(c)) {
                if (word.length() > 0) {
                    String currentWordStr = word.toString();
                    if (wordViolatesFilter(currentWordStr, key)) {
                        result.append("*".repeat(currentWordStr.length()));
                    } else {
                        result.append(currentWordStr);
                    }
                    word.setLength(0);
                }
                result.append(c);
            } else {
                word.append(c);
            }
        }

        if (word.length() > 0) {
            String currentWordStr = word.toString();
            if (wordViolatesFilter(currentWordStr, key)) {
                result.append("*".repeat(currentWordStr.length()));
            } else {
                result.append(currentWordStr);
            }
        }

        return result.toString();
    }

    public static String censorCaseInsensitive(String text, String key) {
        StringBuilder word = new StringBuilder();
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);

            if (isSpace(c) || isPunct(c)) {
                if (word.length() > 0) {
                    String currentWordStr = word.toString();
                    if (wordViolatesFilterInsensitive(currentWordStr, key)) {
                        result.append("*".repeat(currentWordStr.length()));
                    } else {
                        result.append(currentWordStr);
                    }
                    word.setLength(0);
                }
                result.append(c);
            } else {
                word.append(c);
            }
        }

        if (word.length() > 0) {
            String currentWordStr = word.toString();
            if (wordViolatesFilterInsensitive(currentWordStr, key)) {
                result.append("*".repeat(currentWordStr.length()));
            } else {
                result.append(currentWordStr);
            }
        }

        return result.toString();
    }
}
