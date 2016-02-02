#ifndef _TWO_WORDS_CORRELATIONS_H
#define _TWO_WORDS_CORRELATIONS_H

#include<vector>
#include<algorithm>
#include "wordPositions.h"

struct TwoWordsCorrelation
{
    const WordPositions* wordA;
    const WordPositions* wordB;
    double correlationAB;
    double correlationBA;
    double correlation;
};

template<class T>
class TwoWordsCorrelations
{
    public:
        std::vector<TwoWordsCorrelation>::const_iterator begin;
        std::vector<TwoWordsCorrelation>::const_iterator end;
        TwoWordsCorrelations(const WordPositionsCollection& collection)
        {
            calculate_words_correlations(collection);
            begin = correlations.begin();
            end = correlations.end();
        }
    private:
        double threshold = 0.01;
        std::vector<TwoWordsCorrelation> correlations;
        T words_binding_measure;
        TwoWordsCorrelation calculate_two_words_correlation(const WordPositions*, const WordPositions*);
        void calculate_words_correlations(const WordPositionsCollection&);
};

template<class T>
void
TwoWordsCorrelations<T>::calculate_words_correlations
    (const WordPositionsCollection& collection)
{
    correlations.clear();

    for (auto iA = collection.begin; iA < (collection.end - 1); ++iA)
    {
        for (auto iB = iA + 1; iB < collection.end; ++iB)
        {
            auto correlation = calculate_two_words_correlation(&(*iA), &(*iB));
            if (correlation.correlation > threshold)
            {
                correlations.push_back(correlation);
            }
        }   
    }

    sort(correlations.begin(), correlations.end(), 
        [](const TwoWordsCorrelation& a, const TwoWordsCorrelation& b)
            { return a.correlation > b.correlation; });
}

template<class T>
TwoWordsCorrelation
TwoWordsCorrelations<T>::calculate_two_words_correlation
    (const WordPositions* wordA, const WordPositions* wordB)
{
   auto itA = wordA -> begin;
   auto itB = wordB -> begin;

   double correlationAB = 0.0;
   double correlationBA = 0.0;

   while (itA < (wordA -> end) && itB < (wordB -> end))
   {
        if (itA -> position < itB -> position)
        {
            if ((itA + 1) -> position > itB -> position)
            {
                auto distance = (itB -> position) - (itA -> position);
                correlationAB += words_binding_measure(distance);
            }

            itA++;
        }    
        else
        {
            if ((itB + 1) -> position > itA -> position)
            {
                auto distance = (itA -> position) - (itB -> position);
                correlationBA += words_binding_measure(distance);
            }

            itB++;
        }
   } 

   double weight = 2.0 * (double)std::min(wordA -> count(), wordB -> count());
   correlationAB /= weight;
   correlationBA /= weight;

   TwoWordsCorrelation result;
   result.wordA = wordA;
   result.wordB = wordB;
   result.correlation = correlationAB + correlationBA;
   result.correlationAB = correlationAB;
   result.correlationBA = correlationBA;

   return result;
}

#endif
