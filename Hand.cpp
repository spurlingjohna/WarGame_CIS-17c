/* 
 * File:   Hand.cpp
 * Author: Andrew Spurling
 * Date:   4/23/23
 */
#include "Hand.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <deque>
#include <iterator>
#include <random>
#include <chrono>



//Default constructor for the Hand class.
Hand::Hand(){}

//Returns a Hand object containing a full deck of 52 cards.
Hand Hand::fullDeck()
{
    //Create a Hand with a full deck of cards
    Hand deck;

    const int NUM_SUITS = 4;
    const int NUM_FACES = 13;
    Suit suit[NUM_SUITS] = {clubs, diamonds, hearts, spades};
    Value face[NUM_FACES] = {ace, deuce, trey, four, five, six, seven, eight, nine, ten, jack, queen, king};

    for (int i = 0; i < NUM_SUITS; i++)
    {
        for (int j = 0; j < NUM_FACES; j++)
        {
            deck.placeBottom(Card(face[j], suit[i]));
        }
    }

    return deck;
}

void Hand::shuffle()
{
    //Shuffles the cards in the Hand.
    srand(time(0) + size());
    std::deque<Card> temp_deck(cards.begin(), cards.end());
    std::random_shuffle(temp_deck.begin(), temp_deck.end());

    cards.assign(temp_deck.begin(), temp_deck.end());
}

Card Hand::drawTop()
{
    //Remove and return the top card.
    Card drawnCard = cards.front();
    cards.pop_front();
    return drawnCard;
}

Card Hand::drawBottom()
{
    //Remove and return the last card
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Hand::placeTop(Card c)
{
    //Insert the card at the beginning of the deck
    cards.push_front(c);
}

void Hand::placeBottom(Card c)
{
    //Insert the card at the end of the deck
    cards.push_back(c);
}

bool Hand::empty()
{
    //Returns true if the Hand is empty, false otherwise.
    return cards.empty();
}

int Hand::size()
{
    //Returns the number of cards in the Hand.
    return cards.size();
}

template <typename InputIt>
void Hand::placeBottom(InputIt first, InputIt last)
{
    //Inserts a range of cards at the bottom of the Hand.
    for (InputIt it = first; it != last; ++it)
    {
        cards.push_back(*it);
    }
}

// Explicit template instantiation for the placeBottom function
template void Hand::placeBottom<std::deque<Card>::iterator>
(std::deque<Card>::iterator first, std::deque<Card>::iterator last);
//Instantiates the template function placeBottom for the deque iterator type.

// Functor for comparing cards
struct CardComparer {
    Card targetCard;

    CardComparer(const Card &card) : targetCard(card) {}

    bool operator()(const Card &card) const {
        return (card.getSuit() == targetCard.getSuit()) && (card.getValue() == targetCard.getValue());
    }
};

// Function to find a specific card in the hand
std::deque<Card>::iterator Hand::findCard(const Card &targetCard) {
    return std::find_if(cards.begin(), cards.end(), CardComparer(targetCard));
}

// Swaps the cards at the given indices in the hand.
void Hand::swapCards(int index1, int index2) {
    if (index1 < 0 || index1 >= cards.size() || index2 < 0 || index2 >= cards.size()) {
        throw std::out_of_range("Invalid index.");
    }
    std::swap(cards[index1], cards[index2]);
}

// Struct to sort through cards
struct CardSorter {
    bool operator()(const Card &a, const Card &b) const {
        if (a.getValue() == b.getValue()) {
            return a.getSuit() < b.getSuit();
        }
        return a.getValue() < b.getValue();
    }
};

// Function to sort hand
void Hand::sortHand() {
    std::sort(cards.begin(), cards.end(), CardSorter());
}

// Function copies all cards from 'cards' and returns new list
std::list<Card> Hand::listCards() const {
    return std::list<Card>(cards.begin(), cards.end());
}
