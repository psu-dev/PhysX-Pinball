#include <iostream>
#include "VisualDebugger.h"

using namespace std;

// Jamie Fixter (17644694) - CGP3021M Physics Simulation Assessment 1

// This pinball simulation is a time-based scoring system with two different types of bumpers. Aim for the best time possible!
// There are bumpers which utilise capsules and custom TriangleMesh StaticActors, in the form of triangles. The bumpers have a coefficent of
// restitution (CoR) which is greater than 1, to give propulsion to the ball. Other CoR for the ball and playfield were given here:
// https://hypertextbook.com/facts/2006/restitution.shtml. The same triangle setup was used for entry supports at the top of the playfield,
// allowing for a smoother entry. The Trampoline function was adjusted to have a static Box actor for the bottom, to stop the entire plunger
// from moving once force is applied. The PlayfieldGeometry function within the ApplicationActors.h file handles the production of all the playfield
// geometry elements; bottom piece, borders, flipper supports, reset zone, plunger chute, entry supports and both bumper types. The flippers are also
// contained within the ApplicationActor.h file, and are DynamicActors using 8 vertices to produce a thin trapezium shape. The flippers revolve around a
// joint with no dependency on any other actors, and have force applied to them through keyboard inputs. This also applies to the plunger. The reset function
// is driven through the reset zone at the bottom of the playfield, which acts as a trigger to signal resetting the game and activates once the ball falls
// below the flippers. The time will be reset, and the best time will be updated if it is greater than the previous.

int main()
{
	try 
	{ 
		VisualDebugger::Init("PhysX Pinball", 800, 800); 
	}
	catch (Exception exc) 
	{ 
		cerr << exc.what() << endl;
		return 0; 
	}

	VisualDebugger::Start();

	return 0;
}