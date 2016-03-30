//
//  waveSystem.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"


class WaveParticle {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    float p;
    float v;
    float a;
    float attack;
    float damping;
    
    WaveParticle(){
        p = v = a = 0;
        attack = 0;
        damping = 0;
    }
    
    void setResponse(float _attack, float _damping){
        attack = _attack;
        damping = _damping;
    }
    
    void calcAccel(float p_left, float p_right);
    
    void update( float p_left, float p_right ){
        float delta_p = p_left + p_right - 2*p;
        a = attack * delta_p;
        v = (v + a) * damping;
        p = p + v;
    }
};


class WaveParticleSystem{
public:
    vector<WaveParticle> waveParticles;
    vector<float> waveParticlesPos;
    
    void setup(int _numArray){
        for(int i = 0; i<_numArray; i++){
            WaveParticle wP;
            wP.p = 0.;
            waveParticles.push_back(wP);
            waveParticlesPos.push_back(wP.p);
        }
    }
    
    void updateResponse(float _attack, float _damping){
        for(int i = 0; i<waveParticles.size(); i++){
            waveParticles[i].damping = _damping;
            waveParticles[i].attack  = _attack;
        }
    }
    
    void update(float _inLeft, float _inRight){
        waveParticles[0].p = _inLeft;
        waveParticles.back().p = _inRight;
        
        for(int i = 1; i<waveParticles.size()-1; i++){
            waveParticles[i].update(waveParticles[i-1].p, waveParticles[i+1].p);
            int i2 = waveParticles.size()-1-i;
            waveParticles[i2].update(waveParticles[i2-1].p, waveParticles[i2+1].p);
        }
        for(int i = 1; i<waveParticles.size(); i++){
            waveParticlesPos[i] = waveParticles[i].p;
        }
    }
    
    void drawLine(int _x, int _y,int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth){
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofSetLineWidth(_lineWidth);
        ofSetColor(colorLine);
        float inc = waveParticles.size()/_w;
        ofPolyline line;
        for(int i = 0; i <= _w; i++){
            float x = i;
            float y = (waveParticles[i*inc].p/2+1-_posHLine)*_h;
            line.addVertex(ofVec2f(x,y));
        }
        line.draw();
        ofPopMatrix();
    }
     
    
};
