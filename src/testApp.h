/*#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxCv.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber camera;
	
	ofxCv::FlowFarneback farneback;
	ofxCv::FlowPyrLK pyrLk;
	
	ofxCv::Flow* curFlow;
		
	ofxAutoControlPanel panel;
};
 */
#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxCv.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include <sys/time.h>

typedef hannWinFunctor grainPlayerWin;

class testApp : public ofBaseApp{
public:
    ~testApp();
	
	void setup();
	void update();
	void draw();
    void keyPressed(int key);

	void keyReleased(int key);
    ofVideoGrabber camera;
    ofVec2f averageFlow;
	ofxCv::FlowFarneback farneback;
	ofxCv::FlowPyrLK pyrLk;
	
	ofxCv::Flow* curFlow;
    
	ofxAutoControlPanel panel;
    ofImage HAlogo, bg, HAsnow;
    ofImage cameraColors;
    ofImage particleBuffer;
    ofImage error;
    int demo;
    //--part 2
    
    int numBubbles;
    float xSnow [1000]; 
    float ySnow [1000]; 
    vector<ofVec3f> particle;
    float dotSize;
    float dotRandomness;
    float flowNeeded;
    float prevAngle;
    float smoothAmount;
    float coneHeight;
    float brightnessFade;
    float brightnessFalloff;
    float spacing, spacing2, spacing3, spacing4, spacing5;
    
    //------
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
	void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
	float 	* lAudioOut; /* outputs */
	float   * rAudioOut;
    
	float * lAudioIn; /* inputs */
	float * rAudioIn;
	
	int		initialBufferSize; 
	int		sampleRate;
	
	
	/* stick your maximilian stuff below */
	
	double wave,sample,outputs[2];
	maxiSample samp, samp2, samp3, samp4, samp5;
	vector<maxiPitchStretch<grainPlayerWin>*> stretches;
	maxiMix mymix;
    maxiFilter myFilter;
	maxiPitchStretch<grainPlayerWin> *ts, *ts2, *ts3, *ts4, *ts5;
	double speed, grainLength, rate;
	maxiOsc osc;
	//ofxMaxiFFT fft;
	//ofxMaxiFFTOctaveAnalyzer oct;
	int current;
	double pos;
    
};
#endif


