//
//  bloubble.h
//  VardeWave_1
//
//  Created by Karina on 11/12/15.
//
//

#include "ofMain.h"
#include "defines.h"

class Spiral{
public:
    float posY = RES_H;
    float vel;
    float amount;
    float height;
    float counter = RES_W;
    ofColor spiralColor;

    
    Spiral() {
       // vel = 3;
       // amount = 2;
       // height = 10;
       // hypo = (sin(90)*height)/sin(angle);
        
        
    }
    
    // Method to update location
    void update() {
        counter -=vel;
        posY += vel;
        if(counter<0){
            counter=RES_W;
          //  posY-=height;
        }
        
    }
    // Method to display
    void draw() {
        //posY-=vel;
        
        ofSetColor(spiralColor);
        ofFill();
        ofSetLineWidth(1);
        
        ofVec2f pos = ofVec2f(0,posY);
        ofVec2f b = ofVec2f(RES_W,height+posY);
        ofVec2f c = pos.getInterpolated(b, counter/RES_W);
        
        for(int i = 1; i<amount;i++){
            ofDrawLine(0,posY+height*(i), RES_W,posY+(i+1)*height);
        }
        
        ofDrawLine(c.x,c.y, RES_W,height+posY);
        
        ofDrawLine(0,posY+height*amount, c.x,c.y+height*amount);
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(posY < 0){
            return true;
        } else {
            return false;
        }
    }
    
};