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
    
    selectNode.setPosition(ROOM_WIDTH/2,0,ROOM_DEPTH/2);
    
    selector.set(width*2,width*2);
    selector.setPosition(selectNode.getPosition());
    
    
}

void ofApp::interpSelector() {
    
    //Get interpolated point for current frame
    float x = selector.getX()+(selectNode.getX()-selector.getX())*slew;
    float y = selector.getY()+(selectNode.getY()-selector.getY())*slew;
    float z = selector.getZ()+(selectNode.getZ()-selector.getZ())*slew;
    
    //Store these in 3d vector
    ofVec3f interpCoords;
    interpCoords.set(x,y,z);
    
    //Set selector position to this point
    selector.setPosition(interpCoords);

}
