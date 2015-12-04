//
//  movements.cpp
//  Sound_Cave
//
//  This file contains functions related to the movement of the position sphere
//
//  Created by Nicholas Gang on 12/4/15.
//
//

#include <stdio.h>
#include "ofApp.h"
#include "maximilian.h"/* include the maximilian library */
#include "time.h"


//---------------------------------------------------------------
// Function to move sphere through the cave in one of a few paths
//---------------------------------------------------------------
void ofApp::moveTarget(string spherePath){
    if(spherePath=="border"){
        //Update target position
        
        switch (wallCounter) {
            case 0:
                if (target.getX()<ROOM_WIDTH-margin){
                    target.setPosition(target.getX()+xInc,target.getY(),target.getZ());
                }
                else {
                    target.setPosition(target.getX(),target.getY(),target.getZ()+zInc);
                    wallCounter++;
                }
                break;
            case 1:
                if (target.getZ()<ROOM_DEPTH-margin){
                    target.setPosition(target.getX(),target.getY(),target.getZ()+zInc);
                }
                else {
                    target.setPosition(target.getX()-xInc,target.getY(),target.getZ());
                    wallCounter++;
                }
                break;
            case 2:
                if (target.getX()>margin){
                    target.setPosition(target.getX()-xInc,target.getY(),target.getZ());
                }
                else {
                    target.setPosition(target.getX(),target.getY(),target.getZ()-zInc);
                    wallCounter++;
                }
                break;
            case 3:
                if (target.getZ()>margin){
                    target.setPosition(target.getX(),target.getY(),target.getZ()-zInc);
                }
                else {
                    target.setPosition(target.getX()+xInc,target.getY(),target.getZ());
                    wallCounter=0;
                }
        }
    }
    else if(spherePath=="figureEight"){
        //Add figure 8 path here
    }
}