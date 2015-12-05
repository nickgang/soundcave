//
//  stalacmite.cpp
//  Sound_Cave
//
//  Created by Nicholas Gang on 12/4/15.
//
//

#include <stdio.h>
#include "stalacmite.h"
#include "ofApp.h"
#include "maximilian.h"/* include the maximilian library */
#include "time.h"

void stalacmite::drawStalacs() {
    //This is the code from draw(), will have to alter this significantly
    
    /*
    stalacs[0].cylPos.setPosition(ROOM_WIDTH/2,0,ROOM_DEPTH/2);
    
    ofPushMatrix();
    ofTranslate(stalacs[0].cylPos.getPosition());
    ofPushStyle();
    ofDisableAlphaBlending();
    ofSetColor(255);
    ofSetCylinderResolution(6,6);
    
    float currentHeight = stalacs[0].cyl.getHeight();
    
    if (currentHeight < maxHeight*0.9 && isGrowing){
        stalacs[0].cyl.setHeight(currentHeight+(maxHeight-currentHeight)*(slew*0.05));
        stalacs[0].cyl.draw();
        
        //update y of the cylPos node with the changing height
        stalacs[0].cylPos.setPosition(stalacs[0].cylPos.getX(),stalacs[0].cyl.getHeight(),stalacs[0].cylPos.getZ());
        
        stalacs[0].isDrawn = true;
    }
    else if (currentHeight>0.5*maxHeight) {
        stalacs[0].cyl.setHeight(currentHeight-(currentHeight)*(slew*0.05));
        stalacs[0].cyl.draw();
        
        //update y of the cylPos node with the changing height
        stalacs[0].cylPos.setPosition(stalacs[0].cylPos.getX(),stalacs[0].cyl.getHeight(),stalacs[0].cylPos.getZ());
        
        stalacs[0].isDrawn = true;
        isGrowing=false;
    }
    else {
        isGrowing=true;
        stalacs[0].cyl.draw();
        
        //update y of the cylPos node with the changing height
        stalacs[0].cylPos.setPosition(stalacs[0].cylPos.getX(),stalacs[0].cyl.getHeight(),stalacs[0].cylPos.getZ());
        
        stalacs[0].isDrawn = true;
    }
     
    
    
    
    ofPopStyle();
    ofPopMatrix();
    */

}

// Return the position of the stalacmite as a 3d vector
ofVec3f stalacmite::getPosition(){
    return cylPos.getPosition();
}

// Perturb the shape in some way, probably with a click
void stalacmite::perturb() {
    //Perturb code goes here
}
