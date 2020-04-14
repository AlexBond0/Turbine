#include "Scene.h"


Scene::Scene() {

	sceneLoaded = false;
}


Scene::~Scene() {

	// delete all saved objects in the hashmap
	for (auto const& objRef : objects) 
		delete objRef.second;
	
	// delete all saved models in the hashmap
	for (auto const& modelRef : models)
		delete modelRef.second;
}

// render the current scene
void Scene::Render(RenderingContext rcontext) {

	// setup opengl environment with rcontext
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glUseProgram(rcontext.glprogram);

	rcontext.InitModelMatrix(true);

	// calculate camera properties
	if (pov) {

		camPOV.PositionCamera(rcontext, camera);
	}
	else {

		rcontext.viewmatrix = glm::lookAt(
			camera.position,
			camera.target,
			camera.up
		);
	}
	light.CalculateHalfPlane(camera.position);

	// assign light handles
	glUniform3fv(rcontext.lighthandles[0], 1, light.GetDirection());
	glUniform3fv(rcontext.lighthandles[1], 1, light.GetHalfplane());
	glUniform4fv(rcontext.lighthandles[2], 1, light.ambient.rgba);
	glUniform4fv(rcontext.lighthandles[3], 1, light.diffuse.rgba);
	glUniform4fv(rcontext.lighthandles[4], 1, light.specular.rgba);

	//// draw full models via their root object
	for (Model3D* model : modelsToDraw)
		model->Draw(rcontext);

	// draw individual objects
	for (Object3D* obj : objectsToDraw)
		obj->Draw(rcontext);

	// drawparticle objects (if transparent)
	for (Particle* particle : particleSystems)
		particle->BlendDraw(rcontext);

}

// setup the scene
void Scene::Setup() {

	// load textures
	textures.loadAllTextures();

	// generate particles
	// (before ride so we can attach them to the heirarchy)
	_GenerateParticles();

	// load the ride
	_LoadRide();

	// set up the POV camera heirarchy
	camPOV.objList.push_back(objects["Base"]);
	camPOV.objList.push_back(objects["Platter"]);
	camPOV.objList.push_back(objects["Pivot"]);
	camPOV.objList.push_back(objects["Arm"]);
	camPOV.objList.push_back(objects["Spinner"]);
	camPOV.objList.push_back(objects["Seats"]);

	// set the POV cam looking from where a rider would see
	camPOV.offset[0] = 0.0f;
	camPOV.offset[1] = -0.1f;
	camPOV.offset[2] = 0.0f;

	// create skybox
	Primitive* skybox = new Primitive();
	skybox->GenerateCube(20.0f, true);
	skybox->SetRotation(0.0f, -90.0f, 0.0f);
	skybox->SetTexture(textures.id["skybox"]);
	skybox->useLight = false;
	objects["skybox"] = skybox;
	objectsToDraw.push_back(objects["skybox"]);

	// load the ground model
	models["ground"] = Model3D::LoadModel(L"GroundPlane2.3dm");
	_UnpackObjects(models["ground"]);
	modelsToDraw.push_back(models["ground"]);

	// set property information for the ground plane
	objects["Plane"]->SetScale(30.0f, 30.0f, 30.0f);
	objects["Plane"]->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	objects["Plane"]->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	objects["Plane"]->SetTexture(textures.id["ground2"]);


	// generate trees
	_GenerateTrees();

	// create sun light
	light.CreateSun();

	// load animator
	animator = RideAnimation(&objects);

	sceneLoaded = true;
}

// handle the event timer firing
void Scene::OnTimer(RenderingContext rcontext, double timePassed) {

	if (sceneLoaded) {

		animator.Animate(timePassed);

		for (Particle* particle : particleSystems)
			particle->Update(timePassed);

		// Render(rcontext);
	}
}

// load the ride model and setup the appropriate loaded data
void Scene::_LoadRide() {

	// load the ride model
	models["ride"] = Model3D::LoadModel(L"uv-spinner.3dm");					// load all model information
	_UnpackObjects(models["ride"]);											// unpack object information from model
	models["ride"]->rootObject = objects["Base"];							// assign the root object to draw from
	modelsToDraw.push_back(models["ride"]);									// save the model

	// Base
	objects["Base"]->SetTexture(textures.id["wood-base"]);					// set texture
	objects["Base"]->SetRotation(0.0f, 0.0f, 3.3f);							// adjust angle
	objects["Base"]->AddChild(objects["Platter"]);							// add child

	// Platter
	objects["Platter"]->SetTexture(textures.id["diamond-metal"]);			// set texture
	objects["Platter"]->AddChild(objects["Pivot"]);							// add child

	// Pivot
	objects["Pivot"]->SetTexture(textures.id["singed-white-metal"]);		// set texture
	objects["Pivot"]->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	objects["Pivot"]->SetSpecLevel(20.0f);									// add specular level
	objects["Pivot"]->AddChild(objects["DryIce"]);							// add the left dry ice smoke effect
	objects["Pivot"]->AddChild(objects["DryIce2"]);							// add the left dry ice smoke effect
	objects["Pivot"]->AddChild(objects["Arm"]);								// add child

	// Arm
	objects["Arm"]->SetTexture(textures.id["singed-white-metal"]);			// set texture
	objects["Arm"]->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	objects["Arm"]->SetSpecLevel(20.0f);									// add specular level
	objects["Arm"]->AddChild(objects["Spinner"]);							// add child

	// Spinner
	objects["Spinner"]->SetTexture(textures.id["hot-metal"]);				// set texture
	objects["Spinner"]->AddChild(objects["Seats"]);							// add child
	objects["Spinner"]->AddChild(objects["Fire"]);							// add the fire effect
	objects["Spinner"]->AddChild(objects["FireSmoke"]);						// add the fire's smoke effect

	// Seats
	objects["Seats"]->SetTexture(textures.id["seats"]);						// set texture
	_GenerateSeats();														// create the duplicate seat information

}

// generate tree primitives for the scene
void Scene::_GenerateTrees() {

	// create primitive tree
	Primitive tree;
	tree.GenerateTree(0.13f, 0.7f);
	tree.SetTexture(textures.id["tree-wrap"]);

	// create instance object
	InstancedObject* trees = new InstancedObject(&tree, "trees");
	objects["trees"] = trees;

	// get vert data
	Object3D* plane = objects["Plane"];
	int size = plane->GetVertCount() * 8;
	float* verts = (float*)plane->GetVertData();

	// create memory space for tree instances
	int noofTrees = plane->GetVertCount();
	int t_ID = 0;
	int length = sizeof(float) * 3 * noofTrees;
	float* treeInstances = (float*)malloc(length);

	// collate instance data
	srand(time(NULL));
	float treeX, treeY, treeZ;
	for (int stride = 0; stride < size; stride += 8) {

		treeX = (verts[stride] * 30);
		treeY = (verts[stride + 1] * 30) - (float(rand() % 20) / 100.0f);
		treeZ = (verts[stride + 2] * 30);

		treeInstances[t_ID++] = treeX;
		treeInstances[t_ID++] = treeY;
		treeInstances[t_ID++] = treeZ;
	}

	// assign the new instance data
	trees->SetInstanceData(treeInstances, noofTrees);
	objectsToDraw.push_back(trees);

	// remove 
	free(treeInstances);
}

// generate copied seats for the ride
void Scene::_GenerateSeats() {

	// these seat copies could be instanced but
	// this shows the object copying capabillities

	Object3D* objRef;

	objRef = CreateObject(objects["Seats"], "seats-1");
	objRef->SetTranslation(-0.16f, 0.0f, 0.095f);
	objRef->SetRotation(0.0f, 60.0f, 0.0f);
	objects["Spinner"]->AddChild(objRef);

	objRef = CreateObject(objects["Seats"], "seats-2");
	objRef->SetTranslation(-0.16f, 0.0f, 0.280f);
	objRef->SetRotation(0.0f, 120.0f, 0.0f);
	objects["Spinner"]->AddChild(objRef);

	objRef = CreateObject(objects["Seats"], "seats-3");
	objRef->SetTranslation(0.0f, 0.0f, 0.372f);
	objRef->SetRotation(0.0f, 180.0f, 0.0f);
	objects["Spinner"]->AddChild(objRef);

	objRef = CreateObject(objects["Seats"], "seats-4");
	objRef->SetTranslation(0.16f, 0.0f, 0.095f);
	objRef->SetRotation(0.0f, -60.0f, 0.0f);
	objects["Spinner"]->AddChild(objRef);

	objRef = CreateObject(objects["Seats"], "seats-5");
	objRef->SetTranslation(0.16f, 0.0f, 0.280f);
	objRef->SetRotation(0.0f, -120.0f, 0.0f);
	objects["Spinner"]->AddChild(objRef);
}

// generate the particle effect objects
void Scene::_GenerateParticles() {

	// create left dry ice smoke effect
	Particle* dryIce = new Particle(ParticleType::SMOKE_WHITE, 10000, 800);
	dryIce->SetProfilePosition(0.04f, 0.04f, 0.08f);
	dryIce->SetProfileSpeed(1.0f, -1.5f, 4.0f);
	objects["DryIce"] = dryIce;				// as we've inherated all Object3D properties, we can simply
	particleSystems.push_back(dryIce);		// add particle generators to all existing systems

	// create right dry ice smoke effect
	Particle* dryIce2 = new Particle(ParticleType::SMOKE_WHITE, 10000, 800);
	dryIce2->SetProfilePosition(-0.04f, 0.04f, 0.08f);
	dryIce2->SetProfileSpeed(-1.0f, -1.5f, 4.0f);
	objects["DryIce2"] = dryIce2;
	particleSystems.push_back(dryIce2);

	// create fire effect
	Particle* fire = new Particle(ParticleType::FIRE, 10000, 800);
	fire->SetProfilePosition(0.0f, 0.0f, 0.0f);
	fire->SetProfileSpeed(0.0f, -3.0f, 0.0f);
	objects["Fire"] = fire;
	particleSystems.push_back(fire);

	// create dark fire smoke effect
	Particle* firesmoke = new Particle(ParticleType::SMOKE_BLACK, 10000, 800);
	firesmoke->SetProfilePosition(0.0f, 0.0f, 0.0f);
	firesmoke->SetProfileSpeed(0.0f, -1.0f, 0.0f);
	objects["FireSmoke"] = firesmoke;
	particleSystems.push_back(firesmoke);
}

// get all named objects from a model and store them in the objects map
void Scene::_UnpackObjects(Model3D* model) {

	for (int i = 0; i < model->GetNoOfObjects(); i++) {

		objects[model->GetObjects()[i]->GetName()] = model->GetObjects()[i];
	}
}