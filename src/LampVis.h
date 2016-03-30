//
//  LampVis.h
//  LED_TREE
//
//  Created by Karina Jensen on 30/03/16.
//
//



#include "ofMain.h"
#include "defines.h"


class LampVis{
public:
    vector<float> alpha;
    
    float tempo;
    float counter;
    ofxAutoReloadedShader shader;
    
    LampVis() {
        alpha.resize(numLamps);
        tempo = 0.1;
        shader.load("shaders/cloud");
    }
    
    // Method to update location
    void update(float _temp) {
        tempo = _temp;
        
    }
    // Method to display
    void draw() {
        counter+=tempo;
        shader.begin();
        shader.setUniform2f("u_resolution",LAMP_W, LAMP_H);
        shader.setUniform1f("u_time", counter);
        shader.setUniform1f("u_zoom", 0.5);
        shader.setUniform1f("u_balance", 0.5);
        shader.setUniform1f("u_contrast", 1+1.0);
        shader.setUniform1i("bwSwitch", false);
        shader.setUniform1i("bgTransparent", true);
        shader.setUniform1i("enableFBM", true);
        shader.setUniform1i("enableRMF", true);
        shader.setUniform3f("u_color", 1,1,1);
        ofSetColor(255,255,255);
        ofFill();
        ofDrawRectangle(0, 0, LAMP_W, LAMP_H);
        shader.end();
        
        
        
    }

    
};