#include "testApp.h"

using namespace ofxCv;
using namespace cv;

testApp::~testApp() {
	delete ts, ts2;
}

void testApp::setup() {
	camera.initGrabber(320, 240);
	
	panel.setup(300, 800);
	
	panel.addPanel("Optical Flow");
	
	panel.addSlider("pyrScale", .5, 0, 1);
	panel.addSlider("levels", 4, 1, 8, true);
	panel.addSlider("winsize", 8, 4, 64, true);
	panel.addSlider("iterations", 2, 1, 8, true);
	panel.addSlider("polyN", 7, 5, 10, true);
	panel.addSlider("polySigma", 1.5, 1.1, 2);
	panel.addToggle("OPTFLOW_FARNEBACK_GAUSSIAN", false);
	
	panel.addToggle("useFarneback", true);
	panel.addSlider("winSize", 32, 4, 64, true);
	panel.addSlider("maxLevel", 3, 0, 8, true);
	
	panel.addSlider("maxFeatures", 200, 1, 1000);
	panel.addSlider("qualityLevel", 0.01, 0.001, .02);
	panel.addSlider("minDistance", 4, 1, 16);

    panel.addPanel("Demo 1");
    panel.addSlider("dotSize", 8.4,0,20);
    panel.addSlider("dotRandomness",0,0,20);
    panel.addSlider("flowNeeded", 5,1,20);
    panel.addSlider("spacing", 3,1,20);
    
    panel.addSlider("speed", 0,-30,30);
    panel.addSlider("grainLength",0,-1000,1000);
    // panel.addSlider("pos", 0,0.0,0.1);
    panel.addSlider("rate", 0,-30,30);
    
    panel.addPanel("Demo 2");
    panel.addSlider("brightnessFade",50, 0, 50);
    panel.addSlider("brightnessFalloff",5,0,50);
    panel.addSlider("dotSize2", 8.4,0,20);
    // panel.addSlider("dotRandomness",0,0,20);
    panel.addSlider("spacing2", 5,1,20);
    
    panel.addPanel("Demo 3");
    panel.addSlider("smoothAmount", 0.001,0.0,0.01);
    panel.addSlider("coneHeight", 38, 0, 300);
    panel.addSlider("spacing3", 10,1,20);
    
    panel.addPanel("Demo 4");
    panel.addSlider("spacing4", 5,1,20);
    
    
    panel.addPanel("Demo 5");
    panel.addSlider("spacing5", 20,1,40);
    
	curFlow = &farneback;
    
    
    error.loadImage("images/error.png");
    HAlogo.loadImage("images/mask.jpg");
    particleBuffer.allocate(640, 480,OF_IMAGE_GRAYSCALE);
    bg.loadImage("images/bg.jpg");
    ofEnableAlphaBlending();
    ofSetLineWidth(2);
    demo = 0;
    
    
    //----audio stuff
    
    
    samp.load(ofToDataPath("4.wav"));
	samp2.load(ofToDataPath("0.wav"));
	samp3.load(ofToDataPath("0.wav"));
	samp4.load(ofToDataPath("0.wav"));
	samp5.load(ofToDataPath("0.wav"));
     
    //	samp5.load(ofToDataPath("sine1sec.wav"));
	
    
	sampleRate 			= 44100; 
	initialBufferSize	= 512;	
	lAudioOut			= new float[initialBufferSize];
	rAudioOut			= new float[initialBufferSize];
	lAudioIn			= new float[initialBufferSize];
	rAudioIn			= new float[initialBufferSize];
	
	
	
	memset(lAudioOut, 0, initialBufferSize * sizeof(float));
	memset(rAudioOut, 0, initialBufferSize * sizeof(float));
	
	memset(lAudioIn, 0, initialBufferSize * sizeof(float));
	memset(rAudioIn, 0, initialBufferSize * sizeof(float));
	
	
    
	ts = new maxiPitchStretch<grainPlayerWin>(&samp);
	ts2 = new maxiPitchStretch<grainPlayerWin>(&samp2);
	ts3 = new maxiPitchStretch<grainPlayerWin>(&samp3);
	ts4 = new maxiPitchStretch<grainPlayerWin>(&samp4);
	ts5 = new maxiPitchStretch<grainPlayerWin>(&samp5);
     
	stretches.push_back(ts);
    
	stretches.push_back(ts2);
	stretches.push_back(ts3);
	stretches.push_back(ts4);
	stretches.push_back(ts5);
     
	speed = 1;
    rate = 2;
	grainLength = 0.05;
    pos = 0;
	current=0;
	
	//fft.setup(1024, 512, 256);
	//oct.setup(44100, 1024, 10);
	
    //current = 0;
	 ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
	ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, initialBufferSize, 4);
	
    

}

void testApp::update(){
	camera.update();
	
	if(camera.isFrameNew()) {
		
		
		if(panel.getValueB("useFarneback")) {
			curFlow = &farneback;
			farneback.setPyramidScale( panel.getValueF("pyrScale") );
			farneback.setNumLevels( panel.getValueF("levels") );
			farneback.setWindowSize( panel.getValueF("winsize") );
			farneback.setNumIterations( panel.getValueF("iterations") );
			farneback.setPolyN( panel.getValueF("polyN") );
			farneback.setPolySigma( panel.getValueF("polySigma") );
			farneback.setUseGaussian(panel.getValueB("OPTFLOW_FARNEBACK_GAUSSIAN"));
			dotSize = panel.getValueF("dotSize");
            dotRandomness = panel.getValueF("dotRandomness");
            flowNeeded = panel.getValueF("flowNeeded");
            smoothAmount = panel.getValueF("smoothAmount");
            coneHeight = panel.getValueF("coneHeight");
            
            brightnessFade = panel.getValueF("brightnessFade");
            brightnessFalloff = panel.getValueF("brightnessFalloff");
            
            spacing = panel.getValueF("spacing");
            spacing2 = panel.getValueF("spacing2");
            spacing3 = panel.getValueF("spacing3");
            spacing4 = panel.getValueF("spacing4");
            spacing5 = panel.getValueF("spacing5");

		} else {
			curFlow = &pyrLk;
			pyrLk.setMaxFeatures( panel.getValueI("maxFeatures") );
			pyrLk.setQualityLevel( panel.getValueF("qualityLevel") );
			pyrLk.setMinDistance( panel.getValueF("minDistance") );			 	
			pyrLk.setWindowSize( panel.getValueI("winSize") );
			pyrLk.setMaxLevel( panel.getValueI("maxLevel") );
		}
        
        if(panel.getCurrentPanelName()== "Optical Flow"){
            demo = 0;
        }
        if(panel.getCurrentPanelName()== "Demo 1"){
            demo = 1;
        } 
        if(panel.getCurrentPanelName()== "Demo 2"){
            demo = 2;
        } 
        if(panel.getCurrentPanelName()== "Demo 3"){
            demo = 3;
        } 
        if(panel.getCurrentPanelName()== "Demo 4"){
            demo = 4;
        } 
        
        if(panel.getCurrentPanelName()== "Demo 5"){
            demo = 5;
        } 
		
		//check it out that that you can use Flow polymorphically
		curFlow->calcOpticalFlow(camera);
	}
}

void testApp::draw(){
	bg.draw(0,0);
		
	ofSetColor(255);
	camera.draw(400,100,640,480);
    ofVec2f flow = farneback.getAverageFlow();
    pos = double(ofMap(flow.y, -3,3,0,0.1));
	curFlow->draw(400,100,640,480);
	
}

void testApp::audioRequested (float * output, int bufferSize, int nChannels){
	for (int i = 0; i < bufferSize; i++){
        //		wave = stretches[current]->play(speed, grainLength, 5, 0);
		//wave = stretches[current]->play(speed*2, rate, 0.1, 4, 0);
        wave = stretches[current]->play2(pos, 0.1, 4);
        /*	if (fft.process(wave)) {
         oct.calculate(fft.magnitudes);
         }
         */
		//play result
		mymix.stereo(wave, outputs, 0.5);
        
		lAudioOut[i] = output[i*nChannels    ] = outputs[0]; 
		rAudioOut[i] = output[i*nChannels + 1] = outputs[1];
	}
}

//--------------------------------------------------------------
void testApp::audioReceived (float * input, int bufferSize, int nChannels){	
	
	for (int i = 0; i < bufferSize; i++){
		
		lAudioIn[i] = input[i*2];
		rAudioIn[i] = input[i*2+1];
	}
}
void testApp::keyPressed(int key){
	switch (key) {
            
        case 'a':
		case 'A':
			current = 0;
			break;
		case 's':
		case 'S':
			current = 1;
			break;
		case 'd':
		case 'D':
			current = 2;
			break;
		case 'f':
		case 'F':
			current = 3;
			break;
		case 'g':
		case 'G':
			current = 4;
			break;
            
	}
    
}
void testApp::keyReleased(int key){
    if(key == 'h'){
        panel.hide();
    } 
    if(key == 's'){
        panel.show();
    }
    if(key == '0'){
        panel.setSelectedPanel(0);
    }
    
    if(key == '1'){
        panel.setSelectedPanel(1);
    }
    
    if(key == '2'){
        panel.setSelectedPanel(2);
    }
    
    if(key == '3'){
        panel.setSelectedPanel(3);
    }
    
    if(key == '4'){
        panel.setSelectedPanel(4);
    }
    
    if(key == '5'){
        panel.setSelectedPanel(5);
    }
}


