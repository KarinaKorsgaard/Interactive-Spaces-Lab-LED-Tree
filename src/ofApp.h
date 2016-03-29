#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"
#include "ofxAutoReloadedShader.h"
#include "ofxPostProcessing.h"

#include "defines.h"
#include "boubble.h"
#include "dancingLine.h"
#include "larve.h"
//#include "wave.h"
#include "spiral.h"
#include "blink.h"
#include "rain.h"
#include "waveSystem.h"
#include "sea.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxPanel gui;

    ofParameterGroup enable;
    ofParameterGroup guiGroup;
    
    ofParameter<bool>masks;
    vector<ofParameter<float>>masksFader;
    ofxOscParameterSync syncOSC;
    
    ofParameterGroup backgroud_Grad;
    ofParameter<ofColor>Fx1ColorTopTop;
    ofParameter<ofColor>Fx1ColorBotTop;
    ofParameter<ofColor>Fx1ColorTopBot;
    ofParameter<ofColor>Fx1ColorBotBot;
    
    ofParameterGroup mainControl;
    
    ofParameter<float> edge;
    ofParameter<float> waveEdge;
    ofParameter<bool>flames;
    ofParameter<bool>  perlin;
    ofParameter<bool>  glow;
    ofParameter<bool>  cloud;
    ofParameter<bool>  drops;
    ofParameter<bool>  gradientColor;
    ofParameter<bool>  b_larve;
    ofParameter<bool>  b_bubbles;
    ofParameter<bool>  b_lines;
    ofParameter<bool>  b_blink;
    ofParameter<bool>  b_spiral;
    ofParameter<bool>  syncColors;
    ofParameter<bool>  interActiveMasks;
    ofParameter<bool>  interActiveWave;
    ofParameter<bool>  interActiveSea;
    ofParameter<float>  intensity;
    ofParameter<float>  disturbWave;
    ofParameter<bool>  bwSwitch;
    
    ofParameter<ofColor>  grafiks;
    ofParameter<ofColor>  textures;

    //Wave theWave;
    
    
    ofParameterGroup waveControl;
    ofParameter<float> dLinesSpeed;
    ofParameter<float> dLinesSync;
    ofParameter<ofColor> colorDLines;
    vector<DancingLine> dancingLines;
    float counterDLines;
    ofFbo fboDancingLines;
    
    ofParameter<float> boubblesIntensity;
    ofParameter<float> boubblesVelMin;
    ofParameter<float> boubblesVelMax;
    ofParameter<ofColor> colorBoubbles;
    vector<Boubble> boubbles;
    ofFbo bubblesFbo;
    
    ofParameter<bool> spiralIntensity;
    ofParameter<float> spiralAngle;
    ofParameter<int> spiralAmount;
    ofParameter<ofColor> colorSpiral;
    
    ofParameter<float> larveIntensity;
    ofParameter<float> larveVelMin;
    ofParameter<float> larveVelMax;
    ofParameter<float> larveMaxLength;
    ofParameter<float> larveMinLength;
    ofParameter<ofColor> colorLarve;
    // BlinkendeLygter
    vector<Larve> larves;
    ofFbo larveFbo;
    
    //ofParameter<float> waveRange;
    //ofParameter<int> waveDivisions;

    
    ofFbo render;
    ofxSyphonServer syphon;
    void ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode);
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture, vector<float>vec);
    
    void setupGui();
    
    void setShaderVals();
    void setGraficVals();
    void shaderNotWarped();
    
    ofFbo meshFbo;
    ofFbo secondMesh;
    //shader stuff
    ofxAutoReloadedShader perlinShader;
    ofxAutoReloadedShader cloudShader;
    ofxAutoReloadedShader flamesShader;
    ofxAutoReloadedShader glowShader;
    ofxAutoReloadedShader dropShader;
    
    ofParameterGroup paramGeneral;

//    
    ofParameterGroup paramFlames;
    ofParameter<bool> enableFlames;
    ofParameter<float> flamesTempo;
    ofParameter<float> flameSize;
    ofParameter<float> flameAmountX;
    ofParameter<float> flameAmountY;
    ofParameter<ofColor> flameColor;
    
    ofParameterGroup paramPerlin;
    ofParameter<float> tempoPerlin;
    ofParameter<float> zoomPerlin;
    ofParameter<bool> horizontalPerlin;
    
//    ofParameter<float> FirstDivision;
//    ofParameter<float> rotationSpeed;
//    ofParameter<float> fallingSpeed;
//    ofParameter<float> rotCenterX;
//    ofParameter<float> rotCenterY;
    
    ofParameterGroup paramCloud;
    ofParameter<float> tempoCloud;
    ofParameter<float> zoomCloud;
    ofParameter<float> balance;
    ofParameter<float> contrast;
    ofParameter<bool>  enableFBM;
    ofParameter<bool>  enableRMF;
    ofParameter<ofColor> cloudColor;
//    ofParameter<bool>  enableCircle;
    
    ofParameterGroup paramDrops;
    ofParameter<float> tempoDrops;
    ofParameter<float> offSetDrops;
    ofParameter<float> colorThresDrops;
    ofParameter<float> contrastDrops;
    ofParameter<float>  waveSizeDrops;
    ofParameter<bool>  inverseDrops;
    ofParameter<ofColor> colorDrops;
    
    
    ofParameterGroup paramGlow;
    ofParameter<float> tempoGlow;
    ofParameter<float> glowDensity;
    ofParameter<float> u_amount;
    ofParameter<bool> glowHorisontal;
    ofParameter<ofColor> glowColor;
    
    
    float counterFlames;
    float counterPerlin;
    float counterCloud;
    float counterGlow;
    float counterDrops;

    
    // Blinkende Lyger
    // GUI / Controlpanel
    ofParameter<float> blinkTempo;
    ofParameter<float> blinkIntensity;
    ofParameter<bool> hard_soft;
    ofParameter<ofColor> colorBlink;
    // BlinkendeLygter
    vector<Blink> blinks;
    
    
    ofParameter<float> inLeft;
    ofParameter<float> inRight;
    ofParameter<float> noiseAmt;
    
    ofParameter<bool>pixelPreview;
    
    vector<Spiral> spirals;
    vector<Rain>rainDrops;
    WaveParticleSystem waveSystem;
    Sea seaSystem;
    float pWaveEdge = 0;
    int waveCounter = 0;
};
