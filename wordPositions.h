#ifndef _WORD_POSITIONS_H
#define _WORD_POSITIONS_H

#include<vector>
#include<iterator>

struct WordPosition
{
    std::string word;
    int position;
};

struct WordPositions
{
    std::string word;
    std::vector<WordPosition>::const_iterator begin;
    std::vector<WordPosition>::const_iterator end;
    inline long count() const { return (long)distance(begin, end) + 1; }
};

struct WordPositionsCollection
{
    public:
        std::vector<WordPositions>::const_iterator begin;
        std::vector<WordPositions>::const_iterator end;

        WordPositionsCollection(const char* pathToFile);
        void remove_words_whose_count_is_smaller_than(int minWordsCount);
        WordPositions get_positions_for_word(std::string word);
        void remove_word_from_collection(std::string word);

    private:
        std::vector<WordPositions> collection;
        std::vector<WordPosition> words;
        std::vector<WordPosition> read_from_file (const char*);
        std::vector<WordPositions> create_collection (const std::vector<WordPosition>&);
};

#endif
