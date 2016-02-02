#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include "wordPositions.h" 

std::vector<WordPosition> 
WordPositionsCollection::read_from_file (const char* path)
{
    std::vector<WordPosition> wordPositions;
    wordPositions.clear();

    std::ifstream file (path);
    std::string word;
    WordPosition wordPosition;
    wordPosition.position = 0;

    while (file >> word) 
    {
        wordPosition.word = word;
        wordPosition.position++;
        
        wordPositions.push_back(wordPosition);
    } 

    file.close();

    std::stable_sort(wordPositions.begin(), wordPositions.end(), 
            [](const WordPosition& a, const WordPosition& b){ return a.word < b.word; });

    return wordPositions;
}

std::vector<WordPositions> 
WordPositionsCollection::create_collection (const std::vector<WordPosition>& words)
{
    std::vector<WordPositions> collection;
    collection.clear();

    WordPositions wordDynamics;

    bool isNewWord = true;
    std::vector<WordPosition>::const_iterator it = words.begin();
    while (true)
    {
        if (isNewWord)
        {
            wordDynamics.word = it -> word;
            wordDynamics.begin = it;
            isNewWord = false;
        }

        it++;

        if (it == words.end())
        {
            wordDynamics.end = it - 1;
            collection.push_back (wordDynamics);
            break;
        }

        isNewWord = it -> word != wordDynamics.word;

        if (isNewWord)
        {
            wordDynamics.end = it - 1;
            collection.push_back (wordDynamics);
        }
    }

    return collection;
}

void
WordPositionsCollection::remove_words_whose_count_is_smaller_than(int minWordsCount)
{
    if (minWordsCount <= 1)
    {
        end = collection.end();
    }
    else
    {
        end = std::remove_if(collection.begin(), collection.end(), 
                [minWordsCount](WordPositions& a)
                    { return a.count() < minWordsCount; });
    }
}

WordPositions
WordPositionsCollection::get_positions_for_word(std::string word)
{
    auto iw = find_if(collection.begin(), collection.end(), 
            [word](const WordPositions& a){ return a.word == word; });

    if (iw == collection.end())
    {
        std::cerr << "Error: word " << word << "does not exist!" << std::endl;
        throw std::invalid_argument("The input word does not exist.");
    }

    return *iw;
}

void
WordPositionsCollection::remove_word_from_collection(std::string word)
{
    auto length = std::distance(begin, end);
    end = std::remove_if(collection.begin(), collection.begin() + length, 
        [word](WordPositions& a) { return a.word == word; });
}

WordPositionsCollection::WordPositionsCollection(const char* pathToFile)
{
    words = read_from_file(pathToFile);
    collection = create_collection(words);
    begin = collection.begin();
    end = collection.end();
}
