#include "utility.h"

int main() {
    WordData *data = read_data("vader_lexicon.txt");

    // Define an array of sentences
    char *sentences[] = {
        "VADER is smart, handsome, and funny.",
        "VADER is smart, handsome, and funny!",
        "VADER is very smart, handsome, and funny.",
        "VADER is VERY SMART, handsome, and FUNNY",
        "VADER is VERY SMART, handsome, and FUNNY!!!",
        "VADER is VERY SMART, uber handsome, and FRIGGIN FUNNY!!!",
        "VADER is not smart, handsome, nor funny.",
        "At least it isn’t a horrible book.",
        "The plot was good, but the characters are uncompelling and the dialog is not great.",
        "Make sure you :) or :D today!",
		"Not bad at all"
    };

    
    // Calculate the number of sentences
    int num_sentences = sizeof(sentences) / sizeof(sentences[0]);

    // Iterate over each sentence and calculate the sentiment score
    for (int i = 0; i < num_sentences; i++) {
        printf("Sentence: %s\n", sentences[i]);
        float *scores = calculate_sentiment_score(data, sentences[i]);
        printf("Sentiment Score: (Positive: %f, Negative: %f, Neutral: %f, Compound: %f)\n\n", scores[0], scores[1], scores[2], scores[3]);
        free(scores);
    }

    // Free the allocated data
    free(data);
}

