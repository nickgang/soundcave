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
    
    //Create Vector of Saws sound by the stalacmites
    vector<Tonic::Generator> tones;
    tones.resize(MAX_STALAC);
    
    vector<Tonic::Generator> envTones;
    envTones.resize(tones.size());
    
    for (int i=0;i<tones.size();i++) {
        
        //We need to concatenate index to these two strings
        string curMidi = "midiNumber";
        curMidi += (char) i;
        
        midiVect[i] = curMidi;
        
        string curTrig = "trigger";
        curTrig += (char) i;
        
        trigVect[i] = curTrig;
        
        midiNotes[i] = synth.addParameter(midiVect[i]);
        
        noteFreqs[i] = ControlMidiToFreq().input(midiNotes[i]);
        
        tones[i] = SawtoothWave().freq(noteFreqs[i]);
        
        //Filter the saws
        float filtFreq=10000;
        tones[i] = LPF24().input(tones[i]).Q(5).cutoff(filtFreq);
        
        envTriggers[i] = synth.addParameter(curTrig);
        
        //Send them through an envelope
        envTones[i] = tones[i] * ADSR().attack(0.75).decay(1.5).sustain(0).decay(0).trigger(envTriggers[i]).legato(true);
        
    }
    
    //Send all of the tones through a delay
    //Generator toneWithDelay = StereoDelay(0.5, 0.75).input(toneWithEnvelope).wetLevel(0.1).feedback(0.2);
    
    //Trying this here for now
    triggerTonic();
}

void ofApp::triggerTonic() {
    
    // set a parameter that we created when we defined the synth
    synth.setParameter("midiNumber", 44 + pitches[0]);
    
    // simply setting the value of a parameter causes that parameter to send a "trigger" message to any
    // using them as triggers
    synth.setParameter("trigger", 1);
}