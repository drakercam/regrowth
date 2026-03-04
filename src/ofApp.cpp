#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("Regrowth");
    ofEnableDepthTest();
    // ensures FBO textures are sampled in normalized [0,1] UVs
	ofDisableArbTex(); // disables ARB rectangular textures
	ofEnableNormalizedTexCoords(); // normalized 0..1 UVs

    // --- REGROWTH GAME SETUP ---
    manager.LoadImages();
    manager.LoadShaders();
    
    // ALREADY IMPLEMENTED
    manager.SetupTerrain();
    manager.SetupParticles();
    manager.SetupProceduralPlants();
    manager.SetupScreenSpaceEffects();
    manager.SetupPlayer();
    manager.SetupSkybox();
    manager.SetupPlantParticles();
    manager.SetupWitherers();
    manager.SetupPlantableAreas();
    manager.SetupSeeds();
    manager.SetupPlantDoor();
    manager.SetupFertilizer();
    manager.SetupPlantWall();
}

//--------------------------------------------------------------
void ofApp::update() 
{
    manager.Update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    manager.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{	
	manager.ChangeGameState(key);
}
