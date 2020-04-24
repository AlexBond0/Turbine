#include "Scene.h"


Scene::Scene() {

	sceneLoaded = false;

	camera = new Camera("Default Camera");
	camPOV = new Camera("POV Camera");
}


Scene::~Scene() {

	// delete all saved objects in the hashmap
	//for (auto const& objRef : objects) 
	//	delete objRef.second;
	
	// delete all saved models in the hashmap
	for (auto const& modelRef : models)
		delete modelRef.second;

	delete camera;
	delete camPOV;
}

// render the current scene
void Scene::Render(RenderingContext& rcontext) {

	// setup opengl environment with rcontext
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glUseProgram(rcontext.shaders["object"]->ID);

	rcontext.InitModelMatrix(true);

	// calculate camera properties
	if (pov) 
		camPOV->LookThrough(rcontext);
	
	else 
		camera->LookThrough(rcontext);
	

	light.CalculateHalfPlane(camera->camPosition);

	// assign light handles
	rcontext.shaders["object"]->SetVector("u_l_direction", *light.GetDirection());
	rcontext.shaders["object"]->SetVector("u_l_halfplane", *light.GetHalfplane());
	rcontext.shaders["object"]->SetColor("u_l_ambient", light.ambient.rgba);
	rcontext.shaders["object"]->SetColor("u_l_diffuse", light.diffuse.rgba);
	rcontext.shaders["object"]->SetColor("u_l_specular", light.specular.rgba);

	_ObjectPass(rcontext);

	_TransparencyPass(rcontext);

}

void Scene::_ShadowPass(RenderingContext& rcontext) {


}

void Scene::_ObjectPass(RenderingContext& rcontext) {

	// draw full models via their root object
	//for (Model3D* model : modelsToDraw)
	//	model->Draw(rcontext);

	// draw individual objects
	//for (Object3D* obj : objectsToDraw)
	//	obj->Draw(rcontext);

	world.Render(rcontext);
}

void Scene::_TransparencyPass(RenderingContext& rcontext) {

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

	// set up the POV camera
	camPOV->SetPosition(glm::vec3(0.0f, -0.083f, 0.0f));
	camPOV->SetTarget(glm::vec3(0.039f, -0.1f, 1.0f));
	camPOV->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	// objects["Seats"]->AddChild(camPOV);
	world.GetEntity("Seats")->AddChild(camPOV);
	
	// create skybox
	Primitive* skybox = new Primitive();
	skybox->GenerateCube(20.0f, true);
	skybox->SetOrientation(0.0f, -90.0f, 0.0f);
	skybox->SetTexture(textures.id["skybox"]);
	skybox->useLight = false;

	// objects["skybox"] = skybox;
	// objectsToDraw.push_back(objects["skybox"]);
	world.AddEntity(skybox);

	// load the ground model
	models["ground"] = Model3D::LoadModel(L"GroundPlane2.3dm");
	_UnpackObjects(models["ground"]);
	modelsToDraw.push_back(models["ground"]);

	// set property information for the ground plane
	//objects["Plane"]->SetScale(30.0f, 30.0f, 30.0f);
	//objects["Plane"]->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	//objects["Plane"]->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	//objects["Plane"]->SetTexture(textures.id["ground2"]);

	Object3D* plane = static_cast<Object3D*>(world.GetEntity("Plane"));
	plane->SetScale(30.0f, 30.0f, 30.0f);
	plane->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	plane->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	plane->SetTexture(textures.id["ground2"]);


	// generate trees
	_GenerateTrees();

	// create sun light
	light.CreateSun();

	// load animator
	animator = RideAnimation(&world);

	world.Update();
	sceneLoaded = true;
}

// handle the event timer firing
void Scene::OnTimer(RenderingContext& rcontext, double timePassed) {

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
	models["ride"]->rootObject = world.GetObject3D("Base");					// assign the root object to draw from
	modelsToDraw.push_back(models["ride"]);									// save the model

	// Base
	world.GetObject3D("Base")->SetTexture(textures.id["wood-base"]);					// set texture
	world.GetObject3D("Base")->SetOrientation(0.0f, 0.0f, 3.3f);						// adjust angle
	world.GetObject3D("Base")->AddChild(world.GetObject3D("Platter"));					// add child

	// Platter
	world.GetObject3D("Platter")->SetTexture(textures.id["diamond-metal"]);			// set texture
	world.GetObject3D("Platter")->AddChild(world.GetObject3D("Pivot"));				// add child

	// Pivot
	world.GetObject3D("Pivot")->SetTexture(textures.id["singed-white-metal"]);		// set texture
	world.GetObject3D("Pivot")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	world.GetObject3D("Pivot")->SetSpecLevel(20.0f);									// add specular level
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce"));							// add the left dry ice smoke effect
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce2"));							// add the left dry ice smoke effect
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("Arm"));								// add child

	// Arm
	world.GetObject3D("Arm")->SetTexture(textures.id["singed-white-metal"]);			// set texture
	world.GetObject3D("Arm")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	world.GetObject3D("Arm")->SetSpecLevel(20.0f);									// add specular level
	world.GetObject3D("Arm")->AddChild(world.GetObject3D("Spinner"));				// add child

	// Spinner
	world.GetObject3D("Spinner")->SetTexture(textures.id["hot-metal"]);				// set texture
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("Seats"));							// add child
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("Fire"));							// add the fire effect
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("FireSmoke"));						// add the fire's smoke effect

	// Seats
	world.GetObject3D("Seats")->SetTexture(textures.id["seats"]);						// set texture
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
	// objects["trees"] = trees;
	world.AddEntity(trees);

	// get vert data
	// Object3D* plane = objects["Plane"];
	Object3D* plane = world.GetObject3D("Plane");
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
	// objectsToDraw.push_back(trees);

	// remove 
	free(treeInstances);
}

// generate copied seats for the ride
void Scene::_GenerateSeats() {

	// these seat copies could be instanced but
	// this shows the object copying capabillities

	Object3D* objRef;
	// Object3D* originalSeats = world.GetObject3D("Seats");
	Object3D* spinner = world.GetObject3D("Spinner");

	// objRef = CreateObject(originalSeats, "seats-1");
	objRef = world.DuplicateObject3D("Seats", "seats-1");
	objRef->SetTranslation(-0.16f, 0.0f, 0.095f);
	objRef->SetOrientation(0.0f, 60.0f, 0.0f);
	// objects["Spinner"]->AddChild(objRef);
	spinner->AddChild(objRef);

	objRef = world.DuplicateObject3D("Seats", "seats-2");
	objRef->SetTranslation(-0.16f, 0.0f, 0.280f);
	objRef->SetOrientation(0.0f, 120.0f, 0.0f);
	spinner->AddChild(objRef);

	objRef = world.DuplicateObject3D("Seats", "seats-3");
	objRef->SetTranslation(0.0f, 0.0f, 0.372f);
	objRef->SetOrientation(0.0f, 180.0f, 0.0f);
	spinner->AddChild(objRef);

	objRef = world.DuplicateObject3D("Seats", "seats-4");
	objRef->SetTranslation(0.16f, 0.0f, 0.095f);
	objRef->SetOrientation(0.0f, -60.0f, 0.0f);
	spinner->AddChild(objRef);

	objRef = world.DuplicateObject3D("Seats", "seats-5");
	objRef->SetTranslation(0.16f, 0.0f, 0.280f);
	objRef->SetOrientation(0.0f, -120.0f, 0.0f);
	spinner->AddChild(objRef);
}

// generate the particle effect objects
void Scene::_GenerateParticles() {

	// create left dry ice smoke effect
	Particle* dryIce = new Particle("DryIce", ParticleType::SMOKE_WHITE, 10000, 800);
	dryIce->SetProfilePosition(0.04f, 0.04f, 0.08f);
	dryIce->SetProfileSpeed(1.0f, -1.5f, 4.0f);

	// objects["DryIce"] = dryIce;				// as we've inherated all Object3D properties, we can simply
	world.AddEntity(dryIce);
	particleSystems.push_back(dryIce);		// add particle generators to all existing systems

	// create right dry ice smoke effect
	Particle* dryIce2 = new Particle("DryIce2", ParticleType::SMOKE_WHITE, 10000, 800);
	dryIce2->SetProfilePosition(-0.04f, 0.04f, 0.08f);
	dryIce2->SetProfileSpeed(-1.0f, -1.5f, 4.0f);
	// objects["DryIce2"] = dryIce2;
	world.AddEntity(dryIce2);
	particleSystems.push_back(dryIce2);

	// create fire effect
	Particle* fire = new Particle("Fire", ParticleType::FIRE, 10000, 800);
	fire->SetProfilePosition(0.0f, 0.0f, 0.0f);
	fire->SetProfileSpeed(0.0f, -3.0f, 0.0f);
	// objects["Fire"] = fire;
	world.AddEntity(fire);
	particleSystems.push_back(fire);

	// create dark fire smoke effect
	Particle* firesmoke = new Particle("FireSmoke", ParticleType::SMOKE_BLACK, 10000, 800);
	firesmoke->SetProfilePosition(0.0f, 0.0f, 0.0f);
	firesmoke->SetProfileSpeed(0.0f, -1.0f, 0.0f);
	// objects["FireSmoke"] = firesmoke;
	world.AddEntity(firesmoke);
	particleSystems.push_back(firesmoke);
}

// get all named objects from a model and store them in the objects map
void Scene::_UnpackObjects(Model3D* model) {

	for (int i = 0; i < model->GetNoOfObjects(); i++) {

		// objects[model->GetObjects()[i]->GetName()] = model->GetObjects()[i];
		world.AddEntity(model->GetObjects()[i]);
	}
}