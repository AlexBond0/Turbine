#include "Scene.h"


Scene::Scene() {

	sceneLoaded = false;
}


Scene::~Scene() {

}

// render the current scene
void Scene::Render(RenderingContext& rcontext) {

	// setup opengl environment with rcontext
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glUseProgram(rcontext.liveShader->ID);

	rcontext.InitModelMatrix(true);

	// calculate camera properties
	world.GetActiveCamera()->LookThrough(rcontext);

	glm::vec3 pos = world.GetActiveCamera()->GetWorldPosition();
	rcontext.liveShader->SetVector("u_c_position", pos);

	// setup environment 
	world.enviro.Render(rcontext);

	// assign light handles
	world.lights.RenderLights(rcontext, pos);

	// render solid objects
	_ObjectPass(rcontext);

	// render transparent objects
	_TransparencyPass(rcontext);

}

void Scene::_ShadowPass(RenderingContext& rcontext) {

}

// render objects in the scene
void Scene::_ObjectPass(RenderingContext& rcontext) {

	rcontext.renderPass = RenderPass::OBJECT;
	world.Render(rcontext);
}

// render transparent objects in the scene
void Scene::_TransparencyPass(RenderingContext& rcontext) {

	rcontext.renderPass = RenderPass::BLEND;
	world.Render(rcontext);
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

	// create the default camera
	Camera* defaultCam = new Camera("Default Camera");
	world.AddEntity(defaultCam);
	defaultCam->fZFar = 1500.0f;
	defaultCam->fDirty = true; // needs calling as f properties have changed

	// set up the POV camera
	Camera* camPOV = new Camera("POV Camera");
	world.AddEntity(camPOV);
	camPOV->moveable = false;
	world.GetModelEntity("uv-spinner", "Seats")->AddChild(camPOV);

	// set these after parenting as parenting would offsett these
	camPOV->SetLocalPos(glm::vec3(0.0f, -0.083f, 0.0f));
	camPOV->SetTarget(glm::vec3(0.0f, -0.083f, 1.0f));
	camPOV->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	
	// create skybox
	Primitive* skybox = new Primitive();
	skybox->GenerateBox(20.0f, true);
	skybox->SetOrientation(0.0f, -90.0f, 0.0f);
	skybox->SetScale(40.0f);
	skybox->SetTexture(textures.id["skybox"]);
	skybox->useLight = false;
	skybox->SetName("skybox");
	world.AddEntity(skybox);

	// load the ground model
	_LoadBigScene();
	// _LoadSmallScene();

	// load the bulb fly model
	ModelLoader* bulbfly = ModelLoader::LoadModel("bulbFly.obj");
	world.AddEntity(bulbfly->GetModel());
	world.GetModelObject3D("bulbFly", "Body_Base")->SetTranslation(glm::vec3(1.0, 1.0, 0.0));
	dynamic_cast<Model*>(world.GetEntity("bulbFly"))->Clean();

	// generate lights
	_GenerateLights();

	// load animator
	animator = RideAnimation(&world);

	// clean the world from processing and loading objects into the scene
	world.Clean();

	sceneLoaded = true;
}

// handle the event timer firing
void Scene::OnTimer(RenderingContext& rcontext, double timePassed) {

	if (sceneLoaded) {

		animator.Animate(timePassed);

		world.UpdateParticles(timePassed);
	}
}

// load the ride model and setup the appropriate loaded data
void Scene::_LoadRide() {

	// load the ride and save it in the world
	ModelLoader* spinner = ModelLoader::LoadModel("uv-spinner.3dm");
	world.AddEntity(spinner->GetModel());

	Model* ride = dynamic_cast<Model*>(world.GetEntity("uv-spinner"));

	// Base
	ride->GetObject3D("Base")->SetTexture(textures.id["wood-base"]);				// set texture
	ride->GetObject3D("Base")->SetOrientation(0.0f, 0.0f, 3.3f);					// adjust angle
	ride->GetObject3D("Base")->AddChild(ride->GetObject3D("Platter"));				// add child

	// Platter
	ride->GetObject3D("Platter")->SetTexture(textures.id["diamond-metal"]);			// set texture
	ride->GetObject3D("Platter")->AddChild(ride->GetObject3D("Pivot"));				// add child

	// Pivot
	ride->GetObject3D("Pivot")->SetTexture(textures.id["singed-white-metal"]);		// set texture
	ride->GetObject3D("Pivot")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);				// add specular material information
	ride->GetObject3D("Pivot")->SetSpecLevel(20.0f);								// add specular level
	ride->GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce"));				// add the left dry ice smoke effect
	ride->GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce2"));				// add the left dry ice smoke effect
	ride->GetObject3D("Pivot")->AddChild(ride->GetObject3D("Arm"));					// add child

	// Arm
	ride->GetObject3D("Arm")->SetTexture(textures.id["singed-white-metal"]);		// set texture
	ride->GetObject3D("Arm")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	ride->GetObject3D("Arm")->SetSpecLevel(20.0f);									// add specular level
	ride->GetObject3D("Arm")->AddChild(ride->GetObject3D("Spinner"));				// add child

	// Spinner
	ride->GetObject3D("Spinner")->SetTexture(textures.id["hot-metal"]);				// set texture
	ride->GetObject3D("Spinner")->AddChild(ride->GetObject3D("Seats"));				// add child
	ride->GetObject3D("Spinner")->AddChild(world.GetObject3D("Fire"));				// add the fire effect
	ride->GetObject3D("Spinner")->AddChild(world.GetObject3D("FireSmoke"));			// add the fire's smoke effect

	// Seats
	ride->GetObject3D("Seats")->SetTexture(textures.id["seats"]);						// set texture
	_GenerateSeats();														// create the duplicate seat information

	ride->Clean();
}

// generate tree primitives for the scene
void Scene::_GenerateTrees() {

	int treeDividor = 1;

	// create primitive tree
	Primitive tree;
	tree.GenerateTree(0.13f, 0.7f);
	tree.SetTexture(textures.id["tree-wrap"]);

	// create instance object
	InstancedObject* trees = new InstancedObject(&tree, "trees");
	world.AddEntity(trees);

	// get vert data
	Object3D* plane = world.GetModelObject3D("island", "Plane");
	int noofTrees = plane->GetVertCount();
	std::vector<Instance> treeInstances;
	glm::vec4 point;

	// collate instance data
	noofTrees /= treeDividor;

	glm::mat4 translation = *plane->GetWorldTranslation().GetCurrentModelMatrix();

	srand(time(NULL));
	for (int treeID = 0; treeID < noofTrees; treeID++) {

		point = translation * glm::vec4(plane->vertices.GetPointUV(treeID * treeDividor)->vertex, 1.0f);

		Instance i;
		i.position[0] = point.x + (float(rand() % 20) / 40.0f);
		i.position[1] = point.y - (float(rand() % 20) / 100.0f); // -(float(rand() % 20) / 100.0f);
		i.position[2] = point.z + (float(rand() % 20) / 40.0f);

		treeInstances.push_back(i);
	}

	// assign the new instance data
	trees->instances.SetData(treeInstances);
	trees->UpdateInstanceCount();
}

// generate copied seats for the ride
void Scene::_GenerateSeats() {

	// these seat copies could be instanced but
	// this shows the object copying capabillities

	Object3D* objRef;
	Object3D* spinner = world.GetModelObject3D("uv-spinner", "Spinner");
	Object3D* seats = world.GetModelObject3D("uv-spinner", "Seats");
	Model* ride = dynamic_cast<Model*>(world.GetEntity("uv-spinner"));

	objRef = world.DuplicateObject3D(seats, "seats-1", false);
	ride->AddEntity(objRef);

	objRef->SetTranslation(-0.16f, 0.0f, 0.095f);
	objRef->SetOrientation(0.0f, 60.0f, 0.0f);
	spinner->AddChild(objRef);


	objRef = world.DuplicateObject3D(seats, "seats-2", false);
	ride->AddEntity(objRef);

	objRef->SetTranslation(-0.16f, 0.0f, 0.280f);
	objRef->SetOrientation(0.0f, 120.0f, 0.0f);
	spinner->AddChild(objRef);


	objRef = world.DuplicateObject3D(seats, "seats-3", false);
	ride->AddEntity(objRef);

	objRef->SetTranslation(0.0f, 0.0f, 0.372f);
	objRef->SetOrientation(0.0f, 180.0f, 0.0f);
	spinner->AddChild(objRef);


	objRef = world.DuplicateObject3D(seats, "seats-4", false);
	ride->AddEntity(objRef);

	objRef->SetTranslation(0.16f, 0.0f, 0.095f);
	objRef->SetOrientation(0.0f, -60.0f, 0.0f);
	spinner->AddChild(objRef);


	objRef = world.DuplicateObject3D(seats, "seats-5", false);
	ride->AddEntity(objRef);

	objRef->SetTranslation(0.16f, 0.0f, 0.280f);
	objRef->SetOrientation(0.0f, -120.0f, 0.0f);
	spinner->AddChild(objRef);
}

// generate the particle effect objects
void Scene::_GenerateParticles() {

	int particleRate = 200;

	// create left dry ice smoke effect
	Particle* dryIce = new Particle("DryIce", ParticleType::SMOKE_WHITE, 10000, particleRate);
	dryIce->SetProfilePosition(0.04f, 0.04f, 0.08f);
	dryIce->SetProfileSpeed(1.0f, -1.5f, 4.0f);
	world.AddEntity(dryIce);

	// create right dry ice smoke effect
	Particle* dryIce2 = new Particle("DryIce2", ParticleType::SMOKE_WHITE, 10000, particleRate);
	dryIce2->SetProfilePosition(-0.04f, 0.04f, 0.08f);
	dryIce2->SetProfileSpeed(-1.0f, -1.5f, 4.0f);
	world.AddEntity(dryIce2);

	// create fire effect
	Particle* fire = new Particle("Fire", ParticleType::FIRE, 10000, particleRate);
	fire->SetProfilePosition(0.0f, 0.0f, 0.0f);
	fire->SetProfileSpeed(0.0f, -3.0f, 0.0f);
	world.AddEntity(fire);

	// create dark fire smoke effect
	Particle* firesmoke = new Particle("FireSmoke", ParticleType::SMOKE_BLACK, 10000, particleRate);
	firesmoke->SetProfilePosition(0.0f, 0.0f, 0.0f);
	firesmoke->SetProfileSpeed(0.0f, -1.0f, 0.0f);
	world.AddEntity(firesmoke);
}

void Scene::_GenerateLights() {

	Light* light;

	// create sun light
	light = new DirectionalLight("sun");
	light->CreateSun();
	light->SetLocalPos(0.0f, 0.6f, 0.0f);
	world.AddEntity(light);
	world.lights.SetCurrentDirLight("sun");

	// create sun light
	light = new PointLight("light1");
	light->CreateSun();
	light->diffuse = color4(0.0, 1.0, 0.0, 1.0);
	world.GetModelEntity("uv-spinner", "seats-5")->AddChild(light);
	world.AddEntity(light);

	// create sun light
	light = new PointLight("light2");
	light->CreateSun();
	light->diffuse = color4(0.0, 0.0, 1.0, 1.0);
	world.GetModelEntity("uv-spinner", "Spinner")->AddChild(light);
	world.AddEntity(light);

	// create sun light
	light = new PointLight("light3");
	light->CreateSun();
	light->diffuse = color4(1.0, 0.0, 0.0, 1.0);
	world.GetModelEntity("uv-spinner", "Pivot")->AddChild(light);
	world.AddEntity(light);

	// create spotlight
	light = new SpotLight("spotlight");
	light->CreateSun();
	light->diffuse = color4(1.0, 0.0, 1.0, 1.0);
	world.AddEntity(light);
}

void Scene::_LoadBigScene() {

	// load the ground model
	ModelLoader* ground = ModelLoader::LoadModel("island.obj"); //"GroundPlane2.3dm");
	world.AddEntity(ground->GetModel());
	ground->GetModel()->Clean();

	Model* island = ground->GetModel();
	island->SetScale(1.0, 1.0, 1.0);
	island->SetLocalPos(-25.3, -10.0, 19.0);
	// island->SetScale(30.0f);

	Object3D* plane = world.GetModelObject3D("island", "Plane");
	plane->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	plane->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	plane->SetTexture(textures.id["ground2"]);
	plane->vertices.ScaleUV(200.0f);

	// make some water
	Primitive* ocean = new Primitive();
	ocean->SetName("Ocean");
	Geomitory water = Primitive::GeneratePlane(5.0, 5.0, true);
	ocean->AssignGeomitoryData(water);
	ocean->SetLocalPos(0.0, -10.0, 0.0);
	ocean->SetScale(100.0, 100.0, 100.0);
	ocean->SetAmbient(0.0f, 0.3f, 1.0f, 1.0f);
	ocean->SetDiffuse(0.0f, 0.6f, 1.0f, 1.0f);
	world.AddEntity(ocean);

	// generate trees
	_GenerateTrees();
}

void Scene::_LoadSmallScene() {

	// load the ground model
	ModelLoader* ground = ModelLoader::LoadModel("GroundPlane2.3dm"); //"GroundPlane2.3dm");
	world.AddEntity(ground->GetModel());
	ground->GetModel()->Clean();

	Model* island = ground->GetModel();
	island->SetScale(30.0f);

	Object3D* plane = world.GetModelObject3D("GroundPlane2", "Plane");
	plane->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	plane->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	plane->SetTexture(textures.id["ground2"]);
}
