#include "ofMain.h"
#include "defines.h"

class Larve{
public:
    float x;
    float a,b;
    ofVec2f velocity;
    ofColor larveColor;
    bool moveFront;
    bool moveBack;
    float length;
    
    Larve() {
        velocity = ofVec2f(0., - ofRandom(0.1, 2.));
        length = ofRandom(7,10);
        x = ofRandom(RES_W);
        a = RES_H;
        b = RES_H+length;
        moveFront = true;
        moveBack = false;
    }
    
    // Method to update location
    void update() {
        
        float vel = velocity.y + velocity.y * sin(abs(a-b)/length*PI);
        if(moveFront){
            //move front of larve while b stands still until reach lengt
            
            a+=vel;
            
            if(a<=b-length){
                moveBack = true;
                moveFront = false;
            }
            
        }if(moveBack){
            
            b+=vel;
            
            if(b<a){
                moveBack = false;
                moveFront = true;
            }
        }
    }
    // Method to display
    void draw() {
        int pixels = abs(a-b);
        int alpha = 255;
        
        for(int i = 0; i< pixels + 1 ; i+=3){
            if(i<pixels/2 && alpha>30){
                alpha*=0.9;
            }else if(i>=pixels/2 && alpha<255){
                alpha*=1.1;
            }
            
            ofSetColor(larveColor.r,larveColor.g,larveColor.b,alpha);
            ofDrawRectangle(x, a+i, 2, 3);
            ofDrawRectangle(x, a+(pixels-i), 2, 3);
        }
        ofSetColor(larveColor);
        //ofDrawRectangle(x, a, 2, 3);
        //ofDrawRectangle(x, b, 2, 3);
        
        
        ofFill();
        ofSetLineWidth(.5);
        
       // ofDrawRectangle(x,a,2,abs(a-b));
        
     //   ofDrawRectangle(x,b,2,2);
     //   ofDrawLine(x,a,x,b);
        

        
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(b < 0){
            return true;
        } else {
            return false;
        }
    }
    
};