#include "kre.c"

// link methods
bool kre_is_match(struct KREItem* item, char character);

// local methods
bool kre_match_is_whitespace(char character);
bool kre_match_is_alphanumeric(char character);
bool kre_match_is_digit(char character);
bool kre_match_set(String* set, char character);

// implement methods
bool kre_match_is_whitespace(char character) {
    if (
        character == '\t' ||
        character == '\f' ||
        character == '\r' ||
        character == '\n' ||
        character == '\v' ||
        character == ' ') {
        return true;
    }

    return false;
}
bool kre_match_is_alphanumeric(char character) {
    if (
        (character >= 'a' && character <= 'z') ||
        (character >= 'A' && character <= 'Z') ||
        (character >= '0' && character <= '9') ||
        character == '_') {
        return true;
    }

    return false;
}
bool kre_match_is_digit(char character) {
    if (
        (character >= '0' && character <= '9')) {
        return true;
    }

    return false;
}
bool kre_match_set(String* set, char character) {
    // detect type of set
    bool is_not = set->vtable->value(set)[1] == '^';

    // iterate set characters and match with character
    for (int cursor = (is_not ? 2 : 1); cursor < set->vtable->length(set) - 1; cursor++) {
        if (set->vtable->value(set)[cursor] == '-') {
            // range type

            // return if character was in range
            if (
                character >= set->vtable->value(set)[cursor - 1] &&
                character <= set->vtable->value(set)[cursor + 1]) {
                return !is_not;
            }
        } else if (set->vtable->value(set)[cursor] == '\\') {
            // special character
            cursor++;
            char meta_character = set->vtable->value(set)[cursor];

            // return if character matches with this part of set
            if (
                (meta_character == 't' && character == '\t') ||
                (meta_character == 'f' && character == '\f') ||
                (meta_character == 'r' && character == '\r') ||
                (meta_character == 'n' && character == '\n') ||
                (meta_character == 'v' && character == '\v') ||
                (meta_character == character)) {
                return !is_not;
            }
        } else {
            // normal character

            // return if character matches with this
            if (set->vtable->value(set)[cursor] == character) {
                return !is_not;
            }
        }
    }

    return is_not;
}

// kre base matcher
bool kre_is_match(struct KREItem* item, char character) {
    // check item type is match
    if (item->type != KRE_ITEM_MATCH) {
        return false;
    }

    // match character
    char first_match = item->value.match.match->vtable->value(item->value.match.match)[0];
    char second_match = item->value.match.match->vtable->value(item->value.match.match)[1];
    if (first_match == '.') {
        // dot type (any character)
        return true;
    } else if (first_match == '\\') {
        if (second_match == 's') {
            // Whitespace, \t \f \r \n \v and spaces
            return kre_match_is_whitespace(character);
        } else if (second_match == 'S') {
            // Non-whitespace
            return !kre_match_is_whitespace(character);
        } else if (second_match == 'w') {
            // Alphanumeric, [a-zA-Z0-9_]
            return kre_match_is_alphanumeric(character);
        } else if (second_match == 'W') {
            // Non-alphanumeric
            return !kre_match_is_alphanumeric(character);
        } else if (second_match == 'd') {
            // Digits, [0-9]
            return kre_match_is_digit(character);
        } else if (second_match == 'D') {
            // Non-digits
            return !kre_match_is_digit(character);
        }
    } else if (first_match == '[') {
        // set type
        return kre_match_set(item->value.match.match, character);
    } else {
        // character type
        return first_match == character;
    }

    // return false when there were no conditions
    return false;
}