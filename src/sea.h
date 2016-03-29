//
//  waveSystem.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"
#include "defines.h"

class Drop {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    float p;
    float v;
    float a;
    float attack;
    float damping;
    
    Drop(){
        p = v = a = 0;
        attack = ATTACK;
        damping = DAMPING;
    }
    
    
    void calcAccel(float p_left, float p_right);
    
    void update( float p_left, float p_right ){
        float delta_p = p_left + p_right - 2*p;
        a = attack * delta_p;
        v = (v + a) * damping;
        p = p + v;
    }
};


class Sea{
public:
    vector<Drop> drops;
    vector<float> dropsPos;
    
    void setup(){
        for(int i = 0; i<RES_W; i++){
            Drop wP;
            wP.p = 0.;
            drops.push_back(wP);
            dropsPos.push_back(wP.p);
        }
    }
    
    void updateResponse(){
        for(int i = 0; i<drops.size(); i++){
            drops[i].damping = DAMPING;
            drops[i].attack  = ATTACK;
        }
    }
    
    void update(float _inLeft, float _inRight, float _at){
        int at = _at*drops.size();
        drops[at].p = _inLeft;
 

//        for(int i = at-1; i>1; i--){
//            //equals left and right of particle
//            drops[i].update(drops[i-1].p, drops[i+1].p);
//            
//            int i2 = drops.size()-1-i;
//            drops[i2].update(drops[i2-1].p, drops[i2+1].p);
//        }
        
        for(int i = at+1; i<drops.size()+at-1; i++){
            
            int u = i%drops.size();//1-drops.size()
        
            
            if(u==0){
                drops[u].update(drops.back().p, drops[1].p);
            }
            if(u==drops.size()-1){
                drops[u].update(drops[u-1].p, drops[0].p);
            }
            else{
                drops[u].update(drops[u-1].p, drops[u+1].p); //i = 0. at=at-1,at+1.
            }
            
            int i2 = drops.size()-1-u;
            drops[i2].update(drops[i2-1].p, drops[i2+1].p);
        }
        
        //make float vector
        for(int i = 1; i<drops.size(); i++){
            dropsPos[i] = drops[i].p;
        }
    }
    
};
