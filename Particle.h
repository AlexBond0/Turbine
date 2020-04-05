#pragma once

#include <deque>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "InstancedObject.h"

// the type of particle generated
enum class ParticleType {
	FIRE,
	SMOKE_WHITE,
	SMOKE_BLACK
};

// the profile of the generation of particles
struct ParticleProfile {

	float position[3];	// current particle position
	float speed[3];		// current particle speed vector

	float colour[4];	// particle colour
	float weight;		// particle weight
	int spread;			// particle random spread

	double life;		// time till particle death
};

// sortable vertex struct for transparency ordering
struct vertex {
	float x;
	float y;
	float z;
	float dist;

	bool operator<(vertex& that) {

		return this->dist > that.dist;
	}
};

class Particle
	: public InstancedObject
{
public:

	bool active;				// is the particle generator active
	ParticleProfile profile;	// the particle generation profile

	bool isTransparent;			// are the particles transparrent

	Particle(int max, int perms);
	Particle(ParticleType type, int max, int perms);
	Particle(ParticleProfile profile, int max, int perms);

	~Particle();

	void Draw(RenderingContext rcontext);
	void BlendDraw(RenderingContext rcontext);

	void SetProfilePosition(float x, float y, float z);
	void SetProfileSpeed(float x, float y, float z);

	void Particle::Update(long step);

protected:

	int maxParticles;		// maximum particles in generator at one time
	int particlesPerms;		// particles generated per millisecond

	virtual void _AssignHandleInformation(RenderingContext& rcontext);

private:

	long time_ms;			// current animation time

	float billboard_u[3];	// billboard up vector
	float billboard_r[3];	// billboard right vector

	float inverseView[16];	// inverse view matrix
	float* modelView;		// model view matrix

	float* rawParticleData;	// raw partcile data passed to instance VBO

	void _SetupParticle(int max, int perms);

	void _CleanOldParticles();
	void _GenerateNewParticles(long step);
	void _UpdateParticles(long step);

	ParticleProfile _CreateParticle();
	void _GenerateParticleInstance();

	void _SetupDefaultProfile();
	void _SetupDefaultProfile(ParticleType type);

	std::deque<ParticleProfile> particles;
};

inline void Particle::SetProfilePosition(float x, float y, float z) {

	profile.position[0] = x;
	profile.position[1] = y;
	profile.position[2] = z;
}

inline void Particle::SetProfileSpeed(float x, float y, float z) {

	profile.speed[0] = x;
	profile.speed[1] = y;
	profile.speed[2] = z;
}