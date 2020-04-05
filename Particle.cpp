#include "Particle.h"

Particle::Particle(int max, int perms) {

	_SetupDefaultProfile();

	_SetupParticle(max, perms);
}

Particle::Particle(ParticleType type, int max, int perms) {

	_SetupDefaultProfile(type);

	_SetupParticle(max, perms);
}

Particle::Particle(ParticleProfile profile, int max, int perms) {

	this->profile = profile;

	_SetupParticle(max, perms);
}

// setup the particle class
void Particle::_SetupParticle(int max, int perms) {
	
	maxParticles = max;
	particlesPerms = perms;
	isBillboarded = true;

	srand(time(NULL));

	_GenerateParticleInstance();

	SetAmbient(
		profile.colour[0], 
		profile.colour[1], 
		profile.colour[2],
		profile.colour[3]
	);

	// create memory space for tree instances
	int particleDataSize = sizeof(float) * 3 * max;
	rawParticleData = (float*)malloc(particleDataSize);
	noofinstances = max;
}

Particle::~Particle() {

	free(rawParticleData);
}



// =================================================================================

// overriden draw method from Object3D to take transparency into account
void Particle::Draw(RenderingContext rcontext) {

	modelView = rcontext.GetCurrentModelMatrix();

	// as we want to have transparent textures, this
	// gets deferred to after all opaque objects have rendered
	if (!isTransparent)
		InstancedObject::Draw(rcontext);
}

// blend-sepciffic draw function called after all other opaque draw methods
void Particle::BlendDraw(RenderingContext rcontext) {

	if (isTransparent) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_DST_COLOR
		InstancedObject::Draw(rcontext);
		glDisable(GL_BLEND);
	}
}

// overriden handle info method from Object3D to get matrix info & set billboard data
void Particle::_AssignHandleInformation(RenderingContext& rcontext) {

	// calculate the inverted view matrix
	MyMatrix::InvertMat4(rcontext.viewmatrix, inverseView);

	// get up dimention from the inverseView
	billboard_u[0] = inverseView[4];
	billboard_u[1] = inverseView[5];
	billboard_u[2] = inverseView[6];

	// get right dimention from the inverseView
	billboard_r[0] = inverseView[0];
	billboard_r[1] = inverseView[1];
	billboard_r[2] = inverseView[2];

	// assign billboard information
	glUniform3fv(rcontext.billboardhandles[0], 1, billboard_u);
	glUniform3fv(rcontext.billboardhandles[1], 1, billboard_r);

	// continue handle assignment
	InstancedObject::_AssignHandleInformation(rcontext);
}


// =================================================================================

// update the animation of the particles
void Particle::Update(long step) {

	time_ms += step;

	_CleanOldParticles();

	if (active)
		_GenerateNewParticles(step);

	_UpdateParticles(step);
}

// remove dead particles from the queue
void Particle::_CleanOldParticles() {

	bool stillAlive = false;

	while (!stillAlive && particles.size() > 0) {

		// pop old particles off of the particles queue
		if (particles.front().life < time_ms)
			particles.pop_front();

		// found forst alive particles, all others are also alive
		else
			stillAlive = true;
		
	}
}

// create new particles according to the ParticleProfile
void Particle::_GenerateNewParticles(long step) {

	// get particles to be rendered 
	int newParticles = ((long)particlesPerms / step);

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
	MyMatrix::MultiplyVecByMatrix(newParticle.speed, modelView);

	// position the new particle correctly
	MyMatrix::MultiplyVecByMatrix(newParticle.position, modelView);

	// assign life offset
	newParticle.life += time_ms;

	return newParticle;
}

// update the current alive particles in the particle queue
void Particle::_UpdateParticles(long step) {

	ParticleProfile* p;
	vertex v;
	std::vector<vertex> points;

	float camera[3] = {
		inverseView[12],
		inverseView[13],
		inverseView[14]
	};

	float timeStep = (float)step / 10000;

	// get all 
	for (int particleID = 0; particleID < particles.size(); particleID++) {

		p = &particles[particleID];

		// do gravity physics
		p->speed[1] += -9.81f * timeStep * p->weight;

		p->position[0] += p->speed[0] * timeStep;
		p->position[1] += p->speed[1] * timeStep;
		p->position[2] += p->speed[2] * timeStep;

		// save particle to a vertex struct
		v.x = p->position[0];
		v.y = p->position[1];
		v.z = p->position[2];
		v.dist = MyMatrix::distance(camera, p->position);

		// save point
		points.push_back(v);
	}

	// sort the points so drawing happens furthest first
	std::sort(points.begin(), points.end());

	// get all 
	for (int particleID = 0; particleID < particles.size(); particleID++) {

		// save particle to raw data
		rawParticleData[(particleID * 3)]		= points[particleID].x;
		rawParticleData[(particleID * 3) + 1]	= points[particleID].y;
		rawParticleData[(particleID * 3) + 2]	= points[particleID].z;
	}

	// set instance data
	SetInstanceData(rawParticleData, particles.size());
}



// =================================================================================

// create the default ParticleProfile
void Particle::_SetupDefaultProfile() {

	profile.position[0] = 0.0f;
	profile.position[1] = 0.0f;
	profile.position[2] = 0.0f;

	profile.speed[0] = 1.0f;
	profile.speed[1] = 1.0f;
	profile.speed[2] = 1.0f;

	profile.colour[0] = 1.0f;
	profile.colour[1] = 1.0f;
	profile.colour[2] = 1.0f;
	profile.colour[3] = 0.2f;

	profile.weight = 1.0f;
	profile.life = 2000;
	profile.spread = 6;
}

// create a ParticleProfile depending on a given ParticleType
void Particle::_SetupDefaultProfile(ParticleType type) {

	switch (type) {

	case ParticleType::SMOKE_WHITE: {

		_SetupDefaultProfile();

		profile.weight = -1.0f;
		profile.life = 2000;
		profile.spread = 80;

		useLight = false;
		isTransparent = true;
		break;
	}

	case ParticleType::SMOKE_BLACK: {

		_SetupDefaultProfile();

		profile.colour[0] = 0.0f;
		profile.colour[1] = 0.0f;
		profile.colour[2] = 0.0f;
		profile.colour[3] = 0.2f;

		profile.weight = -0.6f;
		profile.life = 2500;
		profile.spread = 100;

		useLight = false;
		isTransparent = true;
		break;
	}

	case ParticleType::FIRE: {

		_SetupDefaultProfile();

		profile.colour[0] = 1.0f;
		profile.colour[1] = 0.25f;
		profile.colour[2] = 0.0f;
		profile.colour[3] = 0.4f;

		profile.weight = -3.0f;
		profile.life = 700;
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
	SetTriangles(*polys, 6);

	SetDiffuse(0.0f, 0.0f, 0.0f, 0.0f);
}