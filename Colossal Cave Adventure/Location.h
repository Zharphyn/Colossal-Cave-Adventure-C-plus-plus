//
//  Location.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Location__
#define __Colossal_Cave_Adventure__Location__

#include <iostream>
#include <string>
#include <vector>
#include "MotionVerb.h"
#include "Object.h"
#include "Message.h"

using namespace std;

class Location {
    
private:
    int number;
    string shortDescription;
    string longDescription;
    
    vector<Object*> *objects;
    vector<bool> assets;
    
    vector<Location*> *accessibleLocations;
    vector<vector<MotionVerb*>*> *motionVerbs;
    
    vector<Message*> *printMessages;
    vector<vector<MotionVerb*>*> *motionVerbsForPrintingMessage;
    
    void init(int number = -1, string longDescription = "", string shortDescription = "");
    
    const int getAccessibleLocationIndex(Location* loc) const;
    const int getPrintMessageIndex(Message* msg) const;
    
    const string listObjects();
    const string getAccessibleLocationsAndMotionVerbsAsString();
    const string getPrintMessagesAndMotionVerbsAsString();
    const string getAssetsAsString();
    
public:
    Location();
    Location(int number);
    Location(int number, string longDescription);
    Location(int number, string longDescription, string shortDescription);
    ~Location();
    
    const int getNumber() const;
    void setNumber(const int number);
    
    const string getShortDescription();
    void setShortDescription(const string desc);
    void appendToShortDescription(const string desc);
    
    const string getLongDescription();
    void setLongDescription(const string desc);
    void appendToLongDescription(const string desc);
    
    void addObject(Object* obj);
    
    void setAsset(const int index, const bool value = true);
    
    void addAccessibleLocation(Location* loc);
    void addMotionVerb(Location* loc, MotionVerb* verb);
    Location* shouldGoToLocation(MotionVerb* verb);
    
    void addPrintMessage(Message* msg);
    void addMotionVerbForPrintMessage(Message* msg, MotionVerb* verb);
    Message* shouldPrintMessage(MotionVerb* verb);
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Location__) */
