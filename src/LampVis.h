//
//  LampVis.h
//  LED_TREE
//
//  Created by Karina Jensen on 30/03/16.
//
//



#include "ofMain.h"
#include "defines.h"


class Lamp{
public:
    float posY;
    float width = 4;
    float tempo;
    float alpha = 255;
    float counter;
    
    Lamp() {
        posY = 0;
        tempo = 0.1;
    }
    
    // Method to update location
    void update() {
        
        counter+=tempo;
        
        posY += abs(sin(counter/10))*(tempo+0.5);
    
        if(posY > LAMP_H){
            posY = 0;
            width = ofRandom(1,10);
            tempo = ofRandom(0.01,0.05);
        }
    }
    // Method to display
    
    void draw() {
        ofSetColor(255);
        
        ofDrawRectangle(0,posY,LAMP_W,width);
    }
    
};

class LampVis{
public:
    //vector<float> alpha;
    
    int tempo;
    float counter;
    ofxAutoReloadedShader shader;
    vector<Lamp>lamps;
    
    LampVis() {

    }
    
    void setup(){
    
        for(int i = 0 ; i<15;i++){
            Lamp l;
            l.posY = ofRandom(LAMP_H);
            lamps.push_back(l);
        }
    }
    
    // Method to update location
    void update(float _temp) {
        counter+=0.2;
        if(counter>LAMP_H)counter = ofRandom(LAMP_H);
  
        
        for(int i = 0 ; i<lamps.size();i++){
            lamps[i].update();
        }
    }
    // Method to display
    void draw() {
        ofBackground(50);
        for(int i = 0 ; i<lamps.size();i++){
            lamps[i].draw();
        }
        ofSetLineWidth(2);
        ofDrawLine(0,counter,LAMP_W,counter);
        ofDrawLine(counter,0,counter,LAMP_H);
//        counter+=tempo;
//        shader.begin();
//        shader.setUniform2f("u_resolution",LAMP_W, LAMP_H);
//        shader.setUniform1f("u_time", counter);
//        shader.setUniform1f("u_zoom", 0.5);
//        shader.setUniform1f("u_balance", 0.5);
//        shader.setUniform1f("u_contrast", 1+1.0);
//        shader.setUniform1i("bwSwitch", false);
//        shader.setUniform1i("bgTransparent", true);
//        shader.setUniform1i("enableFBM", true);
//        shader.setUniform1i("enableRMF", true);
//        shader.setUniform3f("u_color", 1,1,1);
//        ofSetColor(255,255,255);
//        ofFill();
//        ofDrawRectangle(0, 0, LAMP_W, LAMP_H);
//        shader.end();
        
        
        
    }

    
};