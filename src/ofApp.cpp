#include "ofApp.h"
#include "maximilian.h" //Granular library
#include "ofxTonic.h"
#include "time.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Load fonts for the menu screen
    title.loadFont("Alfphabet-IV.ttf", 30);
    subTitle.loadFont("Alfphabet-IV.ttf", 18);
    instructions.loadFont("Alfphabet-IV.ttf", 10);
    
    ofEnableDepthTest();
    ofEnableSmoothing();
    
    // Menu screen comes up on
    menuScreen = true;
    bInstructions = false;
    
    // Try drawing this in setup to reduce blank screen time
    drawMenuScreen();
    
    
    //--------------------------------------------------------------
    
    // Cave audio
    cave.loadSound("cave-drips.wav");
    cave.setLoop(true);
    
    playCave = false;
    
    //----------Stuff for the cameras-------------------------------
    
    setupCams();

    
    //----------User position sphere stuff -------------------------
    
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
    
    int degrees[MAX_STALAC] = {0,7,3,10,2,12,17,15,22,19,29,26,36,33,38,41};
    
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
    
    //Check for overlap between selector and stalacmites
    detectOverlap();
    
    //Update grain speed with sphere height
    //speed = -.75+userHeight*0.001;
    
    //Update position of selector with some interpolation
    interpSelector();
    
    updateCams();
    
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
                
                ofTranslate(stalacs[i].cylPos.getX(),stalacs[i].cyl.getHeight()/2,stalacs[i].cylPos.getZ());
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


//-----------------------------------------------------------------
// Audio Out
//-----------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    float temp = 0;
    float scaleRMS=40;
    avgRMS = 0;
    
    
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
    
    
    for (int i = 0; i < bufferSize; i++){
    
        //		wave = stretches[current]->play(speed, grainLength, 5, 0);
        wave = stretches[current]->play(speed, 0.1, 4, 0);
        //		wave = stretches[current]->play2(pos, 0.1, 4);
        if (fft.process(wave)) {
            oct.calculate(fft.magnitudes);
        }
        
        //float tau = 0.4;
        //wallBuffer[i] += (output[i*nChannels]-wallBuffer[i])*tau;

        //play result
        if (playGrains){
            mymix.stereo(wave, outputs, 0.5);
            output[i*nChannels    ] += outputs[0];
            output[i*nChannels + 1] += outputs[1];
            
            //Normalizing by dividing by number of voices playing
            //output[i*nChannels ] /= numPlaying;
            //output[i*nChannels +1] /= numPlaying;
            
            temp = (outputs[0] + outputs[1])/2;
            avgRMS= avgRMS+temp;
        }
        else if(!playGrains) {
            //Normalizing by dividing by number of voices playing
            //output[i*nChannels ] /= numPlaying;
            //output[i*nChannels +1] /= numPlaying;
        }
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
        case 'i':
            bInstructions = !bInstructions;
            break;
        //Grab a screen shot
        case OF_KEY_RETURN:
            screenShot.grabScreen(0,0,ofGetWidth(),ofGetHeight());
            screenShot.saveImage("soundcave.png");
            break;
        //This one is for Maximilian stuff
        case OF_KEY_SHIFT:
            isTraining=!isTraining;
            //cout << isTraining;
            break;
            
        /*Disabling this for 1.0 presentation
        case 'p':
            playGrains = !playGrains;
            break;
            */
            
        //For user position sphere stuff
        case 'f':
            ofToggleFullscreen();
            break;
        case '8':
            bFill = !bFill;
            break;
        case '9':
            bDrawNormals = !bDrawNormals;
            break;
            
        /*For 1.0 performance, disabling wireframe toggle
        case '0':
            bWireframe = !bWireframe;
            break;
            
            */
        
        //Disabling changing view mode
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
        case '4':
            currentCam=3;
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
            
        //Test changing FOV in real time
        case 'x':
            cam[0].setFov(cam[0].getFov() + 5);
            break;
        case 'c':
            cam[0].setFov(cam[0].getFov() - 5);
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
            
        //Toggle the cave sounds
        case 'z':
            playCave = !playCave;
            if (playCave){
                cave.play();
            }
            else {
                cave.stop();
            }
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
    
    // If the selector is currently overlapping with a stalacmite
    // kill it.
    if (bOverlap){
        stalacs[overlapIndex].isDrawn=false;
    }
    
    // If not, draw a new one there
    else {
        //Position of selector when clicked is position to draw new stalacmite
        stalacs[nextToDraw].cylPos.setPosition(selector.getPosition());
        stalacs[nextToDraw].isDrawn=true;
    
        //Increment index of stalacmites
        if (nextToDraw++ >= MAX_STALAC) nextToDraw=0;
    }
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
