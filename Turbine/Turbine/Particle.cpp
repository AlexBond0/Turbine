#include "Particle.h"

Particle::Particle(std::string name, int max, int perms)
	: InstancedObject(name)
{

	_SetupDefaultProfile();

	_SetupParticle(max, perms);
}

Particle::Particle(std::string name, ParticleType type, int max, int perms)
	: InstancedObject(name) 
{

	_SetupDefaultProfile(type);

	_SetupParticle(max, perms);
}

Particle::Particle(std::string name, ParticleProfile profile, int max, int perms)
	: InstancedObject(name) 
{

	this->profile = profile;

	_SetupParticle(max, perms);
}

// setup the particle class
void Particle::_SetupParticle(int max, int perms) {

	SetEntityType(EntityType::OBJ_PARTICLE_SYS);
	
	maxParticles = max;
	particlesPerms = perms;
	isBillboarded = true;

	// don't need to save instance data when saved
	serializeInstanceData = false;

	instances.Reserve(maxParticles);

	srand(time(NULL));

	_GenerateParticleInstance();

	SetAmbient(profile.colour);

	// create memory space for tree instances
	// int particleDataSize = sizeof(float) * 3 * max;
	// rawParticleData = (float*)malloc(particleDataSize);
	// noofinstances = max;
}

Particle::~Particle() {

	// free(rawParticleData);
}

// =================================================================================

// save the modelview for particle maths before proceeding with Object3D Draw method
void Particle::Draw(RenderingContext& rcontext) {

	// savea copy of the current modelview for particle maths
	modelView = glm::mat4(*rcontext.GetCurrentModelMatrix());

	Object3D::Draw(rcontext);
}

// overriden handle info method from Object3D to get matrix info & set billboard data
void Particle::_AssignHandleInformation(RenderingContext& rcontext) {

	// calculate the inverted view matrix
	inverseView = glm::inverse(rcontext.viewMatrix);

	// get up dimention from the inverseView
	billboard_u = glm::vec3(
		inverseView[1][0],
		inverseView[1][1],
		inverseView[1][2]
	);

	// get right dimention from the inverseView
	billboard_r = glm::vec3(
		inverseView[0][0],
		inverseView[0][1],
		inverseView[0][2]
	);

	// assign billboard information
	rcontext.liveShader->SetVector("a_b_up", billboard_u);
	rcontext.liveShader->SetVector("a_b_right", billboard_r);

	// continue handle assignment
	InstancedObject::_AssignHandleInformation(rcontext);
}

json Particle::Serialize() {

	json me = Object3D::Serialize();

	Serialize::SerializeVec4(me, "camPosition", profile.camPosition);
	Serialize::SerializeVec4(me, "speed", profile.speed);
	me["colour"] = profile.colour.rgba;
	me["weight"] = profile.weight;
	me["spread"] = profile.spread;
	me["life"] = profile.life;

	// pack and send json
	json ret;
	ret["Particle"] = me;
	return ret;
}

void Particle::Deserialize(json& data) {

}

// =================================================================================

// update the animation of the particles
void Particle::Update(double step_seconds) {

	time_ms += step_seconds;

	_CleanOldParticles();

	if (active)
		_GenerateNewParticles(step_seconds);

	_UpdateParticles(step_seconds);
}

// remove dead particles from the queue
void Particle::_CleanOldParticles() {

	bool stillAlive = false;

	while (!stillAlive && particles.size() > 0) {

		// pop old particles off of the particles queue
		if (particles.front().life < time_ms)
			particles.pop_front();

		// found first alive particles, all others are also alive
		else
			stillAlive = true;
		
	}
}

// create new particles according to the ParticleProfile
void Particle::_GenerateNewParticles(double step_seconds) {

	// get particles to be rendered 
	int newParticles = ((double)particlesPerms * step_seconds);

	// cap to particle limit
	if (particles.size() + newParticles > maxParticles)
		newParticles = maxParticles - particles.size();

	for (int newParticle = 0; newParticle < newParticles; newParticle++) {

		particles.push_back(_CreateParticle());
	}
}

// create an individual particle according to the ParticleProfile
ParticleProfile Particle::_CreateParticle() {

	ParticleProfile newParticle = profile;

	int halfSpread = profile.spread / 2;

	// slightly randomise particle projection
	newParticle.speed[0] += (float)((rand() % profile.spread) - halfSpread) / 100.0f;
	newParticle.speed[1] += (float)((rand() % profile.spread) - halfSpread) / 100.0f;
	newParticle.speed[2] += (float)((rand() % profile.spread) - halfSpread) / 100.0f;

	// rotate the direction of speed correctly
	newParticle.speed = modelView * newParticle.speed;

	// position the new particle correctlyl
	newParticle.camPosition = modelView * newParticle.camPosition;

	// assign life offset
	newParticle.life += time_ms;

	return newParticle;
}

// update the current alive particles in the particle queue
void Particle::_UpdateParticles(double step_seconds) {

	ParticleProfile* p;
	vertex v;
	std::vector<vertex> points;

	glm::vec4 camera = {
		inverseView[3][0],
		inverseView[3][1],
		inverseView[3][2],
		1
	};

	float timeStep = (float)step_seconds / 10;

	// get all 
	for (int particleID = 0; particleID < particles.size(); particleID++) {

		p = &particles[particleID];

		// do gravity physics
		p->speed.y += -9.81f * timeStep * p->weight;
		p->camPosition += p->speed * timeStep;

		// save particle to a vertex struct
		v.x = p->camPosition.x;
		v.y = p->camPosition.y;
		v.z = p->camPosition.z;
		v.dist = glm::distance(camera, p->camPosition);

		// save point
		points.push_back(v);
	}

	// sort the points so drawing happens furthest first
	std::sort(points.begin(), points.end());

	// get all 
	std::vector<Instance>* particleData = instances.GetVector();

	for (int particleID = 0; particleID < particles.size(); particleID++) {

		// save particle to raw data
		particleData->at(particleID).position.x = points[particleID].x;
		particleData->at(particleID).position.y = points[particleID].y;
		particleData->at(particleID).position.z = points[particleID].z;
	}

	UpdateInstanceCount(particles.size());
}



// =================================================================================

// create the default ParticleProfile
void Particle::_SetupDefaultProfile() {

	profile.camPosition = glm::vec4(0.0, 0.0, 0.0, 1.0);

	profile.speed = glm::vec4(1.0, 1.0, 1.0, 0.0);

	profile.colour = color4(1.0, 1.0, 1.0, 0.2);

	profile.weight = 1.0f;
	profile.life = 2;
	profile.spread = 6;
}

// create a ParticleProfile depending on a given ParticleType
void Particle::_SetupDefaultProfile(ParticleType type) {

	switch (type) {

	case ParticleType::SMOKE_WHITE: {

		_SetupDefaultProfile();

		profile.weight = -1.0f;
		profile.life = 2;
		profile.spread = 80;

		useLight = false;
		isTransparent = true;
		break;
	}

	case ParticleType::SMOKE_BLACK: {

		_SetupDefaultProfile();

		profile.colour = color4(0.0, 0.0, 0.0, 0.2);

		profile.weight = -0.6f;
		profile.life = 2.5;
		profile.spread = 100;

		useLight = false;
		isTransparent = true;
		break;
	}

	case ParticleType::FIRE: {

		_SetupDefaultProfile();

		profile.colour = color4(1.0, 0.25, 0.0, 0.4);

		profile.weight = -3.0f;
		profile.life = 0.7;
		profile.spread = 50;

		useLight = false;
		isTransparent = true;
		break;
	}

	default:
		_SetupDefaultProfile();
		break;
	}
}

// create and assign particle vertex/polygon data
void Particle::_GenerateParticleInstance() {

	float size = 0.007;

	float verts[4][8] = {
		{ size, size, 0, 1, 1, 0, 1.0f, 1.0f },
		{ -size, size, 0, -1, 1, 0, 0.0f, 1.0f },
		{ size, -size, 0, 1, -1, 0, 1.0f, 0.0f },
		{ -size, -size, 0, -1, -1, 0, 0.0f, 0.0f }
	};
	SetVertexData(*verts, 4, true);

	unsigned short polys[2][3] = {
		{ 0, 1, 2 },
		{ 3, 2, 1 }
	};
	SetTriangles(*polys, 2);

	SetDiffuse(0.0f, 0.0f, 0.0f, 0.0f);
}