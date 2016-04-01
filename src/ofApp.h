#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"
#include "ofxAutoReloadedShader.h"
#include "ofxPostProcessing.h"

#include "defines.h"
#include "blink.h"
#include "transPix.h"
#include "LampVis.h"
#include "rain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
//    void keyPressed(int key);
    void keyReleased(int key);
//    void mouseMoved(int x, int y );
//    void mouseDragged(int x, int y, int button);
//    void mousePressed(int x, int y, int button);
//    void mouseReleased(int x, int y, int button);
//    void mouseEntered(int x, int y);
//    void mouseExited(int x, int y);
//    void windowResized(int w, int h);
//    void dragEvent(ofDragInfo dragInfo);
//    void gotMessage(ofMessage msg);


    void exit();
    
    //functions for drawing gradient
    void ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode);
    //functions for warped image
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture, vector<float>vec);
    
    void setupGui();
    void setShaderVals();
    void setGraficVals();
    
    //transition at midnight
    vector<TransPix>transPix;
    bool transistionBegun;
    
    //shader stuff
    ofxAutoReloadedShader glowShader;
    ofxAutoReloadedShader dropShader;
    
    float counterCloud;
    float counterGlow;
    float drawnEdge;
    
    //grafiks
    vector<DataRain>rainDrops;
    vector<Blink> blinks;

    ofFbo render;
    ofFbo meshFbo;
    ofFbo lamps;
    LampVis lampVis;
    ofxSyphonServer syphon;
    ofxSyphonServer syphonLamp;
    ofxOscParameterSync syncOSC;

    //gui vars
    ofxPanel gui;
    ofParameterGroup enable;
    ofParameter<bool>masks;
    vector<ofParameter<float>>masksFader;
    
    ofParameterGroup guiGroup;
    
    ofParameterGroup mainControl;
    ofParameter<float> edge;
    ofParameter<bool>pixelPreview;
    ofParameter<bool>  glow;
    ofParameter<bool>  gradientColor;
    ofParameter<bool>  b_blink;
    ofParameter<bool>  syncColors;
    ofParameter<bool>  interActiveMasks;
    ofParameter<float>  intensityLamp;
    ofParameter<float>  disturbWave;
    ofParameter<bool>  transition;
    ofParameter<ofColor>  grafiks;
    ofParameter<ofColor>  textures;
    
    //background gradient colors
    ofParameterGroup backgroud_Grad;
    ofParameter<ofColor>Fx1ColorTopTop;
    ofParameter<ofColor>Fx1ColorTopBot;

    //blink
    ofParameter<float> blinkTempo;
    ofParameter<float> blinkIntensity;
    ofParameter<bool> hard_soft;
    ofParameter<ofColor> colorBlink;
    
    //shader GUI
    ofParameterGroup paramGeneral;
    
    ofParameterGroup paramCloud;
    ofParameter<float> tempoCloud;
    ofParameter<float> zoomCloud;
    ofParameter<float> balance;
    ofParameter<float> contrast;
    ofParameter<bool>  enableFBM;
    ofParameter<bool>  enableRMF;
    ofParameter<ofColor> cloudColor;
    
    ofParameterGroup paramGlow;
    ofParameter<float> tempoGlow;
    ofParameter<float> glowDensity;
    ofParameter<float> u_amount;
    ofParameter<bool> glowHorisontal;
    ofParameter<ofColor> glowColor;
    
    
};
