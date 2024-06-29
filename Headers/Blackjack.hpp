#pragma once

#include "OCCF.hpp"

#include <algorithm>
#include <random>

enum class SaveData
{
    HandsPlayed,
    HandsWon,


    All
};

enum class DetailLevel
{
    Minimal, // Only shows Numbers
    Mini, // Small Cards
    Full // Full size cards
};

class Blackjack
{
private:

    unsigned int Money;
    unsigned int Decks = 6;
    bool Verified = false;
    DetailLevel Detail_Level = DetailLevel::Full;

    int UniversalInput;
    void Menu();
    void Betting();
    void Table(const int bet);

public:

    Blackjack();
    Blackjack(const char* card_data_file_path,const char* save_data_file_path);
    
    int getDeckCount();

    void setDeckCount(const unsigned int decks);
    void setDetailLevel(const DetailLevel detail_level);

    void resetStats(std::vector<SaveData> stats);

    void Play();

};