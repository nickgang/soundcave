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