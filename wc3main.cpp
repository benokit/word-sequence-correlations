#include<iostream>
#include<algorithm>
#include<cstdio>
#include "wordPositions.h"
#include "threeWordsCorrelations.h"
#include "optionsParser.h"

using namespace std;

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

    auto correlations = ThreeWordsCorrelations<InverseMeasure>(collection, params.firstWord);

    for (auto it = correlations.begin; it < correlations.end; ++it)
    {
        cout << it -> wordA -> word << ", " << it -> wordB -> word << ", " << it -> wordC -> word << ", " << it -> correlation << endl;
    } 
}

