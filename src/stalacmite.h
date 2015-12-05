//
//  stalacmite.h
//  Granular_Testing
//
//  Created by Nicholas Gang on 11/8/15.
//
//

#ifndef stalacmite_h
#define stalacmite_h
#include "ofMain.h"
#include "ofApp.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "ofxOsc.h"
#include "ofxStk.h"
#include <sys/time.h>


#endif /* stalacmite_h */

//Attempt at a stalactite class
class stalacmite {
    
public:
    //Methods
    void drawStalac();
    void killStalac();
    ofVec3f getPosition();
    void perturb();
    
    //Physical properties
    ofCylinderPrimitive cyl;
    ofNode cylPos;
    ofApp::MusicalNote xChord[2];
    ofApp::MusicalNote zChord[2];
    int xOctave;
    int zOctave;
    bool isDrawn;
    
    //Constructor
    stalacmite();
    
    //Destructor
    ~stalacmite();
    
private:
    
};

