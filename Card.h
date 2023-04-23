/* 
 * File:   Card.h
 * Author: Andrew Spurling
 * Date:   4/23/23
 */

#ifndef CARD_H
#define CARD_H

#include <string>
using std::string;

enum Suit {clubs, diamonds, hearts, spades};
enum Value {deuce, trey, four, five, six, seven, 
            eight, nine, ten, jack, queen, king, ace};

class Card {
public:
	// Default constructor, sets suit to clubs and faceValue to deuce
	Card (Value faceValue=deuce, Suit suit = clubs):
		suit (suit), faceValue(faceValue) {}
	
	// Returns the suit of the card
	Suit getSuit()  { return suit; }
	
	// Returns the face value of the card
	Value getValue() { return faceValue; }
	
	// Returns the string representation of the suit of the card
	static string suitName (Suit s);
	
	// Returns the string representation of the face value of the card
	static string valueName (Value v);
	
	// Returns the name of the card as a string, in the format "face value of suit"
	string name() { return valueName (faceValue) + " of " + suitName(suit);}

        // Returns the suit of the card
         Suit getSuit() const { return suit; }
    
         // Returns the face value of the card
         Value getValue() const { return faceValue; }    
   
        
private:
	// Private member variables representing the suit and face value of the card
	Suit suit;
	Value faceValue;
};

#endif
