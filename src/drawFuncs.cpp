//
//  drawFuncs.cpp
//  Sound_Cave
//
//  This file contains the functions for drawing the various objects that make up the
//  cave and that exist inside of the cave. These are called by draw() in ofApp.cpp
//
//  Created by Nicholas Gang on 12/4/15.
//
//

#include <stdio.h>
#include "ofApp.h"
#include "maximilian.h"/* include the maximilian library */
#include "time.h"

//--------------------------------------------------------------
// drawMenuScreen: This determines the look of the menu screen
//--------------------------------------------------------------
void ofApp::drawMenuScreen(){
    
    float height = ofGetWindowHeight();
    float width = ofGetWindowWidth();
    
    ofPushStyle();
    
    ofSetColor(255);
    title.drawString("Welcome to Sound Cave",20,height*0.1);
    
    ofPushStyle();
    ofSetColor(255, 255, 255, 255*sin((MY_PIE/2)*ofGetElapsedTimef()));
    subTitle.drawString("press 'm' to begin",width/4,height/4);
    ofPopStyle();
    
    instructions.drawString("Use arrow keys to move selector, click to create a stalacmite \n 1,2,3 keys swtich camera view \n 8 key freezes the sphere. \n 0 key sharpens sphere, " ,width/8,3*height/4);
    
    
    ofPopStyle();
}

//-----------------------------------------------------------
// Function to draw basic shape of the room in the background
//-----------------------------------------------------------
void ofApp::drawRoom() {
    
    //shorter variable names
    float width = ROOM_WIDTH;
    float height = ROOM_HEIGHT;
    float depth = ROOM_DEPTH;
    
    float waveScale=0;
    
    ofPushStyle();
    
    //Some style parameters
    ofNoFill();
    ofSetLineWidth(3);
    
    //Draw wall 1
    ofSetColor(255,255,255);
    
    for (int i=0;i<NUM_LINES;i++){
        ofBeginShape();
        for (int j=0;j<depth;j++){
            ofVertex(0,((i*height)/NUM_LINES)+waveScale*wallBuffer[round((double)(depth/bufferSize))],j);
        }
        ofEndShape(false);
    }
    
    //Draw wall 2
    for (int i=0;i<NUM_LINES;i++){
        ofBeginShape();
        for (int j=0;j<width;j++){
            ofVertex(j,((i*height)/NUM_LINES)+waveScale*wallBuffer[round((double)(depth/bufferSize))],depth);
        }
        ofEndShape(false);
    }
    
    //Draw wall 3
    for (int i=0;i<NUM_LINES;i++){
        ofBeginShape();
        for (int j=depth;j>=0;j--){
            ofVertex(width,((i*height)/NUM_LINES)+waveScale*wallBuffer[round((double)(depth/bufferSize))],j);
        }
        ofEndShape(false);
    }
    
    //Draw wall 4
    for (int i=0;i<NUM_LINES;i++){
        ofBeginShape();
        for (int j=width;j>=0;j--){
            ofVertex(j,((i*height)/NUM_LINES)+waveScale*wallBuffer[round((double)(depth/bufferSize))],0);
        }
        ofEndShape(false);
    }
    
    
    ofPopStyle();
}

//--------------------------------------------------------------
// drawSprite: Function to draw user position ball (aka sprite)
//--------------------------------------------------------------
void ofApp::drawSprite() {
    
    //Spin amounts
    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);
    
    //Click resets spin
    if(bMousePressed) {
        spinX = spinY = 0.0f;
    }
    
    //Enable additive blending
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    material.begin();
    
    //Place the sphere and make it spin
    userPos.setPosition(0,ROOM_HEIGHT/8,0);
    userPos.rotate(spinX, 1.0, 0.0, 0.0);
    userPos.rotate(spinY, 0, 1.0, 0.0);
    
    //Triangles
    if(viewMode == 3) {
        triangles = userPos.getMesh().getUniqueFaces();
    }
    
    if(bFill) {
        ofFill();
        ofSetColor(22,51,90);
        
        //Send the triangles around
        if(viewMode == 3) {
            float angle = (ofGetElapsedTimef() * 1.4);
            ofVec3f faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc );
                }
            }
            userPos.getMesh().setFromTriangles( triangles );
        }
        
        userPos.draw();
    }
    
    //Draw wireframe sphere
    if(bWireframe) {
        ofNoFill();
        //Wireframe color
        ofSetColor(128, 128, 128);
        if(!bFill) ofSetColor(255);
        userPos.setScale(1.01f);
        userPos.drawWireframe();
        userPos.setScale(1.f);
    }
    
    material.end();
    
    //Draw normals of sphere
    if(bDrawNormals) {
        ofSetColor(255);
        userPos.drawNormals(20, bSplitFaces);
    }
    
    //Draw axes on sphere
    if(bDrawAxes) {
        userPos.drawAxes(userPos.getRadius()+30);
    }
    ofDisableBlendMode();
}

