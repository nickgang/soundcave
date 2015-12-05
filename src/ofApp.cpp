#include "ofApp.h"
#include "maximilian.h" //Granular library
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
    
    //----------STK setup ---------------------------------------
    stk::Stk::setRawwavePath(ofToDataPath("rawwaves",true));
    
    voicer = new stk::Voicer();
    reverb.setEffectMix(.3);
    reverb.setT60(8);
    
    lpf.setResonance(2000, 0.5);
    
    for (int i=0;i<100;i++) {
        stk::TubeBell *tBell = new stk::TubeBell;
        voicer->addInstrument(tBell);
    }
    
    c.noteNumber = 36;
    cs.noteNumber = 37;
    d.noteNumber = 38;
    ds.noteNumber = 39;
    e.noteNumber = 40;
    f.noteNumber = 41;
    fs.noteNumber = 42;
    g.noteNumber = 43;
    gs.noteNumber = 44;
    a.noteNumber = 45;
    as.noteNumber = 46;
    b.noteNumber = 47;
    
    cDown=false;
    fDown=false;
    
    //-----------------------------------------------------------
    
    //----------Stuff for the cameras-------------------------------
    dollyInc = 8;
    panInc = 90;
    boomInc = 1;
    wallCounter = 0;
    margin = 400;
    
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
    
    //Point Light Setup
    isLit=false;
    ofSetSmoothLighting(true);
    pointLight.setPosition(0,ROOM_HEIGHT*4,0);
    
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
    
    
    //Initialize all stalacmites to the same parameters for now
    for (int i=0;i<MAX_STALAC;i++){
        stalacs[i].cyl.set(2*width,0);
        
        stalacs[i].isDrawn = false;
        
        stalacs[i].xChord[0]=c;
        stalacs[i].xChord[1]=g;
        
        stalacs[i].zChord[0]=f;
        stalacs[i].zChord[1]=c;
        
        stalacs[i].xOctave=3;
        stalacs[i].zOctave=4;
        
    }
    maxHeight=10*width;
    isGrowing=false;
    
    //--------------------------------------------------------------
    
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
    
    //Update grain speed with sphere height
    //speed = -.75+userHeight*0.001;
    
    //Call the chord triggering function
    triggerChord();
    
    //Update filter cutoff based on sphere height
    lpf.setResonance(10*userHeight,0.5);
    
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
            pointLight.enable();
        }

    //Draw the room
    drawRoom();
        
        selector.draw();
        
        //This is drawing a cylinder in the middle of the room
        
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
    
        //Update target position
        moveTarget("border");

    ofPushMatrix();
    
    //Translate to where we need to draw the sphere
    ofTranslate(posNode.getPosition());
    
    //Draw user position ball
    drawSprite();
    
    ofPopMatrix();
    
    pointLight.disable();
        
    cam[currentCam].end();
        
    }
    
}

//-----------Trigger Chord Function-----------------------------
void ofApp::triggerChord(){
    float gain=50;
    
    for (int i=0;i<MAX_STALAC;i++){
        if (stalacs[i].isDrawn){
            if (abs(posNode.getX()-stalacs[i].cylPos.getX())<10){
                //Play note 1 of x chord
                stalacs[i].xChord[0].voiceTag = voicer->noteOn(stalacs[i].xChord[0].noteNumber+12*stalacs[i].xOctave,gain);
                
                //Play note 2 of x chord
                stalacs[i].xChord[1].voiceTag = voicer->noteOn(stalacs[i].xChord[1].noteNumber+12*stalacs[i].xOctave,gain);
                
                //Show normals for a frame
                bDrawNormals=true;
                
                }
            
            else if (abs(posNode.getZ()-stalacs[i].cylPos.getZ())<10){
                //Play note 1 of z chord
                stalacs[i].zChord[0].voiceTag = voicer->noteOn(stalacs[i].zChord[0].noteNumber+12*stalacs[i].zOctave,gain);
                //Play not 2 of z chord
                stalacs[i].zChord[1].voiceTag = voicer->noteOn(stalacs[i].zChord[1].noteNumber+12*stalacs[i].zOctave,gain);
                
                bDrawNormals=true;
                }
            else {
                bDrawNormals=false;
            }
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    float temp = 0;
    float scaleRMS=40;
    avgRMS = 0;
    
    //----------Stk output stuff--------------------------------------
    
    stk::StkFrames frames(bufferSize,2);
    stk::StkFrames value = voicer->tick(frames);
    
    
    //Send bell through filter
    lpf.tick(value);
    
    //Send filtered bell through the reverb
    stk::StkFrames reverbOut(bufferSize,2);
    reverb.tick(value,reverbOut,0,0);
    reverbOut.getChannel(0,value,0);
    
    
    
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
            output[i*nChannels    ] = outputs[0] + value(i,0); /* You may end up with lots of outputs. add them here */
            output[i*nChannels + 1] = outputs[1] + value(i,0);
        
            temp = (outputs[0] + outputs[1])/2;
            avgRMS= avgRMS+temp;
        }
        else if(!playGrains) {
            output[i*nChannels] = value(i,0);
            output[i*nChannels + 1] = value(i,0);
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
            
        //STK test stuff
        case 'z':
            if(!cDown){
                c.voiceTag = voicer->noteOn(c.noteNumber+12*octaveScale, gain);
                //ds.voiceTag = voicer->noteOn(ds.noteNumber+12*octaveScale, gain);
                g.voiceTag = voicer->noteOn(g.noteNumber+12*octaveScale, gain);
                cDown=true;
            }
            break;
        case 'x':
            if(!dsDown) {
                ds.voiceTag = voicer->noteOn(ds.noteNumber+12*octaveScale,gain);
                //fs.voiceTag = voicer->noteOn(fs.noteNumber+12*octaveScale,gain);
                as.voiceTag = voicer->noteOn(as.noteNumber+12*(octaveScale+1),gain);
                dsDown=true;
            }
            break;
        case 'c':
            if(!fDown) {
                f.voiceTag = voicer->noteOn(f.noteNumber+12*octaveScale,gain);
                //a.voiceTag = voicer->noteOn(a.noteNumber+12*octaveScale,gain);
                c.voiceTag = voicer->noteOn(c.noteNumber+12*(octaveScale+1),gain);
                fDown=true;
            }
            break;
            
            
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    float gain = 50;
    int octaveScale = 3;
    switch (key){
        case 'z':
            voicer->noteOff(c.noteNumber+12*octaveScale,gain);
            //voicer->noteOff(ds.noteNumber+12*octaveScale,gain);
            voicer->noteOff(g.noteNumber+12*octaveScale,gain);
            cDown= false;
            break;
        case 'x':
            voicer->noteOff(ds.noteNumber+12*octaveScale,gain);
            //voicer->noteOff(fsx.noteNumber+12*octaveScale,gain);
            voicer->noteOff(as.noteNumber+12*(octaveScale+1),gain);
            dsDown=false;
            break;
        case 'c':
            voicer->noteOff(f.noteNumber+12*octaveScale,gain);
            //voicer->noteOff(a.noteNumber+12*octaveScale,gain);
            voicer->noteOff(c.noteNumber+12*(octaveScale+1),gain);
            fDown=false;
            break;
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
