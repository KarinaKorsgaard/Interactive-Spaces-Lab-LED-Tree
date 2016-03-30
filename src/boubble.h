//
//  bloubble.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"
#include "defines.h"

class Boubble{
public:
    ofVec2f location;
    ofVec2f velocity;
    ofColor boubbleColor;
    int trace;
    bool up;
    
    Boubble() {
        //velocity = ofVec2f(0., - ofRandom(0.1, 2.));
//        if(up){
//            location.y = RES_H;
//        }
//        if(!up){
//            location.y = 0;
//        }
    }
    
    // Method to update location
    void update() {
   
        location.operator+=(velocity);

    }
    // Method to display
    void draw() {
        
       // if(trace){

        int alpha = 255;
        int inc = 255/trace;
        ofFill();
        ofSetLineWidth(0.);
 
       
        for(int i = 0; i<trace;i++){
            ofSetColor(ofColor(boubbleColor.r,boubbleColor.g,boubbleColor.b,alpha));
            alpha-=inc;
            if(up){
                ofDrawRectangle(location.x,location.y+i, 1,1);
            }
            if(!up){
                ofDrawRectangle(location.x,location.y-i, 1,1);
            }
        }
   
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(location.y < 0 && up){
            return true;
        }
        if(location.y > RES_H && !up){
            return true;
        
        } else {
            return false;
        }
    }
    
};