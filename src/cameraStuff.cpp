//
//  cameraStuff.cpp
//  Sound_Cave
//
//  Created by Nicholas Gang on 12/10/15.
//
//

#include <stdio.h>
#include "ofApp.h"
#include "maximilian.h" //Granular library
#include "ofxTonic.h"
#include "time.h"

void ofApp::setupCams() {
    //----------Stuff for the cameras-------------------------------
    dollyInc = 8;
    panInc = 90;
    boomInc = 1;
    wallCounter = 0;
    margin = 600;
    
    //Sets default starting camera
    currentCam=0;
    
    //Setup individual cameras
    
    //Cam 0 is the moving camera behind the sphere
    cam[0].pan(220);
    cam[0].setFov(65);
    
    //Cam 1 is the top view, looking at the sphere
    cam[1].setPosition(ROOM_WIDTH/2,ROOM_HEIGHT*2,ROOM_DEPTH/2);
    cam[1].lookAt(cam[0]);
    
    //Cam 2 is first person view of the sphere
    cam[2].setPosition(posNode.getPosition());
    cam[2].lookAt(target);
    
    //Cam 3 orbits the sphere
    cam[3].setPosition(posNode.getPosition());
    cam[3].lookAt(posNode);
    cam[3].orbit(500,500,500,posNode.getPosition());
    
    //Point Lighting Setup, turning lights on by default
    isLit=true;
    ofSetSmoothLighting(true);
    pointLight1.setPosition(0,ROOM_HEIGHT*4,0);
    pointLight2.setPosition(ROOM_WIDTH,ROOM_HEIGHT*4,0);
    pointLight3.setPosition(ROOM_WIDTH,ROOM_HEIGHT*4,-ROOM_DEPTH);
    pointLight4.setPosition(0,ROOM_HEIGHT*4,-ROOM_DEPTH);
    
    pointLight1.setPointLight();
    pointLight2.setPointLight();
    pointLight3.setPointLight();
    pointLight4.setPointLight();
    
    
    ofVec3f roomMiddle;
    roomMiddle.set(ROOM_WIDTH/2,0,ROOM_DEPTH/2);
    
    pointLight1.lookAt(roomMiddle);
    pointLight2.lookAt(roomMiddle);
    pointLight3.lookAt(roomMiddle);
    pointLight4.lookAt(roomMiddle);
    
    
    // Make the diffuse light color vaguely blue
    pointLight1.setDiffuseColor( ofFloatColor(.30, .50, .80) );
    pointLight2.setDiffuseColor( ofFloatColor(.30, .50, .80) );
    pointLight3.setDiffuseColor( ofFloatColor(.30, .50, .80) );
    pointLight4.setDiffuseColor( ofFloatColor(.30, .50, .80) );
    //pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    
    //-------------------------------------------------------------
}

void ofApp::updateCams() {
    
    // Update height of the target (if it has changed)
    target.setPosition(target.getX(),userHeight,target.getZ());
    
    // Figure out slew coordinates
    slewCoords.set(posNode.getX()+(target.getX()-posNode.getX())*slew,posNode.getY()+(target.getY()-posNode.getY())*slew,posNode.getZ()+(target.getZ()-posNode.getZ())*slew);
    
    // Move position node to slew coordinates
    posNode.setPosition(slewCoords);
    
    // Set position of moving camera relative to target
    cam[0].setPosition(posNode.getX()-250,posNode.getY()+250,posNode.getZ()-250);
    
    // Cam 1 looks at sphere from above
    cam[1].lookAt(posNode);
    
    // Cam2 looks at target from inside the sphere
    cam[2].setPosition(posNode.getPosition());
    cam[2].lookAt(target);
    
    // Cam 3 looks at sphere while orbiting it
    cam[3].lookAt(posNode);
    cam[3].orbit(100,100,500,posNode.getPosition());
    
}

void ofApp::interpCams(float targetFov) {
    //Coming in v 2.0
}

