//
//  tonicStuff.cpp
//  Sound_Cave
//
//  Created by Nicholas Gang on 12/7/15.
//
//

#include <stdio.h>
#include "ofApp.h"
#include "maximilian.h" //Granular library
#include "ofxTonic.h"
#include "time.h"


void ofApp::setupTonic() {
    //-----------------------Tonic Setup----------------------------
    
    //Setup vector to hold buffers
    voice.resize(MAX_STALAC);
    
    //Fill pitch vector with chormatic degrees
    pitches.resize(12);
    
    for (int i=0;i<pitches.size();i++) {
        pitches[i]=i;
    }
    
    // create a named parameter on the synth which we can set at runtime
    vector<ControlGenerator> midiNotes;
    midiNotes.resize(MAX_STALAC);
    midiVect.resize(midiNotes.size());

     // convert a midi note to a frequency (plugging that parameter into another object)
    vector<ControlGenerator> noteFreqs;
    noteFreqs.resize(midiNotes.size());
    
    
    //Create trigger messages for each ADSR
    vector<ControlGenerator> envTriggers;
    envTriggers.resize(midiNotes.size());
    trigVect.resize(midiNotes.size());
    
    //Create Vector of Saws sound by the stalacmites
    vector<Tonic::Generator> tones;
    tones.resize(MAX_STALAC);
    
    //Create generator vector to hold 16 voices post envelope
    vector<Tonic::Generator> envTones;
    envTones.resize(tones.size());
    
    Tonic::Adder outputSum;
    Tonic::Generator summedSaws;
    
    for (int i=0;i<tones.size();i++) {
        
        //We need to concatenate index to these two strings
        std::ostringstream curMidi;
        curMidi << "midiNumber" << (i+1);
        
        //Store the stringstream as a regular string in a vector
        midiVect[i] = curMidi.str();
        
        std::ostringstream curTrig;
        curTrig << "trigger" << (i+1);
        
        trigVect[i] = curTrig.str();
        
        midiNotes[i] = synth[i].addParameter(midiVect[i]);
        
        noteFreqs[i] = ControlMidiToFreq().input(midiNotes[i]);
        
        tones[i] = SawtoothWave().freq(noteFreqs[i]);
        
        //Filter the saws
        float filtFreq=10000;
        tones[i] = LPF24().input(tones[i]).Q(3).cutoff(filtFreq);
        
        envTriggers[i] = synth[i].addParameter(trigVect[i]);
        
        //Send them through an envelope
        envTones[i] = tones[i] * ADSR().attack(0.90).decay(0).sustain(1).release(1).trigger(envTriggers[i]).legato(true);
        
        //synth[i].setOutputGen(envTones[i]);
        
        //Mix
        outputSum.input(envTones[i]);
    }
    
    summedSaws = outputSum;
    
    //Send all of the tones through a delay
    Generator toneDelay = StereoDelay(0.5, 0.75).input(summedSaws).wetLevel(0.1).feedback(0.2);
    
    synth[0].setOutputGen(toneDelay);
    
    
    
}

void ofApp::triggerTonic() {
    
    //Loop through
    for (int i=0;i<MAX_STALAC;i++){
        if (stalacs[i].isDrawn && !isTriggered[i]){
            synth[i].setParameter(midiVect[i], 24 + pitches[0]);
            synth[i].setParameter(trigVect[i], 1);
            
            
            isTriggered[i]=true;
        }
    }
    
    
    // simply setting the value of a parameter causes that parameter to send a "trigger" message to any
    // using them as triggers
    
}