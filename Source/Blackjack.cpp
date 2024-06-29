#include "Blackjack.hpp"

/*
To Do:
    detect if player bust (pause) during play 
    detect if player has a natural

    render function
        full
        mini
*/

namespace
{

    inline void clearConsole()
    {std::cout << "\033[2J\033[H";
    }

    inline void setCursor(const int x, const int y)
    {std::cout << "\033[" << y << ";" << x << "H";
    }

    void fillLine(std::string& line)
    {
        const size_t DIFFERNCE = 78 - line.length();
        line.append(DIFFERNCE,' ');
        line += "[]\n";
    }

    bool getInput(int& input,const int max_value = INT_MAX,const int min_value = 0)
    {
        int Input;
        std::cin >> Input;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            return false;
        }

        else if (Input < min_value || Input > max_value)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            return false;
        }

        input = Input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        return true;
    }

    struct Hand
    {
        static int DeckIndex;

        std::vector<std::string> &GameDeck;
        std::vector<std::string> Display;
        std::vector<std::string> Cards;
        DetailLevel Detail_Level;
        bool isPlayer = false;              // Rendering purposes
        int Value = 0;
        int TrackedAces = 0;

        Hand(std::vector<std::string>& d,DetailLevel dl, bool p) : GameDeck(d), Detail_Level(dl), isPlayer(p){}

        void drawCard()
        {
            std::string Card = GameDeck[DeckIndex];
            DeckIndex++;

            Card.pop_back();                            // Remove the suit
            Cards.push_back(Card);

            /* Calculate Hand Value */
            try
            {Value += std::stoi(Card);
            }
            catch(const std::invalid_argument&)
            {
                Value += 10;
                if (Card[0] == 'A')
                {
                    Value += 1;
                    TrackedAces++;
                }
            }
            
            if (Value > 21 && TrackedAces > 0)                 // Demote Ace
            {
                Value -= 10;
                TrackedAces--;
            }
        }

        void renderCards()
        {
            Display.clear();
            switch (Detail_Level)
            {
            case DetailLevel::Full:return;
            case DetailLevel::Mini:return;
            case DetailLevel::Minimal:
            
                std::string Line;

                if (isPlayer)
                {Line = "[]     Your Hand : " + std::to_string(Value);
                }
                else
                {Line = "[]     Dealer's Hand : " + std::to_string(Value);
                }
                fillLine(Line);

                Display.push_back(Line);

                return;
            }
        }

    };
    int Hand::DeckIndex = 0;
    
}

// static void _renderCards(const std::vector<std::string>& hand,std::vector<std::string>& display,const DetailLevel detail_level,const bool p)
// {
//     switch (detail_level)
//     {
//     case DetailLevel::Full:



//         return;
//     case DetailLevel::Mini:
//         return;

//     case DetailLevel::Minimal:

//         std::string Line;

//         if (p)
//         {Line = "[]     Your Hand : " + std::to_string(VALUE);
//         }
//         else
//         {Line = "[]     Dealer's Hand : " + std::to_string(VALUE);
//         }

//         return;
//     }
// }



void Blackjack::Menu()
{
    std::string HeaderLine = "[]     MENU |$$$ : " + std::to_string(Money) + "|";
    fillLine(HeaderLine);

    do
    {
        clearConsole();
        std::cout 
        << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n" 
        << "[]                                                                            []\n" 
        <<  HeaderLine
        << "[]                                                                            []\n" 
        << "[]     0. Quit    2. View Settings    4. View Stats    6. Rulebook            []\n" 
        << "[]     1. Play    3. Edit Settings    5. Reset Stats                          []\n" 
        << "[]                                                                            []\n" 
        << "[]     Input : _                                                              []\n" 
        << "[]                                                                            []\n" 
        << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n";
        setCursor(16,8);
    } while (!getInput(UniversalInput,6));
}
void Blackjack::Betting()
{
    std::string HeaderLine = "[]     BETTING |$$$ : " + std::to_string(Money) + "|";
    fillLine(HeaderLine);

    do
    {
        clearConsole();
        std::cout 
        << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n" 
        << "[]                                                                            []\n"
        <<  HeaderLine
        << "[]                                                                            []\n" 
        << "[]     0. Go Back                                                             []\n" 
        << "[]                                                                            []\n"
        << "[]     Input Bet : _                                                          []\n" 
        << "[]                                                                            []\n" 
        << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n";
        setCursor(20,7);
    } while (!getInput(UniversalInput,Money));
}
void Blackjack::Table(const int bet)
{
    std::string HeaderLine = "[]     TABLE  |$$$ : " + std::to_string(Money) + " | Bet : " + std::to_string(bet) + "|";
    fillLine(HeaderLine);

    /* Create Deck, Resize Deck, Shuffle Deck */
    std::vector<std::string> BaseDeck = {
    "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
    "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD",
    "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
    "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS"
    };

    std::vector<std::string> Deck;
    for (int i = 0; i < Decks; i++)
    {Deck.insert(Deck.end(),BaseDeck.begin(),BaseDeck.end());}

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(Deck.begin(),Deck.end(),g);

    /* Create Hand, Draw cards in the correct order */
    Hand Dealer(Deck,Detail_Level,false);
    Hand Player(Deck,Detail_Level,true);

    Player.drawCard();
    Dealer.drawCard();
    Player.drawCard();

    Dealer.Cards.push_back("B_");       // B_ is the unidentified flipped card for rendering purposes
    Dealer.renderCards();               // Render the dealer's cards onto the Display Vector
    Dealer.Cards.pop_back();            // Remove the filler card 
    Dealer.drawCard();                  // Add the dealer's 2nd card to its hand


    const bool NATURAL = (Player.Value == 21);
    bool PlayerBust = false;
    bool DealerBust = false;
    if (!NATURAL)
    {
        /* Player's Play */
        do
        {
            Player.renderCards();
            do
            {
                clearConsole();
    std::cout   << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n"
                << "[]                                                                            []\n"
                <<  HeaderLine
                << "[]                                                                            []\n";
                for (std::string& Line : Dealer.Display) {std::cout << Line;}  
    std::cout   << "[]                                                                            []\n";
                for (std::string& Line : Player.Display) {std::cout << Line;}
    std::cout   << "[]                                                                            []\n"
                << "[]     1. Hit                                                                 []\n"
                << "[]     2. Stand                                                               []\n"
                << "[]                                                                            []\n"
                << "[]     Input : _                                                              []\n"
                << "[]                                                                            []\n"
                << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n";
                const int Y = 10 + Dealer.Display.size() + Player.Display.size();
                setCursor(16,Y);
            } while (!getInput(UniversalInput,2,1));        // If Bad Input Redraw and wait for new input

            if (UniversalInput == 1)
            {
                Player.drawCard();
                PlayerBust = (Player.Value > 21);
            }
            else 
            {break;
            }

        } while (!PlayerBust);

        /* Dealer's Play */

        if (!PlayerBust)
        {
            while (Dealer.Value < 17)
            {Dealer.drawCard();  
            }
            DealerBust = (Dealer.Value > 21);
        }
    }

    /* Result Conditions, Payout Calculation */

    const bool DRAW_CONDITION = (Player.Value == Dealer.Value) && !PlayerBust;
    const bool WIN_CONDITION = (   
        (!PlayerBust && !DRAW_CONDITION && Player.Value > Dealer.Value && Dealer.Value <= 21)
        ||
        (DealerBust && !PlayerBust)
    );

    const float PAYOUT_MULTIPLIER = (NATURAL) ? 1.5 : 2.0;
    int Payout;

    if (WIN_CONDITION || NATURAL)
    {Payout = static_cast<int>(bet * PAYOUT_MULTIPLIER);
    }
    else if (DRAW_CONDITION)
    {Payout = bet;
    }
    else
    {Payout = 0;
    }

    Money += Payout;

    /* Result Screen */

    HeaderLine = "[]     TABLE  |$$$ : " + std::to_string(Money) + " | Bet : " + std::to_string(bet) + "|";
    fillLine(HeaderLine);

    std::string ResultLine = "[]     You Won : $" + std::to_string(Payout);
    fillLine(ResultLine);

    Player.renderCards();       // Re-render in case of Bust
    Dealer.renderCards();

    do
    {
            clearConsole(); std::cout
            << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n"
            << "[]                                                                            []\n"
            <<  HeaderLine
            << "[]                                                                            []\n"; 
            for (std::string& Line : Dealer.Display) {std::cout << Line;}   std::cout
            << "[]                                                                            []\n"; 
            for (std::string& Line : Player.Display) {std::cout << Line;}   std::cout
            << "[]                                                                            []\n"
            <<  ResultLine
            << "[]                                                                            []\n"
            << "[]     0. Quit                                                                []\n"
            << "[]     1. Play Again    2. Menu                                               []\n"
            << "[]                                                                            []\n"
            << "[]     Input : _                                                              []\n"
            << "[]                                                                            []\n"
            << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n";
            const int Y = 12 + Dealer.Display.size() + Player.Display.size();
            setCursor(16,Y);
    } while (!getInput(UniversalInput,2));
}

Blackjack::Blackjack(){}
Blackjack::Blackjack(const char* card_data_file_path,const char* save_data_file_path){}

int Blackjack::getDeckCount()
{return Decks;
}

void Blackjack::setDeckCount(const unsigned int decks)
{
    if (decks < 1 || decks > 8)
    {throw std::out_of_range("Deck Count input is out of range [1-8]");}
    Decks = decks;
}
void Blackjack::setDetailLevel(const DetailLevel detail_level)
{Detail_Level = detail_level;
}

void Blackjack::resetStats(std::vector<SaveData> stats)
{

}

void Blackjack::Play()
{


    do
    {
        Menu();

        PlayAgain:                                      

        switch (UniversalInput)
        {
            case 1:                                         // Play
            { 
                Betting();

                if (UniversalInput == 0){break;}
                
                const int BET = UniversalInput;
                Money -= BET;

                Table(BET);

                if (UniversalInput == 1)
                {goto PlayAgain;
                }
                else if (UniversalInput == 2)
                {break;
                }
                
                std::cout << "\n\n";
                return;
            }
            default: 
            {
                std::cout << "\n\n";
                return;
            }
        }
    } while (true); 
}