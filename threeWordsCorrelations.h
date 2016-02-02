#ifndef _THREE_WORDS_CORRELATIONS_H
#define _THREE_WORDS_CORRELATIONS_H

#include<vector>
#include<array>
#include<algorithm>
#include<iterator>

struct WordIterator
{
    std::vector<WordPosition>::const_iterator current;
    std::vector<WordPosition>::const_iterator end;   
};

struct ThreeWordsCorrelation
{
    const WordPositions* wordA;
    const WordPositions* wordB;
    const WordPositions* wordC;
    double correlation;
};

template<class T>
class ThreeWordsCorrelations
{
    public:
        std::vector<ThreeWordsCorrelation>::const_iterator begin;
        std::vector<ThreeWordsCorrelation>::const_iterator end;
        ThreeWordsCorrelations(WordPositionsCollection& collection, std::string wordA)
        {
            wpA = collection.get_positions_for_word(wordA);
            collection.remove_word_from_collection(wordA);
            calculate_three_words_correlations(collection, wpA);
            begin = correlations.begin();
            end = correlations.end();
        }
    private:
        WordPositions wpA;
        double threshold = 0.05;
        std::vector<ThreeWordsCorrelation> correlations;
        T words_binding_measure;
        void calculate_three_words_correlations(const WordPositionsCollection&, const WordPositions& wpA);
        inline ThreeWordsCorrelation calculate_three_words_correlation(const WordPositions*, const WordPositions*, const WordPositions*) const;
        inline bool are_ordered_words_then_calculate_correlation(WordIterator* const a, WordIterator* const b, WordIterator* const c, double* const correlation) const;
        inline bool any_word_triplets_left(const WordIterator& a, const WordIterator& b, const WordIterator& c) const;
};

template<class T>
void
ThreeWordsCorrelations<T>::calculate_three_words_correlations
    (const WordPositionsCollection& collection, const WordPositions& wpA)
{
    correlations.clear();

    for (auto itB = collection.begin; itB < collection.end - 1; ++itB)
    {
        for (auto itC = itB + 1; itC < collection.end; ++itC)
        {
            auto correlation = calculate_three_words_correlation(&wpA, &(*itB), &(*itC));

            if (correlation.correlation > threshold)
            {
                correlations.push_back(correlation);
            }
        }   
    }

    sort(correlations.begin(), correlations.end(), 
        [](const ThreeWordsCorrelation& a, const ThreeWordsCorrelation& b)
            { return a.correlation > b.correlation; });
}

template<class T>
inline ThreeWordsCorrelation
ThreeWordsCorrelations<T>::calculate_three_words_correlation
    (const WordPositions* wordA, const WordPositions* wordB, const WordPositions* wordC) const
{
    WordIterator wA = {wordA -> begin, wordA -> end};
    WordIterator wB = {wordB -> begin, wordB -> end};
    WordIterator wC = {wordC -> begin, wordC -> end};

    double correlation = 0.0;

    while (any_word_triplets_left(wA, wB, wC))
    {
        if (are_ordered_words_then_calculate_correlation(&wA, &wB, &wC, &correlation))
        {
            continue;
        }
        
        if (are_ordered_words_then_calculate_correlation(&wA, &wC, &wB, &correlation))
        {
            continue;
        }

        if (are_ordered_words_then_calculate_correlation(&wC, &wA, &wB, &correlation))
        {
            continue;
        }

        if (are_ordered_words_then_calculate_correlation(&wC, &wB, &wA, &correlation))
        {
            continue;
        }

        if (are_ordered_words_then_calculate_correlation(&wB, &wC, &wA, &correlation))
        {
            continue;
        }

        if (are_ordered_words_then_calculate_correlation(&wB, &wA, &wC, &correlation))
        {
            continue;
        }
   } 

   double weight = 2.0 * (double)std::min({wordA -> count(), wordB -> count(), wordC -> count()});
   correlation /= weight;

   ThreeWordsCorrelation result;
   result.wordA = wordA;
   result.wordB = wordB;
   result.wordC = wordC;
   result.correlation = correlation;

   return result;
}

template<class T>
inline bool
ThreeWordsCorrelations<T>::any_word_triplets_left
    (const WordIterator& a, const WordIterator& b, const WordIterator& c) const
{
    return (a.current <= a.end) && (b.current <= b.end) && (c.current <= c.end);
}

template<class T>
inline bool
ThreeWordsCorrelations<T>::are_ordered_words_then_calculate_correlation
    (WordIterator* const a, WordIterator* const b, WordIterator* const c, double* const correlation) const
{
    if (a -> current -> position > b -> current -> position || 
        b -> current -> position > c -> current -> position ||
        a -> current -> position > c -> current -> position)
    {
        return false;
    }

    while (b -> current -> position < c -> current -> position &&
           b -> current <= b -> end)
    {
        (b -> current)++;
    }
    (b -> current)--;

    while (a -> current -> position < b -> current -> position &&
           a -> current <= a -> end)
    {
        (a -> current)++;
    }
    (a -> current)--;

    if (((b -> current) + 1) -> position > c -> current -> position &&
        ((a -> current) + 1) -> position > c -> current -> position)
    {
        *correlation += words_binding_measure((b -> current -> position) - (a -> current -> position))
                      * words_binding_measure((c -> current -> position) - (b -> current -> position));
    }

    (a -> current)++;

    return true;
}

#endif

