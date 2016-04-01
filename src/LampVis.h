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
    float vel;
    float width = 4;
    float tempo;
    float counter;
    float offSetY;
    bool show;
    float a,b,c;
    int pPosY;
    bool up = false;
    float intensity;
    Lamp() {

    }
    
    // Method to update location
    void update() {
        if(posY<0){
            show = false;
            up = false;
        }
        if(show){
            if(posY>LAMP_H/numLamps)up = true;
            
            if(!up)posY+=0.5;
            if(up)posY-=0.5;
            //if(vel >0.01)vel*=0.95;
        }
    }
    // Method to display
    
    void draw() {
        if(show){
            ofSetLineWidth(2);
            
            
//            if(int(posY)<posY){
//                a += vel * 255;
//            }
//            if(int(posY)!=pPosY){
//                pPosY = posY;
//                a = 0;
//            }

            //            ofSetColor(255,a);
//            ofDrawLine(0,int(posY)+offSetY+1,LAMP_W,int(posY)+offSetY+1);
//            ofSetColor(255,(255-a));
//            ofDrawLine(0,int(posY)+offSetY-1,LAMP_W,int(posY)+offSetY-1);
            ofSetColor(255);
            ofDrawLine(0,int(posY)+offSetY,LAMP_W,int(posY)+offSetY);
            
//            float alpha = 255;
//            for(int i = 0; i < 3 ; i++){
//                ofSetColor(255,alpha);
//                ofDrawLine(0,int(posY)+offSetY-i,LAMP_W,int(posY)+offSetY-i);
//                alpha -= 255/3;
//            }
        }
    }
    
};

class LampVis{
public:
    //vector<float> alpha;
    
    int tempo;
    float counter;
    ofxAutoReloadedShader shader;
    vector<Lamp>lamps;
    ofColor color;
    float a,b,c,d,e;
    
    LampVis() {
        shader.load("shaders/cloudH");
    }
    
    void setup(){
    
        for(int i = 0 ; i<numLamps;i++){
            Lamp l;
            l.posY = 0;
            l.offSetY = i*LAMP_H/numLamps;
            lamps.push_back(l);
        }
    }
    
    // Method to update location
    void update(float _temp,float zoom,float balance,float contrast,bool fbm,bool rmf, ofColor col, float intens) {
        a= zoom;
        b= balance;
        c= contrast;
        d= fbm;
        e= rmf;
        color = col;
        ;
        counter+=_temp;
////        if(counter>LAMP_H)counter = ofRandom(LAMP_H);
////  
////        
        for(int i = 0 ; i<lamps.size();i++){
            if(ofRandom(1)<intens && !lamps[i].show){
                lamps[i].show = true;
                lamps[i].posY = 0;
            }
            lamps[i].update();
        }
    }
    // Method to display
    void draw() {
        
        shader.begin();
        shader.setUniform2f("u_resolution",LAMP_W, LAMP_H);
        shader.setUniform1f("u_time", counter);
        
        shader.setUniform1f("u_zoom", a);
        shader.setUniform1f("u_balance", b);
        shader.setUniform1f("u_contrast", c+1.0);
        shader.setUniform1i("enableFBM", d);
        shader.setUniform1i("enableRMF", e);
        shader.setUniform1i("enableCircle", false);

        shader.setUniform3f("u_color", float(color.r)/255,float(color.g)/255,float(color.b)/255);
        ofSetColor(255,255,255);
        ofFill();
        ofDrawRectangle(0, 0, LAMP_W, LAMP_H);
        shader.end();
        
        for(int i = 0 ; i<lamps.size();i++){
            lamps[i].draw();
        }
//        ofSetLineWidth(2);
//        ofDrawLine(0,counter,LAMP_W,counter);
//        ofDrawLine(counter,0,counter,LAMP_H);
        

        
        
        
    }

    
};