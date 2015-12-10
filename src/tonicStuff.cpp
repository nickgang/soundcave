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
    
    // create a named parameter on the synth which we can set at runtime
    vector<ControlGenerator> midiNotes;
    midiNotes.resize(MAX_STALAC);
    midiVect.resize(midiNotes.size());

    // convert a midi note to a frequency (plugging that parameter into another object)
    vector<ControlGenerator> noteFreqs;
    noteFreqs.resize(midiNotes.size());
    
    
    // Create trigger messages for each ADSR
    vector<ControlGenerator> envTriggers;
    envTriggers.resize(midiNotes.size());
    trigVect.resize(midiNotes.size());
    
    // Create cutoff freq message for each filter
    vector<ControlGenerator> cutoffs;
    cutoffs.resize(midiNotes.size());
    freqVect.resize(midiNotes.size());
    
    // Create Amp Env Release message for killing notes
    vector<ControlGenerator> releases;
    releases.resize(midiNotes.size());
    relVect.resize(midiNotes.size());
    
    //Create Vector of Saws sound by the stalacmites
    vector<Tonic::Generator> tones;
    tones.resize(MAX_STALAC);
    
    //Create generator vector to hold 16 voices post envelope
    vector<Tonic::Generator> envTones;
    envTones.resize(tones.size());
    
    //An adder object to sum the 16 voices and a generator to hold the output
    Tonic::Adder outputSum;
    Tonic::Generator summedSaws;
    
    for (int i=0;i<tones.size();i++) {
        
        //We need to concatenate index to these four strings
        std::ostringstream curMidi;
        std::ostringstream curTrig;
        std::ostringstream curFreq;
        std::ostringstream curRel;
        
        
        curMidi << "midiNumber" << (i+1);
        curTrig << "trigger" << (i+1);
        curFreq << "cutoff" << (i+1);
        curRel << "release" << (i+1);
        
        //Store the stringstreams as regular strings in vectors
        midiVect[i] = curMidi.str();
        trigVect[i] = curTrig.str();
        freqVect[i] = curFreq.str();
        relVect[i] = curRel.str();
        
        //Get frequency for each voice
        midiNotes[i] = synth.addParameter(midiVect[i]);
        noteFreqs[i] = ControlMidiToFreq().input(midiNotes[i]);
        tones[i] = SawtoothWave().freq(noteFreqs[i]);
        
        //Filter the saws
        cutoffs[i]= synth.addParameter(freqVect[i]);
        
        tones[i] = LPF24().input(tones[i]).Q(3).cutoff(cutoffs[i]);
        
        releases[i] = synth.addParameter(relVect[i]);
        envTriggers[i] = synth.addParameter(trigVect[i]);
        
        //Send them through an envelope
        envTones[i] = tones[i] * ADSR().attack(1.0).decay(0).sustain(1).release(releases[i]).trigger(envTriggers[i]).legato(true);
        
        //Mix each subequent signal with an adder
        outputSum.input(envTones[i]);
    }
    
    //Store the sum in a generator
    summedSaws = outputSum;
    
    // Send all of the tones through a delay
    //Generator toneDelay = StereoDelay(0.5, 0.75).input(summedSaws).wetLevel(0.1).feedback(0.2);
    
    // Send it through reverb
    Generator toneReverb = Reverb().input(summedSaws).roomSize(10).dryLevel(0.1).wetLevel(0.8);
    
    Generator toneComp = Compressor().attack(0.5).release(0.06).threshold(-15).ratio(15).input(toneReverb);
    
    synth.setOutputGen(toneComp);
    
    
    
}

void ofApp::triggerTonic() {
    
    //Loop through
    for (int i=0;i<MAX_STALAC;i++){
        if (stalacs[i].isDrawn && !isTriggered[i]){
            
            synth.setParameter(relVect[i],1);
            synth.setParameter(midiVect[i], stalacs[i].octave*12 + stalacs[i].pitch);
            synth.setParameter(trigVect[i], 1);
            
            
            isTriggered[i]=true;
        }
        
        else if (!stalacs[i].isDrawn && isTriggered[i]) {
            synth.setParameter(relVect[i],0);
            
            isTriggered[i]=false;
        }
    }
    
    
    // simply setting the value of a parameter causes that parameter to send a "trigger" message to any
    // using them as triggers
    
}

void ofApp::updateFilters() {
    
    float maxDistance = sqrt(ROOM_DEPTH^2+ROOM_WIDTH^2);
    
    // Calculate distance from sphere to each stalacmite
    float distances[MAX_STALAC][2];
    float hypotenuse[MAX_STALAC];
    
    for (int i=0;i<MAX_STALAC;i++){
        //Fill distances matrix with euclidean distance to stalacs in x and z direction
        distances[i][0] = abs(posNode.getX() - stalacs[i].cylPos.getX());
        distances[i][1] = abs(posNode.getZ() - stalacs[i].cylPos.getZ());
        
        hypotenuse[i]=sqrt(distances[i][0]*distances[i][0]+distances[i][1]*distances[i][1]);
        
        // epsilon
        float eps = 0.0001;
        
        //Send filter cutoff messages to the synth object
        synth.setParameter(freqVect[i], (maxDistance/hypotenuse[i])*15000);
        
    }
    
    
    
    //For Debugging
    //cerr << (maxDistance/hypotenuse[i])*15000) << endl;
    
}


