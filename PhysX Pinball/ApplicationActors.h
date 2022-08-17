#pragma once
// Header file defining specific actors for the pinball game.
// Including geometry for the playfield and moving parts such as the ball, plunger and flippers. 

#include "PhysicsEngine.h"
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>

namespace PhysicsEngine
{
	// Vertices for entry supports and triangular bumpers
	static PxVec3 supportVerts[] = {
				PxVec3(0.f, 0.f, 0.f),
				PxVec3(3.f, 0.f, 0.f),
				PxVec3(0.f, 3.f, 0.f),
				PxVec3(0.f, 0.f, 1.f),
				PxVec3(3.f, 0.f, 1.f),
				PxVec3(0.f, 3.f, 1.f) };

	static PxVec3 bumperVerts[] = {
				PxVec3(0.f, 0.f, 0.f),
				PxVec3(1.5f, 0.f, 0.f),
				PxVec3(0.f, 1.5f, 0.f),
				PxVec3(0.f, 0.f, 1.f),
				PxVec3(1.5f, 0.f, 1.f),
				PxVec3(0.f, 1.5f, 1.f) };

	// Faces for entry supports and triangular bumpers
	static PxU32 triangleTrigs[] = {
		0, 1, 2,
		0, 1, 3,
		1, 4, 3,
		1, 2, 5,
		1, 4, 5,
		0, 2, 5,
		0, 3, 5,
		3, 4, 5 };

	// Geometry class; playfield border, plunger chute, flipper supports and playfield details.
	class PinballGeometry : public StaticActor
	{
	public:
		// State arrays of playfield details
		TriangleMesh* supports[2];
		CapsuleStatic* bumpers[6];
		TriangleMesh* triBumpers[2];
		BoxStatic* resetZone;

		// Function to create the playfield
		PinballGeometry(const PxTransform pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.f, 1.f, 1.f), PxReal thickness = 1.f)
			: StaticActor(pose)
		{
			// Create main playfield geometry
			// Playfield border; top, bottom, left and right respectively
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x - thickness, thickness, dimensions.z)), 1.f);
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x - thickness, thickness, dimensions.z)), 1.f);
			CreateShape(PxBoxGeometry(PxVec3(thickness, dimensions.y + thickness, dimensions.z)), 1.f);
			CreateShape(PxBoxGeometry(PxVec3(thickness, dimensions.y + thickness, dimensions.z)), 1.f);

			// Plunger chute
			CreateShape(PxBoxGeometry(PxVec3(thickness / 2, dimensions.y * .75f, dimensions.z)), 1.f);

			// Flipper supports
			CreateShape(PxBoxGeometry(PxVec3(1.15f, thickness, dimensions.z)), 1.f);
			CreateShape(PxBoxGeometry(PxVec3(1.15f, thickness, dimensions.z)), 1.f);

			// Playfield bottom
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y, dimensions.z / 2.f)));



			// Transform playfield geometry to desired position
			// Playfield border; top, bottom, left and right respectively
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.f, dimensions.y, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.f, -dimensions.y, 0.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-dimensions.x, 0.f, 0.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(dimensions.x, 0.f, 0.f)));

			// Plunger chute
			GetShape(4)->setLocalPose(PxTransform(PxVec3(dimensions.x - (thickness * 4.f), -2.f, 0.f)));

			// Flipper supports
			GetShape(5)->setLocalPose(PxTransform(PxVec3(-2.7f, -4.5f, 0.f), PxQuat(-PxPi / 6.f, PxVec3(0.f, 0.f, 1.f))));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(2.1f, -4.5f, 0.f), PxQuat(PxPi / 6.f, PxVec3(0.f, 0.f, 1.f))));
			
			// Top supports
			supports[0] = new TriangleMesh(vector<PxVec3>(begin(supportVerts), end(supportVerts)), vector<PxU32>(begin(triangleTrigs), end(triangleTrigs)),
				PxTransform(pose.p + PxVec3(4.8f, 3.4f, -7.1f), pose.q * PxQuat(-PxPi, PxVec3(0.f, 0.f, 1.f))));
			supports[1] = new TriangleMesh(vector<PxVec3>(begin(supportVerts), end(supportVerts)), vector<PxU32>(begin(triangleTrigs), end(triangleTrigs)),
				PxTransform(pose.p + PxVec3(-4.8f, 3.4f, -7.1f), pose.q * PxQuat(-PxPi / 2.f, PxVec3(0.f, 0.f, 1.f))));

			// Playfield bottom
			GetShape(7)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, -dimensions.z * 1.5f)));

			// Playfield obstacles
			// Bumpers
			bumpers[0] = new CapsuleStatic(PxTransform(pose.p + PxVec3(.55f, .5f, -2.f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));
			bumpers[1] = new CapsuleStatic(PxTransform(pose.p + PxVec3(-.95f, .5f, -2.f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));
			bumpers[2] = new CapsuleStatic(PxTransform(pose.p + PxVec3(1.55f, 1.5f, -3.5f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));
			bumpers[3] = new CapsuleStatic(PxTransform(pose.p + PxVec3(-1.95f, 1.5f, -3.5f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));
			bumpers[4] = new CapsuleStatic(PxTransform(pose.p + PxVec3(1.55f, -.25f, -.5f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));
			bumpers[5] = new CapsuleStatic(PxTransform(pose.p + PxVec3(-1.95f, -.25f, -.5f), pose.q * PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec2(.375f, .375f));

			// Triangular bumpers
			triBumpers[0] = new TriangleMesh(vector<PxVec3>(begin(bumperVerts), end(bumperVerts)), vector<PxU32>(begin(triangleTrigs), end(triangleTrigs)),
				PxTransform(pose.p + PxVec3(3.025f, -1.3f, 1.25f), pose.q * PxQuat(-PxPi / 4.f, PxVec3(0.f, 0.f, 1.f))));
			triBumpers[1] = new TriangleMesh(vector<PxVec3>(begin(bumperVerts), end(bumperVerts)), vector<PxU32>(begin(triangleTrigs), end(triangleTrigs)),
				PxTransform(pose.p + PxVec3(-3.75f, -1.3f, 1.25f), pose.q * PxQuat(PxPi - PxPi / 4.f, PxVec3(0.f, 0.f, 1.f))));


			// Extra playfield details
			// Configure reset zone
			resetZone = new BoxStatic(PxTransform(pose.p + PxVec3(-.25f, -3.5f, 6.f), pose.q), PxVec3((dimensions.x - thickness * 3.f), dimensions.y / 10.f, dimensions.z));
			resetZone->SetTrigger(true);
			// Set name for reference for callbacks
			((PxRigidActor*)resetZone->Get())->setName("reset");
		}
	};

	// Trampoline class: contains springs with box ends, forming the plunger.
	class Trampoline
	{
		vector<DistanceJoint*> springs;
		Box* top;
		// Bottom box adjusted to be static to not move when force is applied to the top box
		BoxStatic* bottom;

	public:
		Trampoline(const PxTransform& pose = PxTransform(PxIdentity), const PxVec3& dimensions = PxVec3(1.f, 1.f, 1.f), PxReal stiffness = 1.f, PxReal damping = 1.f)
		{
			PxReal thickness = .1f;
			bottom = new BoxStatic(PxTransform(pose.p + PxVec3(0.f, thickness * 6, 0.f), pose.q * PxQuat(-PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec3(dimensions.x, thickness, dimensions.z));
			top = new Box(PxTransform(pose.p + PxVec3(0.f, dimensions.y + thickness, 0.f), pose.q), PxVec3(dimensions.x, thickness, dimensions.z));
			springs.resize(4);
			springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
			springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
			springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
			springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));

			for (unsigned int i = 0; i < springs.size(); i++)
			{
				springs[i]->Stiffness(stiffness);
				springs[i]->Damping(damping);
			}
		}

		void AddToScene(Scene* scene)
		{
			scene->Add(bottom);
			scene->Add(top);
		}

		// Apply force to the top box
		void AddForce(PxReal force)
		{
			((PxRigidDynamic*)top->Get())->addForce(PxVec3(0.f, force / 2.f, -force));
		}

		~Trampoline()
		{
			for (unsigned int i = 0; i < springs.size(); i++)
				delete springs[i];
		}
	};

	// Flipper class: A flipper mesh attached to a revolving joint
	class Flipper : public ConvexMesh
	{
		// Joint the flipper is attached to a rotates around
		RevoluteJoint* joint;

		// Joint limits
		PxReal upperLimit;
		PxReal lowerLimit;

	public:
		// Constructor
		Flipper(const vector<PxVec3>& verts, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f, PxReal upper = 1.f, PxReal lower = 1.f)
			:ConvexMesh(verts, pose, density)
		{
			// Set new joint dependent on the flipper pose
			joint = new RevoluteJoint(nullptr, PxTransform(pose.p, pose.q * PxQuat(-PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), this, PxTransform(PxVec3(1.f, 0.f, 0.f), PxQuat(-PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))));
			// Set the joint limits
			upperLimit = upper;
			lowerLimit = lower;
			joint->SetLimits(upperLimit, lowerLimit);
		}

		// Returns the joint for reference in the scene
		RevoluteJoint* GetJoint()
		{
			return joint;
		}

		// Deconstructor
		~Flipper()
		{
			delete joint;
		}
	};
}
