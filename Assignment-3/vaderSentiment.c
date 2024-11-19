
#include "utility.h"
#include <errno.h>



int containsPunctuationExceptExclamation(const char *str) {
    while (*str != '\0') {
        if (ispunct(*str) && *str != '!') {
            return 1;  // Found punctuation other than '!', return true
        }
        str++;  // Move to the next character
    }
    return 0;  // No punctuation other than '!' found
}

int is_all_caps(const char* word) {
    for (int j = 0; word[j] != '\0'; j++) {
        if (!isupper(word[j])) {
            return 0;  // Return false if any character is not uppercase
        }
    }
    return 1;  // Return true if all characters are uppercase
}


// Reads data from a file and stores it in an array of WordData structs
WordData* read_data(char *filename) {
    FILE *file = fopen(filename, "r");

    // Check if file opened successfully
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate initial memory for WordData
    WordData *data = malloc(sizeof(WordData));
    if (!data) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    size_t line_size = LINE_LENGTH;
    char *line = malloc(line_size);
    int i = 0;

    // Read each line from the file and store it in the WordData array
    while (getline(&line, &line_size, file) != -1) {
        data = realloc(data, (i + 1) * sizeof(WordData));
        sscanf(line, "%s %f %f", data[i].word, &data[i].value1, &data[i].value2);
        i++;
    }

    free(line);
    fclose(file);
    return data;
}

// Searches for a specific word in the WordData array
WordData find_data(WordData *data, char *word) {

    // Loop through data to find the word
    for (int i = 0; data[i].word[0] != '\0'; i++) {
        if (strcmp(data[i].word, word) == 0) {
            return data[i];
        }
    }

    // Return a WordData with an empty word if not found
    WordData nullData;
    nullData.word[0] = '\0';
    return nullData;
}

// Calculates the sentiment score of a sentence based on word data
#include <math.h>

// Modify function to return an array of sentiment scores
float* calculate_sentiment_score(WordData *data, char *sentence) {
    float scores[MAX_STRING_LENGTH] = { 0.0 }; 
    int index = 0;
    int sentimentCount = 0;
    float sentimentSum = 0.0;
    float pos_sum = 0.0;
    float neg_sum = 0.0;
    float neutral = 0.0;
    char sentence_split[MAX_STRING_LENGTH][MAX_STRING_LENGTH];
    char sentence_split_original[MAX_STRING_LENGTH][MAX_STRING_LENGTH];
    char sentence_copy[MAX_STRING_LENGTH];
    int total_exclamations = 0; // Track total exclamations

    strcpy(sentence_copy, sentence);
    char *token = strtok(sentence_copy, " \n\t\v\f\r,.?");

    int isNegated = 0; // Initialize negation flag
    for (; token != NULL; index++) {

        bool allCaps = true;
        int exclamation = 0;
        strcpy(sentence_split_original[index], token);
        char lowerToken[MAX_STRING_LENGTH];
        strcpy(lowerToken, token);

        for (int i = 0; lowerToken[i] != '\0'; i++) {
            if (islower(lowerToken[i])) allCaps = false;
            lowerToken[i] = tolower(lowerToken[i]);

            if (lowerToken[i] == '!') {
                exclamation++;
                lowerToken[i] = '\0';
            }
        }

        total_exclamations += exclamation;
        strcpy(sentence_split[index], lowerToken);
        WordData wordData = find_data(data, lowerToken);

        // Check for negation words and set negation flag
        for (int i = 0; i < NEGATIONS_SIZE; i++) {
            if (strcmp(lowerToken, negation_words[i]) == 0) {
                isNegated = 1;
                break;
            }
        }

        // Check for neutral words; only reset negation on actual sentiment words
        if (strcmp(lowerToken, "and") == 0 || strcmp(lowerToken, ",") == 0 || strcmp(lowerToken, "a") == 0) {
            token = strtok(NULL, " \n\t\v\f\r,.?");
            continue;
        }

        // If relevant word found, apply sentiment score
        if (wordData.word[0] != '\0') {
            sentimentCount++;
            scores[index] = wordData.value1;

            // Apply CAPS multiplier
            if (allCaps && !containsPunctuationExceptExclamation(sentence_split[index])) {
                scores[index] *= CAPS;
            }

            // Apply negation if flagged
            if (isNegated) {
                scores[index] *= NEGATION;
                // Do not reset `isNegated` here; allow it to persist to the next word in this phrase
            }

            if (index > 0) {
                float intensifier_multiplier = INTENSIFIER;
                for (int i = 0; i < POSITIVE_INTENSIFIERS_SIZE; i++) {
                    if (strcmp(sentence_split[index - 1], positive_intensifiers[i]) == 0) {
                        if (is_all_caps(sentence_split_original[index - 1])) {
                            intensifier_multiplier *= CAPS;
                        }
                        scores[index] += scores[index] * intensifier_multiplier;
                    }
                }

                for (int i = 0; i < NEGATIVE_INTENSIFIERS_SIZE; i++) {
                    if (strcmp(sentence_split[index - 1], negative_intensifiers[i]) == 0) {
                        if (is_all_caps(sentence_split_original[index - 1])) {
                            intensifier_multiplier *= CAPS;
                        }
                        scores[index] -= scores[index] * intensifier_multiplier;
                    }
                }
            }


            // Add score to positive, negative, or neutral sums
            if (scores[index] > 0) {
                pos_sum += scores[index];
            } else if (scores[index] < 0) {
                neg_sum += fabs(scores[index]);
            } else {
                neutral += 1;
            }

            sentimentSum += scores[index];
        } else {
            neutral += 1;
        }
        token = strtok(NULL, " \n\t\v\f\r,.?");
    }

    // Adjust final sentimentSum for exclamations
    if (sentimentSum > 0) {
        sentimentSum += total_exclamations * EXCLAMATION;
    } else if (sentimentSum < 0) {
        sentimentSum -= total_exclamations * EXCLAMATION;
    }

    float pos_percent = pos_sum / (pos_sum + neg_sum + neutral);
    float neg_percent = neg_sum / (pos_sum + neg_sum + neutral);
    float neu_percent = neutral / (pos_sum + neg_sum + neutral);
    float compound = sentimentSum / sqrt(pow(sentimentSum, 2) + 15);

    // Allocate array for returning the scores
    float *result = malloc(4 * sizeof(float));
    if (!result) {
        perror("Memory allocation failed");
        return NULL;
    }

    result[0] = pos_percent;
    result[1] = neg_percent;
    result[2] = neu_percent;
    result[3] = compound;

    return result;
}




