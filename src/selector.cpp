//
//  selector.cpp
//  Sound_Cave
//
//  This file contains functions dedicated to the selector plane. This plane
//  serves the purpose of determining where each new stalacmite will be drawn
//
//  Created by Nicholas Gang on 12/5/15.
//
//

#include <stdio.h>
#include "ofApp.h"
#include "ofMain.h"
#include "maximilian.h"/* include the maximilian library */
#include "time.h"


void ofApp::setupSelector() {
    
    float width = ofGetWidth() * .10;
    
    selector.setPosition(ROOM_WIDTH/2,0,ROOM_DEPTH/2);
    selector.set(width*2,width*2);
}
