#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    setupGui();
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    syncOSC.update();
    
    
    waveSystem.setup(RES_W);
    waveSystem.updateResponse(ATTACK, DAMPING);
    seaSystem.setup();
    
   //Danicinglines
    for(int i = 0; i < 10; i++){
        DancingLine newLine;
        newLine.location1 = ofVec2f(0, RES_H/2);
        newLine.location2 = ofVec2f(RES_W, RES_H/2);
        newLine.lineColor = colorDLines;
        newLine.freedom = RES_H/2;
        
        dancingLines.push_back(newLine);
    }

    //shaders
    cloudShader.load("shaders/cloud");
    flamesShader.load("shaders/flames");
    glowShader.load("shaders/glowLines");
    dropShader.load("shaders/drops");
    perlinShader.load("shaders/perlinWorms");
    
    //main render setup
    render.allocate(RES_W,RES_H);
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render.end();
    
    //texture render
    meshFbo.allocate(RES_W,RES_H);
    meshFbo.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    meshFbo.end();
    
    syphon.setName("LED_TREE");
 
}

//--------------------------------------------------------------
void ofApp::update(){
    
    syncOSC.update();
    
    //syncs intensity ---
//    u_amount = intensity;
//    flameSize = 1-intensity;
//    blinkIntensity = intensity;
//    boubblesIntensity = intensity;
//    waveSizeDrops = intensity*3;
    drawnEdge = drawnEdge*0.9 + edge*0.1;
    //syncs colors of tex and grafics
    if(syncColors){
        colorBlink = grafiks;
        colorBoubbles = grafiks;
        colorDLines = grafiks;
        colorLarve = grafiks;
        colorSpiral = grafiks;
        
        colorDrops = textures;
        cloudColor = textures;
        flameColor = textures;
        glowColor = textures;
    }
    
    //rainDrops:
    for(int i = 0; i<masksFader.size();i++){
        if(masksFader[i]==1){
            masksFader[i]=0;
            DataRain rain;
            rain.num = i;
            rain.edge = 1-edge;
            rain.pos = ofVec2f(i*(RES_W/numTreePoles),0);
            rainDrops.push_back(rain);
            
        }
    }
    for (vector<DataRain>::iterator it=rainDrops.begin(); it!=rainDrops.end();)    {
        it->update(1-edge);
        if(it->isDead()){
            it = rainDrops.erase(it);
            edge+=(float(1-edge)/800);
        }
        else{
            ++it;
        }
    }
    
    //shaders tempo
    counterFlames +=flamesTempo;
    counterGlow += tempoGlow;
    counterCloud += tempoCloud/10;
    counterDrops += tempoDrops/10;
    counterPerlin+=tempoPerlin;
    
    //update unwarped image (blink, bubbles, larves, spiral)
    setGraficVals();
    
    //update warped image (textures and dancing lines)
    setShaderVals();
    
    // wave update
    float noiseLeft = noiseAmt*(ofNoise(ofGetElapsedTimef()*4)-0.5);
    float noiseRight = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+300.)-0.5);
    float noiseRight_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+600.)-0.5);
    float noiseLeft_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+900.)-0.5);
    
    //wave system
    waveSystem.update(inLeft+noiseLeft, inRight+noiseRight);
    
    //interactive sea update
    int max = 0 ;
    float number = 0 ;
    for(int i = 0 ; i<masksFader.size();i++){
        if (masksFader[i]>max) {
            max = masksFader[i];
            number = float(i)/masksFader.size();
        }
    }
    if(pWaveEdge == disturbWave){ //thres to stop waves if no-one is there
        waveCounter++;
    }
    if(pWaveEdge != disturbWave){
        pWaveEdge = disturbWave;
        waveCounter = 0;
    }
    if(waveCounter<1000){
        seaSystem.update(inLeft+noiseLeft, inRight+noiseRight,disturbWave);
    }
    else{
        seaSystem.update(0,0,disturbWave);
    }
    
    //mainRender
    render.begin();
    
   
        
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ofBackground(0);
    if(!transistionBegun){
        //draw textures in masks
        if(interActiveMasks){
            if(cloud||flames||glow||drops||perlin||b_lines){
                vector<float>vec;
                vec.resize(masksFader.size(), 0);
                for(int i = 0 ; i<rainDrops.size();i++){
                    if(rainDrops[i].updatePoint){
                        vec[rainDrops[i].num] = rainDrops[i].point;
                    }
                }
                drawGradient(0, 0, RES_W, RES_H, drawnEdge, meshFbo.getTexture(), vec);
            }
        }
        if(interActiveWave){
            if(cloud||flames||glow||drops||perlin||b_lines){
                drawGradient(0, 0, RES_W, RES_H, drawnEdge, meshFbo.getTexture(),waveSystem.waveParticlesPos);
            }
        }
        if(interActiveSea){
            if(cloud||flames||glow||drops||perlin||b_lines){
                drawGradient(0, 0, RES_W, RES_H, drawnEdge, meshFbo.getTexture(),seaSystem.dropsPos);
            }
        }
        //draw textures unwarped (stretched)
        if(!interActiveMasks&&!interActiveWave&&!interActiveSea){
            if(cloud||flames||glow||drops||perlin){
                vector<float>vec;
                for(int i = 0 ; i<masksFader.size();i++){
                    vec.push_back(0);
                }
                drawGradient(0, 0, RES_W, RES_H, drawnEdge, meshFbo.getTexture(), vec);
            }
        }
    }
    
    //draw grafik
    if(b_bubbles){
        for(auto boubble: boubbles){
            boubble.draw();
        }
    }
    if(b_larve){
        for(auto Larve:larves){
            Larve.draw();
        }
    }
    for(auto Spiral:spirals){
        Spiral.draw();
    }
    if(b_blink){
        for(auto Blink:blinks){
            Blink.draw();
        }
    }
    for(auto Rain:rainDrops){ //unused code
        Rain.draw();
    }
    
//    if(rainDrops.size()>2){
//    for(int i = 0; i<rainDrops.size()-1;i++){
//        ofDrawLine(rainDrops[i].pos,rainDrops[i+1].pos);
//    }}
    if(transistionBegun && transPix.size()>0){
        transPix[0].draw();
    }
    
    render.end();
    
    if(transition && !transistionBegun){
        transistionBegun = true;
        transition = false;
        ofPixels pix;
        vector<ofVec2f>pos;
        vector<ofColor>col;
        render.getTexture().readToPixels(pix);
        for(int x = 0; x<RES_W; x+=RES_W/numTreePoles){
            for(int y = RES_H-1; y > (1-edge)*RES_H; y--){
                
                col.push_back(pix.getColor(x+2, y));
                pos.push_back(ofVec2f(x, y));
            }
        }
        TransPix tp(pos,col);
        tp.edge = (1-edge)*RES_H;
        tp.orgEdge = (1-edge)*RES_H;
        transPix.push_back(tp);
    }
    for (vector<TransPix>::iterator it=transPix.begin(); it!=transPix.end();)    {
        it->update();
        if(it->pix.size()<=0){
            it = transPix.erase(it);
            transistionBegun = false;
            edge = 0.01;
            drawnEdge = 0;
        }
        else{
            ++it;
        }
    }
    
    ofFill();
    syphon.publishTexture(&render.getTexture());
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(pixelPreview){
        ofBackground(0);
        ofPixels pix;
        render.getTexture().readToPixels(pix);
        
        ofPushMatrix();
        ofTranslate(gui.getWidth()+50-RES_W/2, 20);
        ofSetLineWidth(0);
        
        float r,g,b,a;
      
        // pixel preview, draw offset at edge to chech how it stitches
        int inc = RES_W/10;
        for(int x = RES_W/2 ; x< RES_W ; x+=inc){
            
            for(int y = 0; y<RES_H; y+=1){
                ofSetColor(pix.getColor(x+5, y));
                ofDrawRectangle(x, y, (inc)/2, 1);
            }
        }
        ofTranslate(RES_W, 0);
        for(int x = 0 ; x< RES_W/2 ; x+=inc){
            
            for(int y = 0; y<RES_H; y+=1){
                ofSetColor(pix.getColor(x+5, y));
                ofDrawRectangle(x, y, (inc)/2, 1);
            }
        }
        ofTranslate(RES_W, 300);
       // ofDrawRectangle(0, 700, 500,500);
        for(int x = 0 ; x< RES_W ; x+=10){
            
            for(int y = 700; y<RES_H; y+=1){
            
                ofSetColor(pix.getColor(x+5, y));
                ofDrawRectangle(x, (y-700)*5, 5, 5);
            }
        }
        ofSetColor(255);
        ofNoFill();
        //ofDrawRectangle(0, 0, RES_W, RES_H);
        ofDrawBitmapString("pixel preview " + ofToString(boubbles.size()), 0, -10);
        if(waveCounter>1000){
            ofSetColor(255, 0, 0);
            ofDrawRectangle(0, 0, 20, 20);
        }
        ofPopMatrix();
    }

    gui.draw();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode){
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

void ofApp:: drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture, vector<float>vec){
    ofPushMatrix();
    ofTranslate(_x,_y);
    float inc = float(vec.size())/_w;
    
    float incTex = _texture.getWidth() / _w;
    float texHeight = _texture.getHeight();
    float texHalfHeight = texHeight/2;
    
    ofMesh mesh;
    mesh.clear();
    mesh.enableTextures();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    float min = 0.;
    float max = 1.;

    for(int i = 0; i < _w; i++){
        float x = i;
        int u =int(float(i)*inc);
        
        float y;
       
        y = (2*(vec[u])/2+1-_posHWave)*_h;
        
        
        ofFloatColor pColor = ofFloatColor(1., ofMap( vec[u]*-1, min, max, 0.9, 1, true));
        
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

//------
void ofApp::setShaderVals(){
    meshFbo.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  
    ofBackground(0);
    if(gradientColor){
        ofRectGradient(0, 0, RES_W, RES_H/2, Fx1ColorTopTop, Fx1ColorTopBot, OF_GRADIENT_LINEAR);
        ofRectGradient(0, RES_H/2, RES_W, RES_H/2, Fx1ColorBotTop, Fx1ColorBotBot, OF_GRADIENT_LINEAR);
    }
    
    if(glow){
        glowShader.begin();
        glowShader.setUniform2f("iResolution", RES_W, RES_H*3);
        glowShader.setUniform1f("iGlobalTime", counterGlow);
        glowShader.setUniform1f("u_density", glowDensity);
        glowShader.setUniform1f("u_amount", u_amount);
        glowShader.setUniform3f("u_color", float(glowColor->r)/255,float(glowColor->g)/255,float(glowColor->b)/255);
        glowShader.setUniform1f("horisontal", glowHorisontal);
        ofSetColor(255);
        ofFill();
        ofDrawRectangle(0, RES_H/2, RES_W, RES_H/2);
        glowShader.end();
    }
    
    if(flames){
        flamesShader.begin();
        flamesShader.setUniform2f("iResolution", RES_W, RES_H/2);
        flamesShader.setUniform1f("speed", counterFlames);
        flamesShader.setUniform1f("size", flameSize);
        flamesShader.setUniform1f("y_size", flameAmountY);
        flamesShader.setUniform3f("u_color", float(flameColor->r)/255,float(flameColor->g)/255,float(flameColor->b)/255);
        
        ofSetColor(255);
        ofFill();
        ofDrawRectangle(0, RES_H/2, RES_W, RES_H/2);
        flamesShader.end();
    }
    
    if(drops){
        dropShader.begin();
        dropShader.setUniform2f("u_resolution",RES_W, RES_H/2);
        dropShader.setUniform1f("u_time", counterDrops);
        dropShader.setUniform1f("u_colorsThres", colorThresDrops);
        dropShader.setUniform1f("u_offSet", offSetDrops);
        dropShader.setUniform1f("u_contrast", contrastDrops);
        dropShader.setUniform1f("u_waveSize", waveSizeDrops);
        dropShader.setUniform1i("u_inverse", inverseDrops);
        dropShader.setUniform3f("u_color", float(colorDrops->r)/255,float(colorDrops->g)/255,float(colorDrops->b)/255);
        ofSetColor(255,255,255);
        ofFill();
        ofDrawRectangle(0, RES_H/2, RES_W, RES_H/2);
        dropShader.end();
    }
    
    if(cloud){
        cloudShader.begin();
        cloudShader.setUniform2f("u_resolution",RES_W, RES_H/2);
        cloudShader.setUniform1f("u_time", counterCloud);
        cloudShader.setUniform1f("u_zoom", zoomCloud);
        cloudShader.setUniform1f("u_balance", balance);
        cloudShader.setUniform1f("u_contrast", contrast+1.0);
        cloudShader.setUniform1i("bwSwitch", false);
        cloudShader.setUniform1i("bgTransparent", true);
        cloudShader.setUniform1i("enableFBM", enableFBM);
        cloudShader.setUniform1i("enableRMF", enableRMF);
        cloudShader.setUniform3f("u_color", float(cloudColor->r)/255,float(cloudColor->g)/255,float(cloudColor->b)/255);
        ofSetColor(255,255,255);
        ofFill();
        ofDrawRectangle(0, RES_H/2, RES_W, RES_H/2);
        cloudShader.end();
    }
    
    if(b_lines){
        for(auto DancingLine:dancingLines){
            DancingLine.draw();
        }
    }
    
    if(perlin){ // perlin is only black, makes masks on the other textures
        perlinShader.begin();
        perlinShader.setUniform2f("u_resolution",RES_W, RES_H/2);
        perlinShader.setUniform1f("u_time", counterPerlin);
        perlinShader.setUniform1f("u_zoom", zoomPerlin);
        perlinShader.setUniform1f("u_date", 1.);
        perlinShader.setUniform1f("u_linesY", horizontalPerlin);
        perlinShader.setUniform1f("u_linesX", false);
        ofSetColor(255,255,255);
        ofFill();
        ofDrawRectangle(0, RES_H/2, RES_W, RES_H/2);
        perlinShader.end();
    }
//    if(!gradientColor){
//        ofRectGradient(0, RES_H/2-40, RES_W, 100,ofColor(0), ofColor(0,0), OF_GRADIENT_LINEAR);
//    }

    meshFbo.end();
}

void ofApp::setGraficVals(){
    
    // Blinkedne blinks
    for(int i = 0; i < 10 ; i++){
        if(ofRandom(100000)/100000 < blinkIntensity){
            Blink blink;
            blink.blinkColor = colorBlink;
            int resH = ofRandom(RES_H*(1-edge));
            blink.location = ofVec2f(((int)ofRandom(RES_W)),(int)ofRandom(resH));
            blink.tempo = blinkTempo;
            blink.hard_soft = hard_soft;
            
            blinks.push_back(blink);
        }
    }
    
    for (vector<Blink>::iterator it=blinks.begin(); it!=blinks.end();)    {
        it->update();
        if(it->isDead())
            it = blinks.erase(it);
        else
            ++it;
    }
    
    // Larves
    if(ofRandom(100000)/100000 < larveIntensity){
        Larve larve;
        larve.larveColor = colorLarve;
        larve.length = ofRandom(larveMinLength,larveMaxLength);
        larve.a = RES_H;
        larve.b = larve.a-larve.length;
        larve.velocity = ofVec2f(0.,  -ofRandom(larveVelMin   , larveVelMax) );
        larves.push_back(larve);
    }
    
    for (vector<Larve>::iterator it=larves.begin(); it!=larves.end();)    {
        it->update();
        if(it->isDead())
            it = larves.erase(it);
        else
            ++it;
    }
    
    
    // Boubbles
    if(ofRandom(100000)/100000 < boubblesIntensity){
        Boubble boubble;
        boubble.boubbleColor = colorBoubbles;
        boubble.trace = bub_trace;
        if(up_down){
            boubble.location = ofVec2f(((int)ofRandom(RES_W)),RES_H);
            boubble.velocity = ofVec2f(0., - ofRandom(boubblesVelMin   , boubblesVelMax));
        }
        if(!up_down){
            boubble.location = ofVec2f(((int)ofRandom(RES_W)),0);
            boubble.velocity = ofVec2f(0., ofRandom(boubblesVelMin   , boubblesVelMax));
        }
        boubble.up = up_down;
        boubbles.push_back(boubble);
    }
    for (vector<Boubble>::iterator it=boubbles.begin(); it!=boubbles.end();)    {
        it->update();
        
        if(it->isDead())
            it = boubbles.erase(it);
        else
            ++it;
    }
    
    // spirals
    if(b_spiral){
        b_spiral = false;
        Spiral spiral;
        spiral.spiralColor = colorSpiral;
        spiral.vel = ofRandom(3,7);
        spiral.amount = spiralAmount;
        spiral.height = spiralAngle;
        spirals.push_back(spiral);
    }
    
    for (vector<Spiral>::iterator it=spirals.begin(); it!=spirals.end();){
        it->update();
        
        if(it->isDead())
            it = spirals.erase(it);
        else
            ++it;
    }
    
    // DANCING LINES
    if(b_lines){
        counterDLines += dLinesSpeed/100;
        
        for (vector<DancingLine>::iterator it=dancingLines.begin(); it!=dancingLines.end();)    {
            it->lineColor = colorDLines;
            it->update(counterDLines);
            it->sync = dLinesSync;
            ++it;
        }
    }
    
//    if (!b_larve) {
//        larveIntensity = 0;
//    }
//    if (b_larve && larveIntensity==0) {
//        larveIntensity = 0.2;
//    }
//    if (!b_blink) {
//        blinkIntensity = 0.0;
//    }
//    if (b_blink && blinkIntensity== 0) {
//        blinkIntensity = 0.2;
//    }
//    if (!b_bubbles) {
//        boubblesIntensity = 0;
//    }
//    if (b_bubbles && boubblesIntensity==0) {
//        boubblesIntensity = 0.4;
//    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
}

//--------------------------------------------------------------

void ofApp::setupGui(){
    guiGroup.setName("guiGroup");
    
    vector<string>n;
    n.push_back("maskI");
    n.push_back("maskII");
    n.push_back("maskIII");
    n.push_back("maskIV");
    n.push_back("maskV");
    n.push_back("maskVI");
    n.push_back("maskVII");
    n.push_back("maskVIII");
    n.push_back("maskIX");
    n.push_back("maskX");
    
    enable.setName("enable");
    masksFader.resize(numTreePoles);
    for(int i =0; i<numTreePoles ; i++){
        masksFader[i].set(n[i],0,0,1);
        enable.add(masksFader[i]);
    }

    
    backgroud_Grad.setName("backgroud_Grad");
    backgroud_Grad.add(Fx1ColorTopTop.set("Backgroud_TopTop", ofColor(0,0), ofColor(0,0),ofColor(255)));
    backgroud_Grad.add(Fx1ColorTopBot.set("Backgroud_TopBot", ofColor(0,0), ofColor(0,0),ofColor(255)));
    backgroud_Grad.add(Fx1ColorBotTop.set("Backgroud_BotTop", ofColor(0,0), ofColor(0,0),ofColor(255)));
    backgroud_Grad.add(Fx1ColorBotBot.set("Backgroud_BotBot", ofColor(0,0), ofColor(0,0),ofColor(255)));
    
    mainControl.setName("mainControl");
    mainControl.add(pixelPreview.set("pixPreview", false));
    mainControl.add(gradientColor.set("gradientColor", false));
    mainControl.add(glow.set("glow", false));
    mainControl.add(flames.set("flames", false));
    mainControl.add(drops.set("drops", false));
    mainControl.add(cloud.set("cloud", false));
    mainControl.add(b_lines.set("dancingLines", false));
    mainControl.add(perlin.set("perlin", false));
    
    mainControl.add(b_larve.set("larves", false));
    mainControl.add(b_blink.set("b_blink", false));
    mainControl.add(b_bubbles.set("bubbles", false));
    mainControl.add(b_spiral.set("Spiral", false));
    
    mainControl.add(interActiveMasks.set("interActiveMasks", false));
    mainControl.add(interActiveWave.set("interActiveWave", false));
    mainControl.add(interActiveSea.set("interActiveSea", false));
    
    mainControl.add(transition.set("transition", false));
    mainControl.add(intensity.set("intensity", 0.5, 0., 1));
    mainControl.add(edge.set("edge", 0.5, 0., 1));
    mainControl.add(disturbWave.set("disturbWave", 0.5, 0., 1));
    mainControl.add(syncColors.set("syncColors", false));
    mainControl.add(grafiks.set("grafiks", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    mainControl.add(textures.set("textures", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    // Bubbles and Lines
    waveControl.setName("BubblesLines");
    ofParameterGroup waveControlWave;
    waveControlWave.setName("waveControlWave");
    waveControlWave.add(inLeft.set("inLeft", 0., -1., 1.));
    waveControlWave.add(inRight.set("inRight", 0., -1., 1.));
    waveControlWave.add(noiseAmt.set("noiseAmt", 0.15, 0., 1));

    ofParameterGroup waveControlSpiral;
    waveControlSpiral.setName("waveControlSpiral");
    waveControlSpiral.add(spiralIntensity.set("spiralIntensity", false));
    waveControlSpiral.add(spiralAngle.set("spiralAngle", 0.0, 2.0, 30.0));
    waveControlSpiral.add(spiralAmount.set("spiralAmount", 2, 1,7));
    waveControlSpiral.add(colorSpiral.set("colorSpiral", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    ofParameterGroup waveControlBub;
    waveControlBub.setName("waveControlBub");
    waveControlBub.add(boubblesIntensity.set("boubblesIntensity", 0.0, 0.0, 1.0));
    waveControlBub.add(boubblesVelMin.set("boubblesVelMin", 0.5, 0.0, 5.0));
    waveControlBub.add(boubblesVelMax.set("boubblesVelMax", 2., 0.0, 5.0));
    waveControlBub.add(bub_trace.set("bub_trace", 1,1,10));
    waveControlBub.add(up_down.set("up_down", true));
    waveControlBub.add(colorBoubbles.set("colorBoubbles", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    ofParameterGroup waveControlLarve;
    waveControlLarve.setName("waveControlLarve");
    waveControlLarve.add(larveIntensity.set("larveIntensity", 1.0, 0.0, 1.0));
    waveControlLarve.add(larveVelMin.set("larveVelMin", 0.5, 0.1, 5.0));
    waveControlLarve.add(larveVelMax.set("larveVelMax", 2., 0.1, 5.0));
    waveControlLarve.add(larveMinLength.set("larveMinLength", 2., 7.0, 50));
    waveControlLarve.add(larveMaxLength.set("larveMaxLength", 2., 10.0, 50));
    waveControlLarve.add(colorLarve.set("colorLarve", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    ofParameterGroup waveControlLines;
    waveControlLines.setName("waveControlLines");
    waveControlLines.add(dLinesSpeed.set("dLinesSpeed", .2, 0.0, 1));
    waveControlLines.add(dLinesSync.set("dLinesSync", .9, 0.5, 1));
    waveControlLines.add(colorDLines.set("colorDLines", ofColor(0,255), ofColor(0,0), ofColor(255,255)));
    
    ofParameterGroup waveControlBlink;
    waveControlBlink.setName("waveControlBlink");
    waveControlBlink.add(blinkIntensity.set("blinkIntensity", 0.0, 0.0, 1.0));
    waveControlBlink.add(blinkTempo.set("blinkTempo", 0.02, 0.0001, 0.1));
    waveControlBlink.add(hard_soft.set("hard/soft", true));
    waveControlBlink.add(colorBlink.set("colorBlink", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    waveControl.add(waveControlWave);
    waveControl.add(waveControlBlink);
    waveControl.add(waveControlSpiral);
    waveControl.add(waveControlBub);
    waveControl.add(waveControlLarve);
    waveControl.add(waveControlLines);

    //textures
    paramGeneral.setName("paramGeneral");
    paramFlames.setName("paramFlames");
    paramFlames.add(flamesTempo.set("flamesTempo", 1, 0., 1));
    paramFlames.add(flameAmountY.set("flame_height", 1, 0., 1));
    paramFlames.add(flameColor.set("flameColor", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));

    paramCloud.setName("paramCloud");
    paramCloud.add(tempoCloud.set("tempoCloud", 0.1, 0., 1));
    paramCloud.add(zoomCloud.set("zoomCloud", 0.1, 0., 1));
    paramCloud.add(balance.set("balance",0.5,0.,1.));
    paramCloud.add(contrast.set("contrast",0.,-1,1.));
    paramCloud.add(enableFBM.set("enableFBM", true));
    paramCloud.add(enableRMF.set("enableRMF", true));
    paramCloud.add(cloudColor.set("cloudColor", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    paramGlow.setName("paramGlow");
    paramGlow.add(tempoGlow.set("tempoGlow", 0.1, 0., 1));
    paramGlow.add(glowDensity.set("glowDensity",0.5,0.,1.));
    paramGlow.add(u_amount.set("u_amount",0.5,0.,1.));
    paramGlow.add(glowHorisontal.set("glowHorisontal",false));
    paramGlow.add(glowColor.set("glowColor", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));
    
    paramDrops.setName("paramDrops");
    paramDrops.add(tempoDrops.set("tempoDrops", 0.1, 0., 1));
    paramDrops.add(offSetDrops.set("offSetDrops", 0.1, 0., 1));
    paramDrops.add(colorThresDrops.set("colorThresDrops", 0.1, 0., 3.));
    paramDrops.add(contrastDrops.set("contrastDrops", 0.1, 0., 2));
    paramDrops.add(waveSizeDrops.set("waveSizeDrops", 0.1, 0., 3));
    paramDrops.add(colorDrops.set("colorDrops", ofColor(0,154,255,255), ofColor(0,0),ofColor(255)));
    paramDrops.add(inverseDrops.set("inverseDrops", false));
    
    paramPerlin.setName("paramPerlin");
    paramPerlin.add(tempoPerlin.set("tempoPerlin", 0.1, 0., 1));
    paramPerlin.add(zoomPerlin.set("zoomPerlin", 0.1, 0., 1));
    paramPerlin.add(horizontalPerlin.set("horizontalPerlin", false));
    
    paramGeneral.add(paramFlames);
    paramGeneral.add(paramCloud);
    paramGeneral.add(paramGlow);
    paramGeneral.add(paramDrops);
    paramGeneral.add(paramPerlin);
    
    guiGroup.add(enable);
    guiGroup.add(mainControl);
    guiGroup.add(waveControl);
    guiGroup.add(backgroud_Grad);
    guiGroup.add(paramGeneral);
    
    gui.setup(guiGroup);
    
    gui.loadFromFile("settings.xml");
    gui.minimizeAll();

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    for(int i = 0; i<10 ;i ++){
        char c = ofToChar(ofToString(i));
        if(key == c ){
            masksFader[i] = 1;
        }
    }
}
