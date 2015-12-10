#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "ofxOsc.h"
//#include "ofxStk.h"
#include "ofxTonic.h"
#include <sys/time.h>
#include <sstream>

#define HOST "localhost"
#define RECEIVEPORT 12000
#define SENDPORT 6448

#define MY_SRATE 44100      //Sample Rate
#define MY_BUFFER_SIZE 512  //Buffer Size
#define MY_PIE      3.1416

//Global dimensions of the room
#define ROOM_HEIGHT 2000
#define ROOM_WIDTH  6000
#define ROOM_DEPTH  3000
#define NUM_LINES   40

#define NUM_CAMS    3
#define MAX_STALAC  16

using namespace Tonic;

typedef hannWinFunctor grainPlayerWin;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        //Method for drawing background 'room'
        void drawRoom();
        //Method for drawing user position sphere
        void drawSprite();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioOut(float * output, int bufferSize, int nChannels);
        void audioIn(float * input, int bufferSize, int nChannels);
    
    
    //Vector to store buffers used to draw waveforms in walls
    vector<float> wallBuffer;
    
    int	bufferSize;
    
    ofTrueTypeFont title;
    ofTrueTypeFont subTitle;
    bool menuScreen;
    void drawMenuScreen();
		
    //Some variables for changing sample rate and buffer size
    int	initialBufferSize;
    int	sampleRate;
    
    float avgRMS;
    
    //Declaration of Maximilian Stuff
    
    double wave,sample,outputs[2];
    maxiSample samp, samp2, samp3, samp4, samp5;
    vector<maxiTimestretch<grainPlayerWin>*> stretches;
    maxiMix mymix;
    maxiTimestretch<grainPlayerWin> *ts, *ts2, *ts3, *ts4, *ts5;
    double speed, grainLength;
    
    ofxMaxiFFT fft;
    ofxMaxiFFTOctaveAnalyzer oct;
    
    //Which audio file is playing
    int current;
    
    double pos;
    
    //osc
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    bool isTraining;
    bool playGrains;
    
    ofSoundPlayer cave;
    
    //-----------------Tonic Stuff-----------------------
    
    // Instance of main tonic synth class
    Tonic::ofxTonicSynth synth;
    
   // Tonic message arrays
    vector<string> midiVect;
    vector<string> trigVect;
    vector<string> freqVect;
    vector<string> relVect;
    
    //Mixer for the output
    Mixer sawMix;
    
    //These will hold buffers for 16 voices
    vector <float *> voice;

    
    
    void setupTonic();
    void triggerTonic();
    void updateFilters();
    
    
    //---- Lots of stuff for the user position ball -----
    
    ofIcoSpherePrimitive userPos;
    
    void moveTarget(string spherePath);
    
    float userHeight;
    
    int viewMode;
    
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bInfoText;
    bool bMousePressed;
    bool bSplitFaces;
    
    ofMaterial material;
    
    ofLight pointLight1;
    ofLight pointLight2;
    ofLight pointLight3;
    ofLight pointLight4;
    
    bool isLit;
    
    vector<ofMeshFace> triangles;
    
    //Trying to track/change position of sphere with an ofNode
    ofNode posNode;
    ofNode target;
    
    float slew;
    ofVec3f slewCoords;
    
    float xInc;
    float yInc;
    float zInc;
    
    
    //--------------------------------------------------
    
    //---------------Cylinder Stuff---------------------
    
    //May not need this node
    ofNode selectNode;
    
    //Plane that will determine where to pop up a new stalacmite
    ofPlanePrimitive selector;
    void setupSelector();
    void interpSelector();
    
    void detectOverlap();
    bool bOverlap;
    int overlapIndex;
    
    int nextToDraw;
    ofVec3f nextPos;
    
    //This is using the real stalacmite class
    //Needs to be implemented elsewhere in ofApp
    //stalacmite stalacs[MAX_STALAC];
    
    float maxHeight;
    bool isGrowing;
    
    // Old stalacmite struct, need to get rid of this
    struct Stalacmite {
        ofCylinderPrimitive cyl;
        ofNode cylPos;
        int pitch;
        int octave;
        bool isDrawn;
        
    };
    
    bool isTriggered[MAX_STALAC];
    
    Stalacmite stalacs[MAX_STALAC];
    ofNode stalacNode[MAX_STALAC];
    
    
    //----------Stuff for the camera--------------------
    ofCamera cam[NUM_CAMS];
    ofCamera singleCam;
    ofNode camNode[NUM_CAMS];
    int currentCam;
    
    float dollyInc;
    float panInc;
    float boomInc;
    int wallCounter;
    
    int margin;
    //--------------------------------------------------

};
