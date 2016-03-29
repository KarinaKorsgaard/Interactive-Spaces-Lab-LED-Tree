//
//  wave.h
//  LED_TREE
//
//  Created by Karina Jensen on 23/03/16.
//
//

#include "ofMain.h"
#include "defines.h"

class Wave{
public:
    float posH;
    ofColor col1;
    ofColor col2;
    ofColor col3;
    ofColor col4;
    ofFbo fbo;
    vector<float> line;
    ofPolyline poly;
    int counter;
    int divisions;
    float range;
    
    Wave() {
        fbo.allocate(RES_W,RES_H);
        fbo.begin();
        ofClear(0);
        fbo.end();
    }

    
    // Method to update location
    void update() {
        counter++;
        line.clear();
        poly.clear();
        for (int i = 0; i<RES_W; i++) {
            float h = RES_H*posH+(sin((float(i+counter)/RES_W)*divisions*PI)*range);
            line.push_back(h);
            poly.addVertex(i,h);
        }
        fbo.begin();
        ofClear(0);
        ofRectGradient(0, 0, RES_W, RES_H/2, col1, col2, OF_GRADIENT_LINEAR);
        ofRectGradient(0, RES_H/2, RES_W, RES_H/2, col3, col4, OF_GRADIENT_LINEAR);
        fbo.end();
    }
    // Method to display
    void draw() {
        drawGradient(0, 0,RES_W,RES_H, posH , fbo.getTexture());
        ofSetLineWidth(1);
        ofSetColor(255);
        poly.draw();
        
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(posH < 0){
            return true;
        } else {
            return false;
        }
    }
    
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture){
        
        ofPushMatrix();


        float inc = float(line.size())/_w;
        float incTex = _texture.getWidth() / _w;
        float texHeight = _texture.getHeight();
        float texHalfHeight = texHeight/2;
        
        ofMesh mesh;
        mesh.clear();
        mesh.enableTextures();
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        float min = 0.;
        float max = 1.;
        
        //    for(int i = 0; i < _w; i++){
        //        int u =int(float(i)*inc);
        //        float pValue = masksFader[u]*-1;
        //        if(pValue < min) min = pValue;
        //        if(pValue > max) max = pValue;
        //
        //    }
        //
        for(int i = 0; i < _w; i++){
            float x = i;
            int u =int(float(i)*inc);
            float y = line[u];
            ofFloatColor pColor = ofFloatColor(1., ofMap( line[u]*-1, min, max, 0.9, 1, true));
            
            mesh.addVertex(ofVec3f(x, 0, 0));
            mesh.addTexCoord(ofVec2f(x,0));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, y, 0));
            mesh.addTexCoord(ofVec2f(x,texHalfHeight));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, _h, 0));
            mesh.addTexCoord(ofVec2f(x,texHeight));
            mesh.addColor(pColor);
        }
        
        for(int i = 0; i < mesh.getNumVertices()/3-1; i++){
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+3);
            mesh.addIndex(i*3+0);
            
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+3);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+1);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+5);
            mesh.addIndex(i*3+4);
        }
        ofSetColor( 255, 255, 255 );  //Set white color
        _texture.bind();
        mesh.draw();
        _texture.unbind();
        
        ofPopMatrix();
    }
    
    void ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode){
        ofVboMesh gradientMesh;
        gradientMesh.clear();
        gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifndef TARGET_EMSCRIPTEN
#ifdef TARGET_OPENGLES
        if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
#else
        gradientMesh.setUsage(GL_STREAM_DRAW);
#endif
#endif
        if(mode == OF_GRADIENT_CIRCULAR) {
            // this could be optimized by building a single mesh once, then copying
            // it and just adding the colors whenever the function is called.
            ofVec2f center(w / 2 + px, h / 2 + py);
            gradientMesh.addVertex(center);
            gradientMesh.addColor(start);
            int n = 32; // circular gradient resolution
            float angleBisector = TWO_PI / (n * 2);
            float smallRadius = ofDist(0, 0, w / 2, h / 2);
            float bigRadius = smallRadius / cos(angleBisector);
            for(int i = 0; i <= n; i++) {
                float theta = i * TWO_PI / n;
                gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
                gradientMesh.addColor(end);
            }
        } else if(mode == OF_GRADIENT_LINEAR) {
            gradientMesh.addVertex(ofVec2f(px, py));
            gradientMesh.addVertex(ofVec2f(px+w, py));
            gradientMesh.addVertex(ofVec2f(px+w, py+h));
            gradientMesh.addVertex(ofVec2f(px, py+h));
            gradientMesh.addColor(start);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
        } else if(mode == OF_GRADIENT_BAR) {
            gradientMesh.addVertex(ofVec2f(px+w / 2, py+h / 2));
            gradientMesh.addVertex(ofVec2f(px, py+h / 2));
            gradientMesh.addVertex(ofVec2f(px, py));
            gradientMesh.addVertex(ofVec2f(px+w, py));
            gradientMesh.addVertex(ofVec2f(px+w, py+h / 2));
            gradientMesh.addVertex(ofVec2f(px+w, py+h));
            gradientMesh.addVertex(ofVec2f(px, py+h));
            gradientMesh.addVertex(ofVec2f(px, py+h / 2));
            gradientMesh.addColor(start);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
            gradientMesh.addColor(start);
        }
        GLboolean depthMaskEnabled;
        glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
        glDepthMask(GL_FALSE);
        gradientMesh.draw();
        if(depthMaskEnabled){
            glDepthMask(GL_TRUE);
        }
    }

    
};