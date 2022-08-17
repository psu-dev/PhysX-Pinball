#pragma once

#include "BasicActors.h"
#include "ApplicationActors.h"
#include "EventHandler.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	using namespace std;

	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = {PxVec3(46.f/255.f,9.f/255.f,39.f/255.f),PxVec3(217.f/255.f,0.f/255.f,0.f/255.f),
		PxVec3(255.f/255.f,45.f/255.f,0.f/255.f),PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};

	// Vertices for the flippers
	static PxVec3 flipperVerts[] = {
				PxVec3(0.f, 0.f, 0.f),
				PxVec3(0.f, .2f, 0.f),
				PxVec3(1.f, -.05f, 0.f),
				PxVec3(1.f, .25f, 0.f),
				PxVec3(0.f, 0.f, 1.f),
				PxVec3(0.f, .2f, 1.f),
				PxVec3(1.f, -.05f, 1.f),
				PxVec3(1.f, .25f, 1.f) };

	struct FilterGroup
	{
		enum Enum
		{
			BALL		= (1 << 0),
			FLIPPERS	= (1 << 1),
			BUMPERS		= (1 << 2)
		};
	};

	///Custom scene class
	class MyScene : public Scene
	{
		// State scene components
		Plane* plane;
		PinballGeometry* playfield;
		Trampoline* plunger;
		Sphere* ball;
		Flipper* flippers[2];
		MySimulationEventCallback* my_callback;
		
	public:
		// State timer variables
		float timer = 0.f;
		float prevTime = 0.f;
		float delta_time = 1.f / 60.f;

		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene(CustomFilterShader) {};

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		}

		// State playfield pose variable for reference
		PxTransform poseRef;

		// Set colour palettes and materials
		PxVec3 baseColour = color_palette[2];
		PxVec3 detailColour = color_palette[0];
		PxVec3 bumperColour = color_palette[4];
		PxVec3 resetColour = color_palette[1];

		PxMaterial* ballMat = CreateMaterial(0.f, 0.f, .597f); // CoR: Steel ball bearing
		PxMaterial* playfieldMat = CreateMaterial(0.f, 0.f, 0.688f); // CoR: Hollow hard plastic
		PxMaterial* bumperMat = CreateMaterial(0.f, 0.f, 1.2f); // Bumpers propel the ball from them at a higher speed than impact

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);

			///Initialise and set the customised event callback
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);

			// Initialise the plane
			plane = new Plane();
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);

			// Initialise playfield
			PxTransform playfieldPose = PxTransform(PxVec3(0.f, 6.f, 0.f), PxQuat(-PxPi / 3.f, PxVec3(1.f, 0.f, 0.f)));
			PxVec3 playfieldDimensions = PxVec3(5.f, 8.f, .5f);
			playfield = new PinballGeometry(playfieldPose, playfieldDimensions, .2f);
			playfield->Color(detailColour);
			playfield->Color(baseColour, 7);
			Add(playfield);

			// Set reference pose value to playfield pose
			poseRef = playfieldPose;

			// Set details and add triangular supports to the scene
			for (int i = 0; i < 2; i++)
			{
				playfield->supports[i]->Color(detailColour);
				playfield->supports[i]->Material(playfieldMat);
				Add(playfield->supports[i]);
			}

			// Set details and add bumpers to the scene
			for (int i = 0; i < 6; i++)
			{
				playfield->bumpers[i]->Color(bumperColour);
				playfield->bumpers[i]->Material(bumperMat);
				Add(playfield->bumpers[i]);
			}

			// Set details and add triangular bumpers to the scene
			for (int i = 0; i < 2; i++)
			{
				playfield->triBumpers[i]->Color(bumperColour);
				playfield->triBumpers[i]->Material(bumperMat);
				Add(playfield->triBumpers[i]);
			}

			// Add reset zone to the scene
			playfield->resetZone->Color(resetColour);
			Add(playfield->resetZone);

			// Initialise the flippers
			flippers[0] = new Flipper(vector<PxVec3>(begin(flipperVerts), end(flipperVerts)), 
				PxTransform(playfieldPose.p + PxVec3(-1.6f, -2.9f, 4.f), playfieldPose.q * PxQuat(PxPi, PxVec3(0.f, 0.f, 1.f))),
				2.f, 0.f, PxPi / 3.f);
			flippers[1] = new Flipper(vector<PxVec3>(begin(flipperVerts), end(flipperVerts)),
				PxTransform(playfieldPose.p + PxVec3(0.9f, -2.9f, 4.3f), playfieldPose.q * PxQuat(-PxPi / 4.f, PxVec3(0.f, 0.f, 1.f))),
				2.f, 0.f, PxPi / 3.f);

			// Set details and add flippers to the scene
			for (int i = 0; i < 2; i++)
			{
				flippers[i]->Color(bumperColour);
				flippers[i]->Material(playfieldMat);
				Add(flippers[i]);
			}

			// Initialise the plunger and add to the scene
			plunger = new Trampoline(PxTransform(playfieldPose.p + PxVec3(4.5f, -4.35f, 6.f), playfieldPose.q), PxVec3(.25f, .25f, .25f), 1.f, 1.f);
			plunger->AddToScene(this);

			// Initialise the playfield ball
			ball = new Sphere(PxTransform(playfieldPose.p + PxVec3(4.5f, -3.5f, 5.f)), .15f, .1f);
			// Set deatils and add ball to the scene
			ball->Material(ballMat);
			Add(ball);
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
			// Reset if out of bounds
			if (my_callback->reset)
			{
				Reset();
			}

			// Update timer
			timer += delta_time;
		}

		void ResetInit(PxTransform pose)
		{
			// Initialise the playfield ball
			ball = new Sphere(PxTransform(pose.p + PxVec3(4.5f, -3.5f, 5.f)), .15f, .1f);
			// Set deatils and add ball to the scene
			ball->Material(ballMat);
			Add(ball);
		}

		// Apply force to flippers
		void FlipperForce(int id, PxReal force)
		{
			flippers[id]->GetJoint()->DriveVelocity(force);
		}

		// Apply force to plunger
		void PlungerForce(PxReal force)
		{
			plunger->AddForce(force);
		}

		// Reset the game upon failure
		void Reset()
		{
			// Return the reset callback to false
			my_callback->reset = false;

			// Remove and reset the ball
			px_scene->removeActor(*(PxActor*)ball->Get());
			delete ball;
			ResetInit(poseRef);

			// Set the previous best time and reset the timer to 0
			if (prevTime < timer)
			{
				prevTime = timer;
			}
		
			timer = 0.f;
		}
	};
}
