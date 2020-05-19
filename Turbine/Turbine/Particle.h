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

	glm::vec4 camPosition;	// current particle position
	glm::vec4 speed;		// current particle speed vector

	color4 colour;		// particle colour
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

// An InstancedObject with billboarding and a custom profile to crudely simulate
// particles
class Particle
	: public InstancedObject
{
public:

	bool active;				// is the particle generator active
	ParticleProfile profile;	// the particle generation profile

	Particle(std::string name, int max, int perms);
	Particle(std::string name, ParticleType type, int max, int perms);
	Particle(std::string name, ParticleProfile profile, int max, int perms);

	~Particle();

	void SetProfilePosition(float x, float y, float z);
	void SetProfileSpeed(float x, float y, float z);

	void Update(double step);

	virtual json Serialize();
	Particle(json& data); // Deserialize

protected:

	void Draw(RenderingContext& rcontext);

	int maxParticles;		// maximum particles in generator at one time
	int particlesPerms;		// particles generated per millisecond

	virtual void _AssignHandleInformation(RenderingContext& rcontext);

private:

	double time_ms;			// current animation time

	glm::vec3 billboard_u;	// billboard up vector
	glm::vec3 billboard_r;	// billboard right vector

	glm::mat4 inverseView;	// inverse view matrix
	glm::mat4 modelView;	// model view matrix

	void _SetupParticle(int max, int perms);

	void _CleanOldParticles();
	void _GenerateNewParticles(double step_seconds);
	void _UpdateParticles(double step_seconds);

	ParticleProfile _CreateParticle();
	void _GenerateParticleInstance();

	void _SetupDefaultProfile();
	void _SetupDefaultProfile(ParticleType type);

	std::deque<ParticleProfile> particles;
};

inline void Particle::SetProfilePosition(float x, float y, float z) {

	profile.camPosition[0] = x;
	profile.camPosition[1] = y;
	profile.camPosition[2] = z;
}

inline void Particle::SetProfileSpeed(float x, float y, float z) {

	profile.speed[0] = x;
	profile.speed[1] = y;
	profile.speed[2] = z;
}