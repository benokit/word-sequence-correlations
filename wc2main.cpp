#include<iostream>
#include<algorithm>
#include<cstdio>
#include "wordPositions.h"
#include "twoWordsCorrelations.h"
#include "optionsParser.h"

struct InverseMeasure
{
    inline double operator () (long distance) const
    {
        return 1.0 / ((double) distance);
    }
};

int main (int argc, char* argv[])
{
    Parameters params = GetParameters(argc, argv);

    auto collection = WordPositionsCollection(params.pathToFileWithWords);

    collection.remove_words_whose_count_is_smaller_than(params.minWordsCount);

    auto correlations = TwoWordsCorrelations<InverseMeasure>(collection);

    for (auto it = correlations.begin; it < correlations.end; ++it)
    {
        std::cout << it -> wordA -> word << ", " 
                  << it -> wordB -> word << ", " 
                  << it -> correlationAB << ", " 
                  << it -> correlationBA << ", " 
                  << it -> correlation << std::endl;
    } 
}

