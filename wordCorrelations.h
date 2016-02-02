#include<vector>
#include<algorithm>

#ifndef _WORD_CORRELATIONS_H
#define _WORD_CORRELATIONS_H

using namespace std;

struct TwoWordsCorrelation
{
    const WordPositions* wordA;
    const WordPositions* wordB;
    double correlation;
};

template<class T>
class WordsCorrelations
{
    public:
        vector<TwoWordsCorrelation>::const_iterator begin;
        vector<TwoWordsCorrelation>::const_iterator end;
        WordsCorrelations(const WordPositionsCollection& collection)
        {
            calculate_words_correlations(collection);
            begin = correlations.begin();
            end = correlations.end();
        }
    private:
        double threshold = 0.01;
        vector<TwoWordsCorrelation> correlations;
        T words_binding_measure;
        TwoWordsCorrelation calculate_two_words_correlation(const WordPositions*, const WordPositions*);
        void calculate_words_correlations(const WordPositionsCollection&);
};

template<class T>
void
WordsCorrelations<T>::calculate_words_correlations
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
WordsCorrelations<T>::calculate_two_words_correlation
    (const WordPositions* wordA, const WordPositions* wordB)
{
   auto itA = wordA -> begin;
   auto itB = wordB -> begin;

   double correlation = 0.0;

   while (itA < (wordA -> end - 1) && itB < (wordB -> end - 1))
   {
        if (itA -> position < itB -> position)
        {
            if ((itA + 1) -> position > itB -> position)
            {
                auto distance = (itB -> position) - (itA -> position);
                correlation += words_binding_measure(distance);
            }

            itA++;
        }    
        else
        {
            if ((itB + 1) -> position > itA -> position)
            {
                auto distance = (itA -> position) - (itB -> position);
                correlation += words_binding_measure(distance);
            }

            itB++;
        }
   } 

   double weight = 2.0 * (double)min(wordA -> count(), wordB -> count());
   correlation /= weight;

   TwoWordsCorrelation result;
   result.wordA = wordA;
   result.wordB = wordB;
   result.correlation = correlation;

   return result;
}

#endif
