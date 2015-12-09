#include "ofApp.h"
#include "maximilian.h" //Granular library
#include "ofxTonic.h"
#include "time.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Load fonts for the menu screen
    title.loadFont("Alfphabet-IV.ttf", 30);
    subTitle.loadFont("Alfphabet-IV.ttf", 18);
    
    ofEnableDepthTest();
    ofEnableSmoothing();
    
    // Menu screen comes up on
    menuScreen = true;
    
    // Try drawing this in setup to reduce blank screen time
    drawMenuScreen();
    
    
    //--------------------------------------------------------------
    
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
    
    //pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    //pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    
    //-------------------------------------------------------------
    
    //----------User position sphere stuff ----------------------
    
    //Initialize some sphere parameters
    ofSetVerticalSync(true);
    
    bFill       = true;
    bWireframe  = true;
    bDrawNormals= false;
    bDrawAxes   = false;
    bInfoText   = true;
    bMousePressed   = false;
    bSplitFaces = false;
    
    float area = ROOM_DEPTH*ROOM_WIDTH;
    float width = ofGetWidth() * .10;
    
    //Initialize size
    userPos.setRadius( width );
    
    viewMode=3;
    
    userPos.setResolution(4);
    
    userPos.setPosition(margin,ROOM_HEIGHT/8,margin);
    userHeight=userPos.getY();
    
    //Read sphere into position vector
    posNode.setPosition(userPos.getX(),userPos.getY(),userPos.getZ());
    target.setPosition(userPos.getX()+20,userPos.getY(),userPos.getZ());
    
    slew = 0.10;
    
    xInc = 10;
    yInc = 50;
    zInc = 10;
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    //--------------------------------------------------------------
    
    //----------Selector Setup--------------------------------------
    setupSelector();
    
    //--------------------------------------------------------------
    
    
    //----------Cylinder Setup--------------------------------------
    
    int degrees[MAX_STALAC] = {0,3,7,10,14,17,24,27,31,34,38,41,48,51,55,58};
    
    //Initialize all stalacmites to the same parameters for now
    for (int i=0;i<MAX_STALAC;i++){
        stalacs[i].cyl.set(width,0);
        
        stalacs[i].isDrawn = false;
        
        stalacs[i].pitch=degrees[i];
        
        stalacs[i].octave=3;
        
    }
    maxHeight=10*width;
    isGrowing=false;
    
    //--------------------------------------------------------------
    
    //Tonic Setup
    setupTonic();
    
    
    //-------------------Maximilian Setup---------------------------
    
    playGrains=false;
    
    //Always needed for Maximilian
    sender.setup(HOST, SENDPORT);
    receiver.setup(RECEIVEPORT);
    
    //Load in samples
    // **************TO DO, add some to this project that will translate well to ambient music****************
    samp.load(ofToDataPath("weapon_short.wav"));
    //samp2.load(ofToDataPath("Egon_5.wav"));
    //samp3.load(ofToDataPath("man_in_me.wav"));
    //samp4.load(ofToDataPath("In_1973.wav"));
    //samp5.load(ofToDataPath("La_Nuit.wav"));
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    
    //Initialize sample rate and buffer size
    sampleRate = MY_SRATE;
    bufferSize = MY_BUFFER_SIZE;
    
    ts = new maxiTimestretch<grainPlayerWin>(&samp);
    ts2 = new maxiTimestretch<grainPlayerWin>(&samp2);
    ts3 = new maxiTimestretch<grainPlayerWin>(&samp3);
    ts4 = new maxiTimestretch<grainPlayerWin>(&samp4);
    ts5 = new maxiTimestretch<grainPlayerWin>(&samp5);
    stretches.push_back(ts);
    stretches.push_back(ts2);
    stretches.push_back(ts3);
    stretches.push_back(ts4);
    stretches.push_back(ts5);
    
    //Initialize Grain parameters
    speed = -.75;
    grainLength = 0.05;
    current=0;
    
    fft.setup(1024, 512, 256);
    oct.setup(44100, 1024, 10);
    
    int current = 0;
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    /* Anything that you would normally find/put in maximilian's setup() method needs to go here. For example, Sample loading.
     
     */
    
    isTraining=true;
    
    ofBackground(0,0,0);
    
    ofSetSphereResolution(10);
    
    wallBuffer.resize(bufferSize);
    
    ofSoundStreamSetup(2,0,this, sampleRate, bufferSize, 4);
    /* this has to happen at the end of setup - it switches on the DAC */
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //This takes care of updating the audio from tonic
    triggerTonic();
    updateFilters();
    
    //Update grain speed with sphere height
    //speed = -.75+userHeight*0.001;
    
    //Update position of selector with some interpolation
    interpSelector();
    
    //Call the chord triggering function
    triggerChord();
    
    //Update filter cutoff based on sphere height
    //lpf.setResonance(10*userHeight,0.5);
    
    //Update height of the target (if it has changed)
    target.setPosition(target.getX(),userHeight,target.getZ());
    
    //Figure out slew coordinates
    slewCoords.set(posNode.getX()+(target.getX()-posNode.getX())*slew,posNode.getY()+(target.getY()-posNode.getY())*slew,posNode.getZ()+(target.getZ()-posNode.getZ())*slew);
    
    //Move position node to slew coordinates
    posNode.setPosition(slewCoords);
    
    // Set position of moving camera relative to target
    cam[0].setPosition(posNode.getX()-250,posNode.getY()+250,posNode.getZ()-250);
    
    //Cam 1 looks at sphere from above
    cam[1].lookAt(posNode);
    
    //Cam2 looks at target from inside the sphere
    cam[2].setPosition(posNode.getPosition());
    cam[2].lookAt(target);
    
    
    //Maxmilian Granular stuff (Wekinator)
    
    if (!isTraining) {
        
        
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            // check for mouse moved message
            if(m.getAddress() == "/wek/outputs"){
                
                //cout << m.getArgAsFloat(0);
                speed = ((double ) m.getArgAsFloat(0) * 4.0) - 2.0;
                grainLength = (m.getArgAsFloat(1) * 0.1) + 0.001;
                pos = ((double) m.getArgAsFloat(0) * 2.0);
                
            }
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (menuScreen) {
        drawMenuScreen();
    }
    
    if (!menuScreen){
    
    cam[currentCam].begin();
    
        
        if (isLit){
            pointLight1.enable();
            pointLight2.enable();
            pointLight3.enable();
            pointLight4.enable();
        }

    //Draw the room
    drawRoom();
        
        //Some code to make the selector look right
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(selector.getPosition());
        ofRotateX(90);
        ofTranslate(-1*selector.getPosition());
        ofSetColor(255);
        selector.draw();
        
        ofPopStyle();
        ofPopMatrix();
        
        for (int i=0;i<MAX_STALAC;i++){
            if (stalacs[i].isDrawn) {
                ofPushMatrix();
                ofPushStyle();
                ofDisableAlphaBlending();
                ofSetColor(255);
                ofSetCylinderResolution(6,6);
                
                ofTranslate(stalacs[i].cylPos.getX(),0,stalacs[i].cylPos.getZ());
                stalacs[i].cyl.draw();
                
                float currentHeight = stalacs[i].cyl.getHeight();
                
                if (currentHeight < maxHeight*0.9 && isGrowing){
                    stalacs[i].cyl.setHeight(currentHeight+(maxHeight-currentHeight)*(slew*0.05));
                    
                    //update y of the cylPos node with the changing height
                    stalacs[i].cylPos.setPosition(stalacs[i].cylPos.getX(),stalacs[i].cyl.getHeight(),stalacs[i].cylPos.getZ());
                }
                else if (currentHeight>0.5*maxHeight) {
                    stalacs[i].cyl.setHeight(currentHeight-(currentHeight)*(slew*0.05));
                    stalacs[i].cyl.draw();
                    
                    //update y of the cylPos node with the changing height
                    stalacs[i].cylPos.setPosition(stalacs[i].cylPos.getX(),stalacs[i].cyl.getHeight(),stalacs[i].cylPos.getZ());
                    
                    isGrowing=false;
                }
                else {
                    isGrowing=true;
                    stalacs[i].cyl.draw();
                    
                    //update y of the cylPos node with the changing height
                    stalacs[i].cylPos.setPosition(stalacs[i].cylPos.getX(),stalacs[i].cyl.getHeight(),stalacs[i].cylPos.getZ());
                }

                
                ofPopStyle();
                ofPopMatrix();
            }
        }
        

    
    //Update target position
    moveTarget("border");

    ofPushMatrix();
    
    //Translate to where we need to draw the sphere
    ofTranslate(posNode.getPosition());
    
    //Draw user position ball
    drawSprite();
    
    ofPopMatrix();
    
    pointLight1.disable();
    pointLight2.disable();
    pointLight3.disable();
    pointLight4.disable();
    
        
    cam[currentCam].end();
        
    }
    
}

//-----------Trigger Chord Function-----------------------------
void ofApp::triggerChord(){
    
    // Calculate distance from sphere to each stalacmite
    float distances[MAX_STALAC][2];
    for (int i=0;i<MAX_STALAC;i++){
        //Fill distances matrix with euclidean distance to stalacs in x and z direction
        distances[i][0]=sqrt((posNode.getX()*posNode.getX())+(stalacs[i].cylPos.getX())*(stalacs[i].cylPos.getX()));
        distances[i][1]=sqrt((posNode.getZ()*posNode.getZ())+(stalacs[i].cylPos.getZ())*(stalacs[i].cylPos.getZ()));
    }
    
    //Gain factor and epsilon for intersecting sphere and stalacmite
    float gain=100;
    float eps=0.0001;
    
    for (int i=0;i<MAX_STALAC;i++){
        if (stalacs[i].isDrawn){
            if (abs(posNode.getX()-stalacs[i].cylPos.getX())<10){
                //Setting relative gain of notes to 1/distance
                
                //Show normals for a frame
                bDrawNormals = !bDrawNormals;
                }
            
            else if (abs(posNode.getZ()-stalacs[i].cylPos.getZ())<10){
                
                //Show normals for a frames
                bDrawNormals = !bDrawNormals;
                }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    float temp = 0;
    float scaleRMS=40;
    avgRMS = 0;
    
    
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
    
    
    for (int i = 0; i < bufferSize; i++){
        
        /* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
         For information on how maximilian works, take a look at the example code at
         http://www.maximilian.strangeloop.co.uk
         under 'Tutorials'.
         */
    
        //		wave = stretches[current]->play(speed, grainLength, 5, 0);
        wave = stretches[current]->play(speed, 0.1, 4, 0);
        //		wave = stretches[current]->play2(pos, 0.1, 4);
        if (fft.process(wave)) {
            oct.calculate(fft.magnitudes);
        }
        

        //play result
        if (playGrains){
            mymix.stereo(wave, outputs, 0.5);
            output[i*nChannels    ] += outputs[0]; /* You may end up with lots of outputs. add them here */
            output[i*nChannels + 1] += outputs[1];
        
            temp = (outputs[0] + outputs[1])/2;
            avgRMS= avgRMS+temp;
        }
        else if(!playGrains) {
            //output[i*nChannels] = 0;
            //output[i*nChannels + 1] = 0;
        }

        // Set wallBuffer to the audio signal we are hearing
        // We will assign this to make waveforms in the walls in drawRoom
        
        //Interpolate the signal
        float tau = 0.4;
        wallBuffer[i] += (output[i*nChannels]-wallBuffer[i])*tau;
        
        /* You may end up with lots of outputs. add them here */
    }
    
    //Update size of user position sphere with RMS level
    
    avgRMS = sqrt((avgRMS/bufferSize)*(avgRMS/bufferSize));
    //userPos.setRadius(userPos.getRadius()+avgRMS*scaleRMS);
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    // samples are "interleaved"
    for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    float gain=100;
    int octaveScale=3;
    
    float selectMove = 50;
    
    //Manual camer pan amount
    float manPan=5;
    float manTilt=5;
    
    switch(key) {
            
        // Toggle Menu Screen
        case 'm':
            menuScreen = !menuScreen;
            break;
            
        //This one is for Maximilian stuff
        case OF_KEY_RETURN:
            isTraining=!isTraining;
            //cout << isTraining;
            break;
        case 'p':
            playGrains = !playGrains;
            break;
            
        //For user position sphere stuff
        case 'f':
            ofToggleFullscreen();
            break;
        case '8':
            bFill = !bFill;
            break;
        case '9':
            bWireframe = !bWireframe;
            break;
        case '0':
            bDrawNormals = !bDrawNormals;
            break;
        case ' ':
            if (viewMode==0){
                viewMode = 3;
            }
            else if (viewMode==3){
                viewMode = 0;
                userPos.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
                // rebuild the sphere
                userPos.mapTexCoords(0, 0, 5, 5);
            }
            break;
        case ',':
            userHeight-=yInc;
            break;
        case '.':
            userHeight+=yInc;
            break;
            
        // Change speed of sphere
        case '=':
            xInc+=5;
            zInc+=5;
            break;
        case '-':
            xInc-=5;
            zInc-=5;
            break;
            
        
        //For camera stuff
        case '1':
            currentCam=0;
            break;
        case '2':
            currentCam=1;
            break;
        case '3':
            currentCam=2;
            break;
        case OF_KEY_LEFT:
            cam[currentCam].pan(manPan);
            break;
        case OF_KEY_RIGHT:
            cam[currentCam].pan(-manPan);
            break;
        case OF_KEY_UP:
            cam[currentCam].tilt(manTilt);
            break;
        case OF_KEY_DOWN:
            cam[currentCam].tilt(-manTilt);
            break;
        
        //Light Stuff
        case 'l':
            isLit = !isLit;
            break;
            
        //Moving the selector TO DO, set boundaries as room walls
        case 'w':
            //Update the node tracking selector position
            //We set the selector plane to this point in update
            selectNode.setPosition(selectNode.getX()+selectMove,selectNode.getY(),selectNode.getZ());
            break;
        case 's':
            selectNode.setPosition(selectNode.getX()-selectMove,selectNode.getY(),selectNode.getZ());
            break;
        case 'd':
            selectNode.setPosition(selectNode.getX(),selectNode.getY(),selectNode.getZ()+selectMove);
            break;
        case 'a':
            selectNode.setPosition(selectNode.getX(),selectNode.getY(),selectNode.getZ()-selectMove);
            break;
            
            
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key){
            //Leaving this here in case I need it
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    //*******TO DO: Right now the mouse changes grain parameters, change this to something about our polygons *********
    if (isTraining) {
        
        //speed = ((double ) x / ofGetWidth() * 4.0) - 2.0;
        grainLength = ((double) y / ofGetHeight() * 0.1) + 0.001;
        pos = ((double) x / ofGetWidth() * 2.0);
        //	cout << pos << endl;
        
        ofxOscMessage m;
        m.setAddress("/wekinator/control/outputs");
        m.addFloatArg((float)x/ofGetWidth());
        m.addFloatArg((float)y/ofGetHeight());
        m.addFloatArg((float)current/stretches.size()-1);
        sender.sendMessage(m);
        //cout << "messageSent" << "\n";
    }
    
    

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //if (++current > stretches.size()-1) current = 0;
    
    //Position of selector when clicked is position to draw new stalacmite
    stalacs[nextToDraw].cylPos.setPosition(selector.getPosition());
    stalacs[nextToDraw].isDrawn=true;
    
    //Increment index of stalacmites
    if (nextToDraw++ > MAX_STALAC) nextToDraw=0;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
