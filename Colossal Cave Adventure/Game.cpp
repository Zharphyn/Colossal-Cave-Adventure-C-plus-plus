//
//  Game.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-05.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include <string>
#include <sstream>
#include <vector>
#include "Game.h"

#include "MotionVerb.h"
#include "ActionVerb.h"
#include "Message.h"
#include "Object.h"

Game::Game() {
    // This will parse the data file
    this->data = new Data();
    
    Location* startingLocation = this->data->getLocationByNumber(1);
    this->player = new Player(startingLocation);
}
Game::~Game() {
    delete this->player;
    
    // Important to delete this one last since a vector may not hold pointers to deleted objects
    delete this->data;
}

/* Split function from: http://stackoverflow.com/questions/236129/splitting-a-string-in-c#answer-236803 */
vector<string>& Game::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
/* End split */

string Game::parseInput(string input) {
    cout << "Input: " << input << endl;
    vector<string> lineVector;
    vector<Word*> spokenWords = vector<Word*>();
    
    this->split(input, ' ', lineVector);
    
    // Get the word object for every word written
    Word* word = NULL;
    for (int i = 0; i < lineVector.size(); i++) {
        if ( (word = this->data->findWord(lineVector.at(i))) != NULL) {
            spokenWords.push_back(word);
        } else {
            // For debugging
            cout << "Word not found: " << lineVector.at(i) << endl;
        }
    }
    
    if (spokenWords.size() > 0) {
        if (dynamic_cast<MotionVerb*>(spokenWords.at(0))) {
            Message* msg = NULL;
            Location* loc = NULL;
            // Printing messages overrides going to a new location, therefore we check that first
            if ( (msg = this->player->getCurrentLocation()->shouldPrintMessage((MotionVerb*)spokenWords.at(0))) != NULL) {
                // Print out message to player
                return msg->getContent();
            } else if ( (loc = this->player->getCurrentLocation()->shouldGoToLocation((MotionVerb*)spokenWords.at(0))) != NULL ) {
                // Go to new location
                this->player->setCurrentLocation(loc);
                return loc->getShortDescription() + "\n" + loc->getLongDescription();
            }
        }
        // Handle actions like "get keys", "get lamp"
        else if (dynamic_cast<ActionVerb*>(spokenWords.at(0)) && spokenWords.size() == 2) {
            cout << "ActionVerb" << endl;
            ActionVerb* verb = (ActionVerb*)spokenWords.at(0);
            if (dynamic_cast<Object*>(spokenWords.at(1))) {
                cout << "Object" << endl;
                Object* obj = (Object*)spokenWords.at(1);
                if (verb->doesPickUpObject()) {
                    if (this->player->getCurrentLocation()->hasObject(obj)) {
                        // Pick up the object from the current location
                        this->player->getCurrentLocation()->removeObject(obj);
                        // Add it to the player
                        this->player->pickUpObject(obj);
                        return "You picked up: " + obj->getInventoryMessage();
                    } else {
                        return "There is no such object at this location.";
                    }
                } else if (verb->doesDropObject()) {
                    if (this->player->hasObject(obj)) {
                        // Drop object
                        this->player->dropObject(obj);
                        // Leave the object at the current location
                        this->player->getCurrentLocation()->addObject(obj);
                        return "You dropped: " + obj->getInventoryMessage();
                    }
                }
            } else {
                return "No such object exist";
            }
        }
    }
    
    return "Unknown words";
}