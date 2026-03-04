#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "ofMain.h"
#include "RegrowthMaster.h"
#include <cmath>

// The game manager will hold all the game logic with functions for each specific
// feature required

class GameManager
{
    private:
        enum GameState 
		{
			TITLE,
			GAMEPLAY,
			GAMEOVER
		};

		GameState currentState = TITLE;
		ofImage titleImage;
		ofImage endImage;

        float systemStartTime = 0.0f;
		CustomCamera camera;

        // Player
		std::unique_ptr<Player> player;

		// Witherers
		ofShader witherer_shader;
		EntitySystem<Witherer> witherers_system;
		size_t numWitherers = 5;
		ofImage coneTexture;
		ofImage sphereTexture;
        
		// Terrain
		ofShader terrain_shader;
		EntitySystem<Terrain> terrain_system;	// holds the garden and wasteland terrains
        int terrain_width = 2000;
        int terrain_height = 2000;
        int resolution = 200;

		// Particles
		ofShader falling_shader;
		ofShader storm_shader;
		ofImage gardenParticleImage;
		ofImage wastelandParticleImage;
		ofImage pollenParticleImage;
		EntitySystem<FallingParticles> particles_system;
		EntitySystem<StormParticles> storm_particles_system;
		size_t numStorms = 3;

		ofShader visibility_shader;
		ofShader withered_shader;
		EntitySystem<ScreenEffect> effects_system;
		ofFbo sceneFbo;

		// Procedural Plants (can easily add more plant types if desired)
        size_t numPlantTypes = 3;   // replace with however many we add
        
		ofShader plant_shader;
		EntitySystem<ProceduralPlant> plants_system;
        size_t numGardenPlants = 10;
		size_t numWastelandPlants = 3;

		// Skybox
		ofShader skybox_shader;
        ofImage skyboxImage;
		std::unique_ptr<Skybox> skybox;

		// Plant Particles
		ofShader plant_particles_shader;
		ofImage plantParticleImage;
		EntitySystem<PlantParticles> plant_particles_system;

        // Plantable Areas
        EntitySystem<PlantableArea> plantable_areas_system;
        size_t numPlantAreas = 10;
        ofImage plantableAreaTexture;
        
        // Seeds
        EntitySystem<Seed> seeds_system;
        size_t numSeeds = 10;        // start off with 10 seeds
        size_t collectedSeeds = 0;   // player hasn't collected any seeds yet
        ofImage seedInventoryImage;
        ofImage seedTexture;
        ofShader seed_shader;
        size_t seedsPlanted = 0;

        // Plant door
        std::unique_ptr<PlantDoor> plant_door;

        //Fertilizer
		EntitySystem<Fertilizer> fertilizer_system;
		size_t numFertilizer = 5;		// start off with 5 fertilizer
		size_t collectedFertilizer = 0;	// player hasn't collected any fertilizer yet
		ofImage fertilizerInventoryImage;
		ofImage fertilizerTexture;
		ofShader fertilizer_shader;

        // Plant wall
        std::unique_ptr<PlantWall> plant_wall;

        // Progress bar
		ofImage progressBarUnfinished;
		ofImage progressBarFinished;
        
        // --- Helpers ---
        bool checkInsideStorm(StormParticles& storm) 
        {
            float dx = camera.getPosition().x - storm.GetStormCenter().x;
            float dz = camera.getPosition().z - storm.GetStormCenter().z;
            float horizontalDist = sqrt(dx * dx + dz * dz);

            bool insideHoriz = (horizontalDist <= storm.GetStormRadius());
            bool insideVert = (camera.getPosition().y >= storm.GetStormCenter().y - storm.GetStormHeight() && camera.getPosition().y <= storm.GetStormCenter().y + storm.GetStormHeight());

            return insideHoriz && insideVert;
        }

        bool checkInsideWither(Witherer & witherer) 
        {
			if (witherer.GetIsHealed()) {
				return false;
			}

			float dx = camera.getPosition().x - witherer.GetTransform().position.x;
            float dz = camera.getPosition().z - witherer.GetTransform().position.z;
            float horizontalDist = sqrt(dx * dx + dz * dz);

            return (horizontalDist <= witherer.GetEffectRadius());
        }

        bool checkCanPlant(PlantableArea& plantableArea) {
			for (size_t i = 0; i < witherers_system.GetSize(); i++) {
				auto & witherer = witherers_system.GetEntityAt(i);

				if (witherer.GetIsHealed()) {
					continue;
				}

				glm::vec3 distance = plantableArea.GetTransform().position - witherer.GetTransform().position;
				float diagonal = sqrt(distance.x * distance.x + distance.z * distance.z);

				if (diagonal <= witherer.GetEffectRadius()) {
					return false;
				}
			}
			return true;
		}

        // Helper to create the procedural plants to keep the Setup cleaner
        void createProceduralPlant(Transform t, std::string axiom, RuleMap rules, int iterations, float segLen, float angle, bool inGarden) 
        {
            if (inGarden) 
            {
                glm::vec4 baseCol = glm::vec4(0.0f, ofRandom(0.3f, 1.0f), 0.0f, 1.0f);
                glm::vec4 leafCol = glm::vec4(ofRandom(0.3f, 1.0f), 0.0f, ofRandom(0.3f, 1.0f), 1.0f);

                plants_system.AddEntity(std::make_unique<ProceduralPlant>(
                    t, plant_shader, axiom, rules,
                    iterations, segLen, angle,
                    baseCol, leafCol, inGarden));

                // GENERATE THE GEOMETRY ONCE
                plants_system.GetEntityAt(plants_system.GetSize() - 1).GenerateGeometry();
            } 
            else 
            {
                glm::vec4 baseCol = glm::vec4(0.33f, 0.24f, 0.2f, 1.0f);
                glm::vec4 leafCol = glm::vec4(0.24f, 0.12f, 0.04f, 1.0f);

                plants_system.AddEntity(std::make_unique<ProceduralPlant>(
                    t, plant_shader, axiom, rules,
                    iterations, segLen, angle,
                    baseCol, leafCol, inGarden));

                // GENERATE THE GEOMETRY ONCE
                plants_system.GetEntityAt(plants_system.GetSize() - 1).GenerateGeometry();
            }
        }

        void createPlantParticles(size_t index)
        {
            glm::vec3 scale = glm::vec3(5.0f,20.0f,5.0f);
            Transform t = Transform();
            t.SetPosition(plants_system.GetEntityAt(index).GetTransform().position);
            t.position.y += 10.0f;
            t.SetScale(scale);
            plant_particles_system.AddEntity(std::make_unique<PlantParticles>
            (
                t, plant_particles_shader, gardenParticleImage
            ));
        }

        // Draw title, gameplay, gameover
        void drawTitleScreen() 
        {
            titleImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        }

        void drawGameplay() 
        {
            // draw scene into FBO
            sceneFbo.begin();
            ofClear(0, 0, 0, 255);

            // draw the terrain, particles and plants
            camera.begin();

            terrain_system.DrawAll(camera);             // Draw Terrains
            plants_system.DrawAll(camera);              // Draw Plants
            witherers_system.DrawAll(camera);           // Draw Witherers
            plantable_areas_system.DrawAll(camera);     // Draw Planting Areas
            seeds_system.DrawAll(camera);               // Draw the seeds
            plant_door->Draw(camera);                   // Draw Plant Door
            fertilizer_system.DrawAll(camera);			// Draw the fertilizer
            plant_wall->Draw(camera);                   // Draw the plant wall

            // Draw the skybox (Draw the particles AFTER the skybox is drawn so they are influenced by it)
            skybox->Draw(camera);

            particles_system.DrawAll(camera);               // Draw Particles
            plant_particles_system.DrawAll(camera);     // Draw Plant Particles
            storm_particles_system.DrawAll(camera);         // Draw Storm Particles

            camera.end();

            sceneFbo.end();

            // draw effects and fbo
            bool drawn = false;

            for (size_t i = 0; i < effects_system.GetSize(); i++) 
            {
                if (effects_system.GetEntityAt(i).getDoDraw()) 
                {
                    effects_system.GetEntityAt(i).Draw(camera);
                    drawn = true;
                    break;
                }
            }

            if (!drawn) 
            {
                ofPushMatrix();
                ofTranslate(0, ofGetHeight());
                ofScale(1, -1);
                sceneFbo.draw(-1, -1, ofGetWidth(), ofGetHeight());
                ofPopMatrix();
            }

            drawCrosshair(2.0f, ofColor::white);

            drawInventoryUI();
        }

        void drawGameover() 
        {
            endImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        }

        void drawCrosshair(float size, ofColor color)
        {
            // Draw a small tiny circle at the center of the screen to act as a crosshair
            ofPushStyle();
            ofDisableDepthTest();

            ofSetColor(color);
            ofDrawCircle(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, size);

            ofEnableDepthTest();
            ofPopStyle();
        }

        void drawInventoryUI() {
			ofPushStyle();
			ofDisableDepthTest();

			ofSetColor(255);
			seedInventoryImage.draw(glm::vec2(0, 0), 128.0f, 128.0f);
			fertilizerInventoryImage.draw(glm::vec2(128.0f, 0), 128.0f, 128.0f);

			std::string numberSeeds = std::to_string(collectedSeeds);
			std::string numberFertilizer = std::to_string(collectedFertilizer);

			ofSetColor(ofColor::black);
			ofDrawBitmapString(numberSeeds, glm::vec2(102.0f, 114.0f));
			ofDrawBitmapString(numberFertilizer, glm::vec2(230.0f, 114.0f));

			float barLength = 500.0f;
			float barHeight = 40.0f;
			glm::vec2 barLocation = glm::vec2(300.0f, 20.0f);
			float progressLength = barLength * (double(seedsPlanted) / double(numPlantAreas));

			ofSetColor(255);
			progressBarUnfinished.draw(barLocation, barLength, barHeight);
			progressBarFinished.draw(barLocation, progressLength, barHeight);

			ofEnableDepthTest();
			ofPopStyle();
		}

        void gatherSeed() { collectedSeeds++; }

        void gatherFertilizer() { collectedFertilizer++; }

        void useSeed()
        {
            if (collectedSeeds <= 0)
            {
                collectedSeeds = 0;
            }
            else
            {
                collectedSeeds--;
            }
            seedsPlanted++;
        }

        void useFertilizer() {
			if (collectedFertilizer <= 0) {
				collectedFertilizer = 0;
			} else {
				collectedFertilizer--;
			}
		}

    public:
        // Setup Functions!
        void LoadImages()
        {
            titleImage.load("images/regrowthTitlePage.png");
	        endImage.load("images/regrowthEndPage.png");
            gardenParticleImage.load("images/free-to-use-pink-spritesheet.png");
            wastelandParticleImage.load("images/snowsheet4x4.jpg");
            pollenParticleImage.load("images/pollenparticle.png");
            skyboxImage.load("images/skybox_texture.png");
            seedInventoryImage.load("images/seed_inventory_picture.png");
            seedTexture.load("images/seed_texture.png");
            plantableAreaTexture.load("images/plantable_area_texture.png");
            fertilizerInventoryImage.load("images/fertilizer_inventory_picture.png");
			fertilizerTexture.load("images/fertilizerTexture.png");
            coneTexture.load("images/cone_texture.png");
			sphereTexture.load("images/sphere_texture.png");
			progressBarUnfinished.load("images/progress_bar_unfinished.png");
			progressBarFinished.load("images/progress_bar_finished.png");
        }

        void LoadShaders()
        {
            terrain_shader.load("shaders/terrain.vert", "shaders/terrain.frag");
            falling_shader.load("shaders/falling.vert", "shaders/falling.frag", "shaders/falling.geom");
            storm_shader.load("shaders/storm.vert", "shaders/storm.frag", "shaders/storm.geom");
            plant_shader.load("shaders/plant_shader.vert", "shaders/plant_shader.frag");
            visibility_shader.load("shaders/storm_visibility.vert", "shaders/storm_visibility.frag");
            withered_shader.load("shaders/withered_effect.vert", "shaders/withered_effect.frag");
            skybox_shader.load("shaders/skybox.vert", "shaders/skybox.frag");
            plant_particles_shader.load("shaders/plant_particles.vert", "shaders/plant_particles.frag", "shaders/plant_particles.geom");
            witherer_shader.load("shaders/witherer.vert", "shaders/witherer.frag");
            seed_shader.load("shaders/seed.vert", "shaders/seed.frag");
            fertilizer_shader.load("shaders/fertilizer.vert", "shaders/fertilizer.frag");
        }

        void SetupTerrain()
        {
            // --- TERRAIN SETUP ---
            Transform t1 = Transform();
            t1.rotation.x = -90.0f;
            t1.SetPosition(glm::vec3(-terrain_width*0.5f, 0.0f, 0.0f));

            // Create Garden terrain
            terrain_system.AddEntity(std::make_unique<GardenTerrain>(
                t1, terrain_shader, terrain_width,
                terrain_height, resolution
            ));

            Transform t2 = Transform();
            t2.rotation.x = -90.0f;
            t2.SetPosition(glm::vec3(terrain_width*0.5f, 0.0f, 0.0f));

            // Create Wasteland terrain
            terrain_system.AddEntity(std::make_unique<WastelandTerrain>(
                t2, terrain_shader, terrain_width,
                terrain_height, resolution
            ));

            // -----------------------
        }

        void SetupParticles()
        {
            // --- PARTICLES SETUP ---
            ofDisableArbTex();
            ofDisableDepthTest();

            ofEnableDepthTest();
            // Create the wasteland falling particles
            Transform t3 = Transform();
            particles_system.AddEntity(std::make_unique<WastelandFallingParticles>(t3, falling_shader, 500, wastelandParticleImage));
            particles_system.GetEntityAt(0).Setup();

            // Create the garden falling particles
            Transform t4 = Transform(glm::vec3(30, 0, 0), glm::vec3(0,0,0), glm::vec3(1,1,1));
            particles_system.AddEntity(std::make_unique<GardenFallingParticles>(t4, falling_shader, 500, gardenParticleImage));
            particles_system.GetEntityAt(1).Setup();

            // Create the storm particles
            for (size_t i = 0; i < numStorms; i++) 
            {
                Transform tTemp = Transform();
                float r = ofRandom(200.0, 400.0);
                float h = ofRandom(50.0, 80.0);
                glm::vec3 c = glm::vec3(ofRandom(-terrain_width + r, -r), h - 5, ofRandom(-terrain_width * 0.5 + r, terrain_width * 0.5 - r));

                storm_particles_system.AddEntity(std::make_unique<StormParticles>(tTemp, storm_shader, 5000, pollenParticleImage, c, r, h));
                storm_particles_system.GetEntityAt(i).Setup();
            }

            // ----------------------
        }

        void SetupProceduralPlants()
        {
            // -- L-SYSTEMS SETUP ---
            // RuleMap is an unordered_map<char, std::string>
            float edgeX = 200.0f;

            PlantType1 pt1;
            PlantType2 pt2;
            PlantType3 pt3;

            // Spawn some garden plants
            for (size_t i = 0; i < numGardenPlants; ++i)
            {
                Transform t = Transform();
                t.SetPosition(glm::vec3(ofRandom(-terrain_width + edgeX, -10.f), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt1.axiom, pt1.rules, pt1.iterations, pt1.segLen, pt1.angle, true);

                t.SetPosition(glm::vec3(ofRandom(-terrain_width + edgeX, -10.f), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt2.axiom, pt2.rules, pt2.iterations, pt2.segLen, pt2.angle, true);

                t.SetPosition(glm::vec3(ofRandom(-terrain_width + edgeX, -10.f), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt3.axiom, pt3.rules, pt3.iterations, pt3.segLen, pt3.angle, true);
            }

            // Spawn some wasteland plants
            for (size_t i = 0; i < numWastelandPlants; ++i)
            {
                Transform t = Transform();
		        t.SetPosition(glm::vec3(ofRandom(10.f, terrain_width - edgeX), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt1.axiom, pt1.rules, pt1.iterations, pt1.segLen, pt1.angle, false);

		        t.SetPosition(glm::vec3(ofRandom(10.f, terrain_width - edgeX), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt2.axiom, pt2.rules, pt2.iterations, pt2.segLen, pt2.angle, false);

		        t.SetPosition(glm::vec3(ofRandom(10.f, terrain_width - edgeX), 0.0f, ofRandom(-terrain_height * 0.5f + 10.f, terrain_height * 0.5f - 10.f)));
                createProceduralPlant(t, pt3.axiom, pt3.rules, pt3.iterations, pt3.segLen, pt3.angle, false);
            }
            // ----------------------
        }

        void SetupScreenSpaceEffects()
        {
            // --- SSE's SETUP --------
            ofFbo::Settings s;
            s.width = ofGetWidth();
            s.height = ofGetHeight();
            s.internalformat = GL_RGBA;
            s.useDepth = true;
            s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
            sceneFbo.allocate(s);

            // clear it once
            sceneFbo.begin();
            ofClear(0, 0, 0, 255);
            sceneFbo.end();

            sceneFbo.getTexture().setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
            sceneFbo.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

            Transform t8 = Transform();
            effects_system.AddEntity(std::make_unique<StormScreenEffect>(t8, visibility_shader, sceneFbo, true));
            effects_system.GetEntityAt(0).Setup();

            effects_system.AddEntity(std::make_unique<WitheredScreenEffect>(t8, withered_shader, sceneFbo, false));
            effects_system.GetEntityAt(1).Setup();
            // ------------------------
        }

        void SetupPlayer()
        {
            // --- PLAYER SETUP -------
            Transform t9 = Transform();
            t9.SetPosition(glm::vec3(-1800,5,0));

            player = std::make_unique<Player>
            (
                t9, plant_shader, 150.0f
            );

            // ------------------------
        }

        void SetupSkybox()
        {
            // --- SKYBOX SETUP ---
            Transform t10 = Transform();
            t10.SetPosition(glm::vec3(0,0,0));

            skybox = std::make_unique<Skybox>
            (
                t10, skybox_shader, 4000.0f, skyboxImage
            );

            // --------------------
        }

        void SetupPlantParticles()
        {
            // --- PLANT PARTICLE SETUP ---
            // The plant particles just need to be attached to the plants via their transform
            // Only draw plant particles for the ones in the garden
            for (size_t i = 0; i < plants_system.GetSize(); ++i)
            {
                auto& plant = plants_system.GetEntityAt(i);

                if (plant.GetInGarden())
                {
                    createPlantParticles(i);
                }
            }

            // ----------------------------
        }

        void SetupWitherers() {
			// --- WITHERERS SETUP ---
			for (size_t i = 0; i < numWitherers; i++) {
				Transform tTemp = Transform();
				tTemp.SetPosition(glm::vec3(ofRandom(100, terrain_width - 300), 10, ofRandom(-terrain_width * 0.5 + 100, terrain_width * 0.5 - 100)));
				witherers_system.AddEntity(std::make_unique<Witherer>(tTemp, witherer_shader, coneTexture, sphereTexture));
			}
			// --------------------
		}

        void SetupPlantableAreas()
        {
            for (size_t i = 0; i < numPlantAreas; ++i)
            {
                Transform tTemp = Transform();
                tTemp.SetPosition(glm::vec3(ofRandom(200, terrain_width - 300), -20.0f, ofRandom(-terrain_width * 0.5 + 200, terrain_width * 0.5 - 200)));
                plantable_areas_system.AddEntity(std::make_unique<PlantableArea>(tTemp, seed_shader, 30.0f, plantableAreaTexture));
            }   
        }

        void SetupSeeds()
        {
            float minX = -terrain_width;
            float maxX = 0.0f;

            float minZ = -terrain_height * 0.5f;
            float maxZ = terrain_height * 0.5f;

            for (size_t i = 0; i < numSeeds; ++i)
            {
                float x = ofRandom(minX, maxX);
                float z = ofRandom(minZ, maxZ);
                GardenTerrain& terrain = static_cast<GardenTerrain&>(terrain_system.GetEntityAt(0));
                float y = terrain.GetHeightAt(x, z) + 1.0f;

                Transform tTemp = Transform();
                tTemp.SetPosition(glm::vec3(x, y, z));
                seeds_system.AddEntity(std::make_unique<Seed>(tTemp, seed_shader, 5.0f, seedTexture));
            }
        }

        void SetupPlantDoor()
        {
            Transform t = Transform();
            plant_door = std::make_unique<PlantDoor>
            (
                t, plant_shader
            );
        }

        void SetupFertilizer() {
			float minX = -terrain_width;
			float maxX = 0.0f;

			float minZ = -terrain_height * 0.5f;
			float maxZ = terrain_height * 0.5f;

			for (size_t i = 0; i < numFertilizer; ++i) {
				float x = ofRandom(minX, maxX);
				float z = ofRandom(minZ, maxZ);
				GardenTerrain & terrain = static_cast<GardenTerrain &>(terrain_system.GetEntityAt(0));
				float y = terrain.GetHeightAt(x, z) + 5.0f;

				Transform tTemp = Transform();
				tTemp.SetPosition(glm::vec3(x, y, z));
				cout << "Fertilizer postions:\n" << tTemp.position << endl;
				fertilizer_system.AddEntity(std::make_unique<Fertilizer>(tTemp, fertilizer_shader, 10.0f, 15.0f, 5.0f, fertilizerTexture));
			}
		}

        void SetupPlantWall()
        {
            Transform t = Transform();
            plant_wall = std::make_unique<PlantWall>
            (
                t, plant_shader
            );
        }

        // Main loop functions
        void Update()
        {
            if (currentState != GAMEPLAY) 
            {
                return;
            }

            if (seedsPlanted >= numSeeds) {
				currentState = GAMEOVER;
				return;
			}

            float dt = ofGetLastFrameTime();
            player->Update(camera.getqForward(), camera.getqSide(), dt);
            camera.Update(*player, dt);

            for (size_t i = 0; i < particles_system.GetSize(); i++) {
                particles_system.GetEntityAt(i).GetTransform().SetPosition(player->GetTransform().position);
            }

            particles_system.UpdateAll();
            storm_particles_system.UpdateAll();
            effects_system.UpdateAll();
            witherers_system.UpdateAll();
            plant_door->Update();

            // Check if plant_door should be opening
            if (glm::distance(plant_door->GetTransform().position, player->GetTransform().position) < 100.0f)
            {
                plant_door->SetIsOpening(true);
            }
            else
            {
                plant_door->SetIsOpening(false);
            }

            // Pollen storm effect
            bool insideStorm = false;
            for (size_t i = 0; i < storm_particles_system.GetSize(); i++) 
            {
                insideStorm = checkInsideStorm(storm_particles_system.GetEntityAt(i));

                if (insideStorm) 
                {
                    break;
                }
            }

            if (insideStorm)
            {
                effects_system.GetEntityAt(0).setDoDraw(true);
                effects_system.GetEntityAt(0).setTarget(1.0f);
            } 
            else 
            {
                effects_system.GetEntityAt(0).setTarget(0.0f);
            }

            // Withering effect
            bool insideWither = false;
            for (size_t i = 0; i < witherers_system.GetSize(); i++) 
            {
                insideWither = checkInsideWither(witherers_system.GetEntityAt(i));

                if (insideWither) 
                {
                    break;
                }
            }

            if (insideWither) 
            {
                effects_system.GetEntityAt(1).setDoDraw(true);
                effects_system.GetEntityAt(1).setTarget(1.0f);
            } 
            else 
            {
                effects_system.GetEntityAt(1).setTarget(0.0f);
            }
            
            // Alter player speed if withered
            if (insideStorm) 
            {
                player->SetSpeed(75.0f);
            } 
            else if (insideWither) 
            {
                player->SetSpeed(100.0f);
            } 
            else 
            {
                player->SetSpeed(150.0f);
            }

            // Ray collision between plantable areas and seeds
            if (ofGetKeyPressed('f'))
            {
                Ray ray;
                ray.SetOrigin(camera.getPosition());
                ray.SetDirection(glm::normalize(camera.getqForward()));

                float hitDistance;

                // seed picking
                for (size_t i = 0; i < numSeeds; ++i)
                {
                    auto& seed = seeds_system.GetEntityAt(i);
                    if (ray.RaySphereIntersect(seed.GetTransform().position, seed.GetRadius(), hitDistance) && !seed.GetPickedUp())
                    {
                        seed.SetPickedUp(true);
                        gatherSeed();  // to be used by ui and planting functionality
                        printf("Picked up seed!\n");
                        printf("Number seeds collected: %lu\n", collectedSeeds);
                    }
                }

                // fertilizer picking
				for (size_t i = 0; i < numFertilizer; i++) {
					auto & fertilizer = fertilizer_system.GetEntityAt(i);
					if (ray.RaySphereIntersect(fertilizer.GetTransform().position, fertilizer.GetHeight() * 0.75f, hitDistance) && !fertilizer.GetPickedUp()) {
						fertilizer.SetPickedUp(true);
						gatherFertilizer(); // to be used by ui and planting functionality
						printf("Picked up fertilizer!\n");
						printf("Number fertilizer collected: %lu\n", collectedFertilizer);
					}
				}

                // plantable areas and planting a plant
                for (size_t i = 0; i < numPlantAreas; ++i)
                {
                    auto& plantableArea = plantable_areas_system.GetEntityAt(i);
                    if (ray.RaySphereIntersect(plantableArea.GetTransform().position, plantableArea.GetRadius(), hitDistance) 
                        && !plantableArea.GetPlantedIn() && collectedSeeds > 0 && checkCanPlant(plantableArea))
                    {
                        size_t randomChoice = rand() % numPlantTypes;
                        Transform t = plantableArea.GetTransform();
                        WastelandTerrain& terrain = static_cast<WastelandTerrain&>(terrain_system.GetEntityAt(1));
                        t.position.y = terrain.GetHeightAt(t.position.x, t.position.z);

                        if (randomChoice == 0)
                        {
                            PlantType1 pt;
                            createProceduralPlant(t, pt.axiom, pt.rules, pt.iterations, pt.segLen, pt.angle, true);
                            printf("Creating Plant Type 1!\n");
                        }
                        else if (randomChoice == 1)
                        {
                            PlantType2 pt;
                            createProceduralPlant(t, pt.axiom, pt.rules, pt.iterations, pt.segLen, pt.angle, true);
                            printf("Creating Plant Type 2!\n");
                        }
                        else if (randomChoice == 2)
                        {
                            PlantType3 pt;
                            createProceduralPlant(t, pt.axiom, pt.rules, pt.iterations, pt.segLen, pt.angle, true);
                            printf("Creating Plant Type 3!\n");
                        }

                        useSeed();  // use a seed
                        plantableArea.SetPlantedIn(true);
                        createPlantParticles(plants_system.GetSize()-1);
                    }
                }

                for (size_t i = 0; i < numWitherers; ++i) {
					auto & witherer = witherers_system.GetEntityAt(i);
					if (ray.RaySphereIntersect(witherer.GetTransform().position, witherer.GetInteractRadius(), hitDistance)
						&& !witherer.GetIsHealed() && collectedFertilizer > 0) {

						useFertilizer(); // use a seed
						witherer.SetIsHealed(true);
					}
				}
            }
        }

        void Draw()
        {
            if (currentState == TITLE) 
            {
                drawTitleScreen();
                return;
            }

            if (currentState == GAMEPLAY) 
            {
                drawGameplay();
                return;	
            }

            if (currentState == GAMEOVER) 
            {
                drawGameover();
                return;
            }
        }

        void ChangeGameState(int key)
        {
            if (currentState == TITLE && key == ' ') 
            {
                currentState = GAMEPLAY;
            }
        }
};

#endif