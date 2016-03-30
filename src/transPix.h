//
//  rain.h
//  LED_TREE
//
//  Created by Karina Jensen on 27/03/16.
//
//


#include "ofMain.h"
#include "defines.h"

class Pixel{
public:
    
    ofVec2f pos;
    ofVec2f vel;
    ofColor col;
    float acc = 0.8;
    float alpha = 255;
    bool moving = false;
    
    Pixel(ofVec2f _pos, ofColor _col) {
        pos = _pos;
        col = _col;
        acc = ofRandom(0.001,0.02);
        vel = ofVec2f(0, 0);
        
    }
    
    // Method to update location
    void update() {
        
        pos+=vel;
        vel.y -= acc ;
        
    }
    // Method to display
    void draw() {
        ofSetLineWidth(0.);
        ofSetColor(col.r,col.g,col.b,alpha);
        ofDrawRectangle(pos,RES_W/numTreePoles,2);
    }
    
    // Is the particle still useful?
    bool isDead() {
        if (pos.y<0) {
            return true;
        }
        else {
            return false;
        }
    }
    
};

class TransPix{
public:
    
    vector<Pixel> pix;
    float edge;
    float orgEdge;
    
    TransPix(vector<ofVec2f> _pos, vector<ofColor> _col) {
        
        for(int i = 0; i<_pos.size();i++){
            Pixel p(_pos[i],_col[i]);
            pix.push_back(p);
        }
    }
    
    // Method to update location
    void update() {
        edge +=0.1;
        for (vector<Pixel>::iterator it=pix.begin(); it!=pix.end();)    {
            if(it->pos.y<edge && ofRandom(1)>0.9){ //make them float from top
                it->moving = true;
                
            }
            if(it->moving){
                it->update();
            }
            if(edge > orgEdge + 10 && !it->moving){
                it->alpha =  sin(PI/2+(edge-orgEdge)*it->acc) *100  + 155;
            }else{
                it->alpha = 255;
            }
            
            
            if(it->isDead())
                it = pix.erase(it);
            else
                ++it;
        }
    }
    // Method to display
    void draw() {
        for(auto Pixel:pix){
            Pixel.draw();
        }
    }
    
    
    
    
};