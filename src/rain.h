//
//  rain.h
//  LED_TREE
//
//  Created by Karina Jensen on 27/03/16.
//
//


#include "ofMain.h"
#include "defines.h"

class Rain{
public:
    
    ofVec2f pos;
    ofVec2f vel;
    float acc = 0.1;
    float height;
    ofColor color;
    Rain() {
        pos = ofVec2f(int(ofRandom(RES_W)),0);
    }
    
    // Method to update location
    void update() {
        pos += vel;
        vel.y+=acc;
        if(pos.y>height*RES_H){
            vel.y *= -ofRandom(.2,.3);
            pos.y = height*RES_H;
        }
    }
    // Method to display
    void draw() {
        ofSetColor(color);
        ofDrawRectangle(pos,2,2);
    }
    
    // Is the particle still useful?
    bool isDead() {
        if (abs(vel.y) < 0.1 && abs(pos.y-RES_H*height)<1) {
            return true;
        } else {
            return false;
        }
    }
    
};