//
//  rain.h
//  LED_TREE
//
//  Created by Karina Jensen on 27/03/16.
//
//


#include "ofMain.h"
#include "defines.h"

class DataRain{
public:
    
    ofVec2f pos;
    ofVec2f vel;
    float acc = 0.1;
    float edge;
    ofColor color;
    int num;
    float point;
    bool updatePoint;
    float counter = 0;
    float damp = 1;
    
    DataRain() {
       // pos = ofVec2f(int(ofRandom(RES_W)),0);
        point = 0;
        updatePoint = false;
        acc = ofRandom(0.01,0.08);
    }
    
    // Method to update location
    void update(float _edge) {
        if(!updatePoint){
            edge = _edge;
            pos += vel;
            vel.y+=acc;
        }
        
        if(pos.y>edge*RES_H && !updatePoint){
          //  vel.y *= -ofRandom(.2,.3);
          //  pos.y = edge*RES_H;
            updatePoint = true;
        }
        if(updatePoint){
            counter++;
            if(damp>0)damp -= 0.008;
            pos.y = ((sin(counter / 15))*vel.y*5 ) * damp + ( edge*RES_H);
            point = 1-( pos.y/(edge*RES_H) );
            //pos.y = ((1-sin(counter / 10))*50 )*damp + ( edge*RES_H);
        }
    }
    // Method to display
    void draw() {
        if(!updatePoint){
            int alpha = 255;
            int inc = 255/10;
            ofFill();
            ofSetLineWidth(0.);
            
            for(int i = 0; i<10;i++){
                ofSetColor(ofColor(color.r,color.g,color.b,alpha));
                alpha-=inc;
                ofDrawRectangle(pos.x,pos.y-i, RES_W/(numTreePoles),1);
            }
        }
        if(updatePoint){
            ofFill();
            ofSetLineWidth(0.);
            int alpha = damp*255;
            ofSetColor(ofColor(color.r,color.g,color.b,alpha));
            
            float p = point * RES_H + edge*RES_H; //1-( pos.y/(edge*RES_H) );
            
            ofDrawRectangle(pos.x,p, RES_W/numTreePoles,1);
            
        }
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(damp<0){
            return true;
        }
        else {
            return false;
        }
    }
    
};