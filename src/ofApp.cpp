/** 

 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	ofSetFrameRate(60);

	ofEnableLighting();
	light.setPointLight();


	//load wav files and play them
	kicks.load("kicks-spare.wav",true);
  	kicks.play(); 
	hihats.load("hihits-spare.wav",true);
  	hihats.play();
	synths.load("synths-spare.wav",true);
  	synths.play();





	//ofSetVerticalSync(true);
	ofBackground(50, 50, 50, 0);

	//we need to call this for textures to work on models
	ofDisableArbTex();

	//this makes sure that the back of the model doesn't show through the front
	ofEnableDepthTest();

	//now we load our model
	model.loadModel("quarry/Dead Sea Qumran.3ds");
	model.setPosition(ofGetWidth()*.5, ofGetHeight() * 0.75, 0);
	light.enable();
	light.setPosition(model.getPosition() + glm::vec3(0, 0, 1600));


	kickSphere.setRadius(50);
	hatSphere.setRadius(30);



	for(int x = 0; x < 80; x++){
    	for(int y = 0; y < 40; y++){
			for(int z = 0; z < 6; z++){

				Cube c;
				c.pos = glm::vec3(x * 60 - 600,y * 60 - 300,z);
				c.targetZ = ofRandom(-400, 400);
				c.baseScale = glm::vec3(40, 40, 40);
				c.scale = c.baseScale;
				cubes.push_back(c);
			}
}
}




	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	ofSoundUpdate();

	float volumekick = ofMap(mouseX, 0, ofGetWidth(), 0.0, 1.0, true); //map mouseX to kick volume
	kicks.setVolume(volumekick);	

	float volumehat = ofMap(mouseY, 0, ofGetWidth(), 0.0, 1.0, true); // map mouseY to hihat volume
	hihats.setVolume(volumehat);	

	kickEnergy = kicks.getVolume(); //set kick energy based on the volume of the kick track
	hihatEnergy  = hihats.getVolume();


	float t = ofGetElapsedTimef(); //get time

	float gravity = 0.0; //gravity variable to increase later

	//------------------SPHERES---------§-------------------------
	
	if(t > 64.0){
		gravity = 0.4;// increase gravity after 64 seconds
		for(auto & s : spheres){
		// apply gravity downward
		s.vel.y += gravity;
		// existing audio force
		s.vel += glm::vec3(kickEnergy * 0.5, 0, 0);
		// move
		s.pos += s.vel;
		// bounce X
		if(s.pos.x < 0 || s.pos.x > ofGetWidth()){
			s.vel.x *= -1;
		}
		// bounce Y
		if(s.pos.y < 0 || s.pos.y > ofGetHeight()){
			s.vel.y *= -0.8; // lose some energy
		}
		// bounce Z
		if(s.pos.z < -500 || s.pos.z > 500){
			s.vel.z *= -1;
		}
			} }

	else if(t > 32.0 && spheres.size() < 50){
		for(int i = 0; i < 10; i++){
			AudioSphere s;
			s.pos = glm::vec3(ofGetWidth()/2, ofGetHeight()/2, 0);
			s.vel = glm::vec3(ofRandom(-5,5), ofRandom(-5,5), ofRandom(-5,5));
			s.radius = ofRandom(20, 60);
			s.isKick = (ofRandom(1.0) > 0.5);
			spheres.push_back(s);
		}

	}




	for(auto & s : spheres){
		s.pos += s.vel;
		s.vel += glm::vec3(kickEnergy * 0.5, 0, 0);
		
	// bounce X
		if(s.pos.x < 0 || s.pos.x > ofGetWidth()) s.vel.x *= -1;
		// bounce Y
		if(s.pos.y < 0 || s.pos.y > ofGetHeight()) s.vel.y *= -1;
		// bounce Z (optional)
		if(s.pos.z < -500 || s.pos.z > 500) s.vel.z *= -1;

		ofColor c = s.color;
    	c *= (0.5 + kickEnergy); // audio brightness modulation
    	ofSetColor(c);
	}



//-----------------CUBOIID WALL---------§-------------------------

	for(auto & c : cubes){ //loops through every cube in the cubes vector
    	c.scale = glm::mix(c.scale, c.baseScale, 0.1); //return scale to normal after manipulation
		// AFTER 64 SECONDS
		if(t > 65.0){	 // synchronised to song
			
			c.pos.z = ofLerp(c.pos.z, c.targetZ, 0.03); // smoothly move toward target depth
		}

		if(hihatEnergy > 0.3 && ofRandom(1.0) < 0.1){//link hihat energy to random chance of manipulation
			int axis = ofRandom(3); //set variable
			if(axis == 0){
				c.scale.x = c.baseScale.x * ofRandom(1.0, 4.0); //manipulate x scale based on random value and base scale
			}
			else if(axis == 1){
				c.scale.y = c.baseScale.y * ofRandom(1.0, 4.0);//manipulate y scale based on random value and base scale
			}
			else{
				c.scale.z = c.baseScale.z * ofRandom(1.0, 4.0);//manipulate z scale based on random value and base scale
				}
			}
		}
	}






//--------------------------------------------------------------
void ofApp::draw(){

	ofEnableLighting();

	//-----------------SPOT LIGHTS---------§-------------------------
	light.setup();
	light.setDiffuseColor(ofFloatColor(2.0, 2.0, 2.0));
	light.setSpecularColor(ofFloatColor(2.0, 2.0, 2.0));	
	light.setSpotConcentration(40);   // tighter = brighter beam
	light.setSpotlightCutOff(25);   

	fill.setPosition(model.getPosition() + glm::vec3(-800, 400, 300));
	fill.setDiffuseColor(ofFloatColor(1.6, 1.6, 2.0));
	fill.setSpecularColor(ofFloatColor(1.6, 2.0, 1.6));
	fill.enable();


	ambient.setDiffuseColor(ofFloatColor(0.2, 0.15, 0.15));
	ambient.setSpecularColor(ofFloatColor(0.1, 0.1, 0.1));
	ambient.setSpotConcentration(4);   // tighter = brighter beam
	ambient.enable();

	float bpm = 60.0; //SET BPM
	float beat = ofGetElapsedTimef() * (bpm / 60.0);

	// create out of sync patterns for light and fill
	bool lightOn = fmod(beat + 0.0, 4.0) < 1.0;
	bool fillOn  = fmod(beat + 1.3, 3.0) < 1.0;

	// APPLY patter to lights
	if(lightOn) light.enable();
	else light.disable();
	if(fillOn) fill.enable();
	else fill.disable();

	float t1 = ofGetElapsedTimef();

	float flash = 0.0;

	ofColor bgColor;

//--------------TIMINGS-------------------------
	if(t1 > 24.0){ //red strobes after 24 seconds
		flash = sin(t1 * 20.0);
		bgColor = ofColor(255 * flash, 0, 0); // red strobe
		whiteflash.setup();
		whiteflash.setPointLight();
		whiteflash.setDiffuseColor(ofFloatColor(5.0 * flash,0.0,0.0));
    	whiteflash.setSpecularColor(ofFloatColor(10.0 * flash,0.0, 0.0 ));
		whiteflash.enable();


	}

	else if(t1 > 16.0){// white strobes after 16 seconds


		flash= sin(t1 * 10.0);
		bgColor = ofColor(255 * flash);
		redflash.setup();
		redflash.setPointLight();
		redflash.setDiffuseColor(ofFloatColor(5.0 * flash,5.0 * flash,5.0 * flash));
    	redflash.setSpecularColor(ofFloatColor(10.0 * flash,10.0 * flash, 10.0 * flash));
		redflash.enable();

	}

	else {

		bgColor = ofColor(20, 20, 30);//set original background colour

	}

	ofBackground(bgColor);


	ofSetColor(255, 255, 255, 255);
	drawWithModel();
	ofEnableDepthTest();
	ofEnableLighting();

		float shake = kickEnergy * 30.0;  // shake the model  based on the kick energy
		ofPushMatrix();
// -----------VOLUME CONTROLLED SPHERES---------§-------------------------
	ofTranslate(ofGetWidth()/2 + ofRandom(-shake, shake),ofGetHeight()/2 + ofRandom(-shake, shake), ofRandom(-shake, shake));

		ofSetColor(255 * kickEnergy); //link sphere to kick volume
		kickSphere.setRadius(50 + kickEnergy * 200);
		kickSphere.setPosition(-300, 200, 0);	
		kickSphere.draw();

		ofSetColor(255 * kickEnergy,0.0,0.0);
		kickSphere.setRadius(50 + kickEnergy * 200);
		kickSphere.setPosition(-300, -200, 0);	
		kickSphere.draw();

		ofSetColor(255 * hihatEnergy); //link sphere to hihat volume
		hatSphere.setRadius(30 + hihatEnergy * 200);
		hatSphere.setPosition(300, -200, 0);
		hatSphere.draw();
		
		
		ofSetColor(255 * hihatEnergy,0.0,0.0);
		hatSphere.setRadius(30 + hihatEnergy * 200);
		hatSphere.setPosition(300, 200, 0);
		hatSphere.draw();
		ofPopMatrix();


	

//----------------SPHERES---------§-------------------------
	for(auto & s : spheres){
		ofSetColor(255 * kickEnergy); //modulate color based on kick energy

		ofPushMatrix();

		ofTranslate(s.pos);

		if(s.isKick){
			kickSphere.setRadius(s.radius + kickEnergy * 100);//modulate radius to kick volume
			kickSphere.draw();
		} 

		ofPopMatrix();

	}
	ofPopMatrix();

//----------------CUBOIID WALL---------§-------------------------
	

	for(auto & c : cubes){
		ofSetColor(255);
		ofPushMatrix();
		ofTranslate(c.pos*4);
		ofScale(c.scale.x, c.scale.y, c.scale.z);
		ofSetColor(255);
		ofDrawBox(0, 0, 0, 1); // unit cube scaled
		ofPopMatrix();

}

	ofDisableLighting();

	ofDisableDepthTest();



	ofDisableLighting();
	ofDisableDepthTest();




}

//draw the model the built-in way
void ofApp::drawWithModel(){

	
	glm::vec3 position = model.getPosition(); //get the position of the model

	
	ofPushMatrix(); //save the current view
	ofScale(1.0,1.0,2.0);
	float bpm = 120.0; // set BPM for model movement
	float beatsPerSecond = bpm / 60.0; 
	float moveAmount = 8.0; // subtle movement amount
	float yOffset = sin(ofGetElapsedTimef() * TWO_PI * beatsPerSecond) * moveAmount; // sine wave movement synced to BPM

	ofTranslate(position.x, position.y + yOffset, position.z); //manipulate the position of the model to add subtle movement based on the beat
	ofRotateDeg(0, 0, 1, 0);
	ofRotateDeg(90,1,0,0);
	ofTranslate(-position);

	
	model.setScale(4.0,4.0,4.0); //draw the models
	model.drawFaces();
	ofScale(15.0, 10.0, 10.0);
	//restore the view position
	ofPopMatrix();


}

void ofApp::drawWithMesh(){

}

//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::keyPressed(int key){


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

