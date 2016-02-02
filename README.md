# word-sequence-correlations
Calculate 2 and 3 words correlations in a sequence of words

Usage:

first:

$./make

two words correlations:

$./wc2 -o 100 input_path_to_file_with_words > output_result

input_path_to_file_with_words : each line is interpreted as a word, line number is a sequence number of a word

output_result : CSV : wordA, wordB, ordered_correlationAB, odered_correaltionBA, order_agnostic_correlation

