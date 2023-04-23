/* 
 * File:   War.cpp
 * Author: Andrew Spurling
 * Date:   4/23/23
 */

#include "War.h"
#include "Card.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <deque>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <random>
#include <list>

// Maximum number of rounds to prevent infinite loops
const int War::MAX_ROUNDS = 1000;

// Declare static variables for player names, hands, etc
std::string War::nameP1;
std::string War::nameP2;
Hand War::player1;
Hand War::player2;
bool War::skipToEnd = false;
int War::rounds = 1;
std::set<std::string> War::uniqueCardsPlayed;
std::map<std::string, int> War::cardFrequency;


// Default constructor (empty)
War::War() {}

// Main function to play the game
void War::play()
{
    // Get player names and print introduction
    getNames();
    introduction();
    Hand initialDeck = Hand::fullDeck();

    // Play game until the user decides to quit
    do
    {
        // Reset variables for a new game
        rounds = 1;
        skipToEnd = false;
        
        // Reset hands to initial state
        resetHands(initialDeck, player1, player2);
        
        // Init the deck and player hands
        std::deque<Card> deck = Hand::fullDeck().getCards();
        std::default_random_engine();

        // Compare the cards until one player runs out of cards
        while (!player1.empty() && !player2.empty())
        {
            battle();
            
            // If the game goes on too long, break to prevent infinite loops
            if (++rounds > MAX_ROUNDS)
            {
                std::cout << "This game of War appears to be infinite." << std::endl;
                break;
            }

            // Check if the player wants to skip to the end of the game
            if (!skipToEnd) checkForSkip();
        }

        // Print a message for the winner
        winner();
        

    } while (playAgain()); // Play again if the user asks
}

// Reset hands after each game
void War::resetHands(Hand& initialDeck, Hand& p1, Hand& p2)
{
    // Empty both player hands
    while (!p1.empty()) {
        p1.drawTop();
    }
    while (!p2.empty()) {
        p2.drawTop();
    }

    // Shuffle the deck
    initialDeck.shuffle();

    // Deal the cards out to the players
    // A full deck has an even number of cards
    std::deque<Card> deck = initialDeck.getCards();
    while (!deck.empty())
    {
        p1.placeBottom(deck.front());
        deck.pop_front();
        p2.placeBottom(deck.front());
        deck.pop_front();
    }
}


// Print introduction message
void War::introduction()
{
    std::cout << "Welcome to the card game War. This is a one or two player game." << std::endl;
    std::cout << "Press Enter to take your turn, or input a letter to skip to the end." << std::endl << std::endl;
}

// Get player names from user input
void War::getNames()
{
    std::cout << "Enter the name of player 1: ";
    std::getline(std::cin, nameP1);
    std::cout << std::endl << "Enter the name of player 2: ";
    std::getline(std::cin, nameP2);
    std::cout << std::endl;
}

// Compare top cards of player hands and perform a "battle"
void War::battle()
{
    // Draw top card from each player's hand
    Card c1, c2;
    c1 = player1.drawTop();
    c2 = player2.drawTop();
    
    // Print cards that were played
    printCards(c1.name(), c2.name());
    
    // Insert cards into the set and update their frequency in the map
    uniqueCardsPlayed.insert(c1.name());
    uniqueCardsPlayed.insert(c2.name());
    cardFrequency[c1.name()]++;
    cardFrequency[c2.name()]++;

    if (c1.getValue() == c2.getValue())
    {
        // If the cards have the same value, a "war" occurs
        int winner = war();

        // Give the spoils to the winner
        if (winner == 1)
        {
            player1.placeBottom(c1);
            player1.placeBottom(c2);
            std::cout << nameP1;
        }
        else if (winner == 2)
        {
            player2.placeBottom(c1);
            player2.placeBottom(c2);
            std::cout << nameP2;
        }

        std::cout << " won the battle!" << std::endl;
    }
    else if (c1.getValue() > c2.getValue())
    {
        // Give the spoils to player 1
        player1.placeBottom(c1);
        player1.placeBottom(c2);

        std::cout << nameP1 << " won this round." << std::endl;
    }
    else if (c2.getValue() > c1.getValue()) 
    {
        // Give the spoils to player 2
        player2.placeBottom(c1);
        player2.placeBottom(c2);
        std::cout << nameP2 << " won the battle!" << std::endl;
    }

}

// Perform a "war" if both players play cards with the same value
int War::war()
{
    std::cout << "War!" << std::endl;
    
    // Spoils of the war are initially empty
    std::deque<Card> spoils;

    // Each player puts three cards face down and plays a fourth card face up
    for (int i = 0; i < 3; ++i)
    {
        if (player1.empty() || player2.empty()) break;
        spoils.push_back(player1.drawTop());
        spoils.push_back(player2.drawTop());
    }

    // If either player runs out of cards, the other player gets all the spoils
    if (player1.empty() || player2.empty())
    {
        player1.placeBottom(spoils.begin(), spoils.end());
        return 1;
    }

    // Both players play a fourth card face up
    if (!player1.empty() && !player2.empty())
    {
        Card p1Card = player1.drawTop();
        Card p2Card = player2.drawTop();

        spoils.push_back(p1Card);
        spoils.push_back(p2Card);

        // Give the spoils to player 1
        if (p1Card.getValue() > p2Card.getValue())
        {
            player1.placeBottom(spoils.begin(), spoils.end());
            return 1;
        }
        // Give the spoils to player 2
        else if (p2Card.getValue() > p1Card.getValue())
        {
            player2.placeBottom(spoils.begin(), spoils.end());
            return 2;
        }
    }
    
    
    // If the war ends in another tie, another "war" occurs
    return 0;
}

// Print winner or tie message
void War::winner()
{
    printStats();
    std::cout << "Game Over!" << std::endl;
    if (player1.empty())
        std::cout << nameP2 << " wins!" << std::endl;
    else if (player2.empty())
        std::cout << nameP1 << " wins!" << std::endl;
    else
        std::cout << "It's a draw!" << std::endl;
}

// Ask if the user wants to play again
bool War::playAgain()
{
    char response;
    std::cout << "Would you like to play again? (y/n): ";
    std::cin >> response;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return response == 'y' || response == 'Y';
}

// Check if the player wants to skip to the end of the game
void War::checkForSkip()
{
    std::cout << "Press Enter to continue, or input a letter to skip to the end: ";
    char input = std::cin.get();
    if (input != '\n')
    {
        skipToEnd = true;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Print the cards that were played by each player
void War::printCards(const string& p1Card, const string& p2Card)
{
    std::cout << nameP1 << " (" << player1.size() << " cards): " << p1Card << std::endl;
    std::cout << nameP2 << " (" << player2.size() << " cards): " << p2Card << std::endl;
}

// Function to print stats at the end of the game
void War::printStats()
{
    std::cout << "Unique cards played: " << uniqueCardsPlayed.size() << std::endl;
    std::cout << "Card frequency:" << std::endl;
    for (const auto& card : cardFrequency)
    {
        std::cout << card.first << ": " << card.second << std::endl;
    }
}

