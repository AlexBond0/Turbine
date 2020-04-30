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

	// calculate light half plane
	Light* sun = dynamic_cast<Light*>(world.GetEntity("sun"));
	sun->CalculateHalfPlane(*world.GetActiveCamera()->GetLocalPosVec());

	// assign light handles
	rcontext.liveShader->SetVector("u_l_direction", sun->GetLightDirection());
	rcontext.liveShader->SetVector("u_l_halfplane", sun->halfplane);
	rcontext.liveShader->SetColor("u_l_ambient", sun->ambient.rgba);
	rcontext.liveShader->SetColor("u_l_diffuse", sun->diffuse.rgba);
	rcontext.liveShader->SetColor("u_l_specular", sun->specular.rgba);

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
	world.AddEntity(new Camera("Default Camera"));

	// set up the POV camera
	Camera* camPOV = new Camera("POV Camera");
	world.AddEntity(camPOV);
	camPOV->moveable = false;
	world.GetEntity("Seats")->AddChild(camPOV);
	camPOV->SetLocalPos(glm::vec3(0.0f, -0.083f, 0.0f));
	// camPOV->SetTarget(glm::vec3(0.039f, -0.1f, 1.0f));
	camPOV->SetTarget(glm::vec3(0.0f, -0.083f, 1.0f));
	camPOV->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	
	// create skybox
	Primitive* skybox = new Primitive();
	skybox->GenerateCube(20.0f, true);
	skybox->SetOrientation(0.0f, -90.0f, 0.0f);
	skybox->SetTexture(textures.id["skybox"]);
	skybox->useLight = false;
	skybox->SetName("skybox");
	world.AddEntity(skybox);

	// load the ground model
	Model3D* ground = Model3D::LoadModel("GroundPlane2.3dm");
	world.UnpackModel3D(ground);

	// load the bulb fly model
	Model3D* bulbfly = Model3D::LoadModel("bulbFly.obj");
	world.UnpackModel3D(bulbfly);
	world.GetObject3D("Body_Base")->SetTranslation(glm::vec3(1.0, 1.0, 0.0));

	//world.GetObject3D("Lightbulb")->isTransparent = true;

	// set property information for the ground plane
	Object3D* plane = world.GetObject3D("Plane");
	plane->SetScale(30.0f, 30.0f, 30.0f);
	plane->SetAmbient(0.3f, 0.3f, 0.3f, 1.0f);
	plane->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	plane->SetTexture(textures.id["ground2"]);

	// generate trees
	_GenerateTrees();

	// create sun light
	Light* sun = new Light("sun");
	sun->CreateSun();
	sun->SetTranslation(0.0f, 2.0f, 0.0f);
	world.AddEntity(sun);

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
	Model3D* ride = Model3D::LoadModel("uv-spinner.3dm");
	world.UnpackModel3D(ride);

	// Base
	world.GetObject3D("Base")->SetTexture(textures.id["wood-base"]);				// set texture
	world.GetObject3D("Base")->SetOrientation(0.0f, 0.0f, 3.3f);					// adjust angle
	world.GetObject3D("Base")->AddChild(world.GetObject3D("Platter"));				// add child

	// Platter
	world.GetObject3D("Platter")->SetTexture(textures.id["diamond-metal"]);			// set texture
	world.GetObject3D("Platter")->AddChild(world.GetObject3D("Pivot"));				// add child

	// Pivot
	world.GetObject3D("Pivot")->SetTexture(textures.id["singed-white-metal"]);		// set texture
	world.GetObject3D("Pivot")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);				// add specular material information
	world.GetObject3D("Pivot")->SetSpecLevel(20.0f);								// add specular level
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce"));				// add the left dry ice smoke effect
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("DryIce2"));				// add the left dry ice smoke effect
	world.GetObject3D("Pivot")->AddChild(world.GetObject3D("Arm"));					// add child

	// Arm
	world.GetObject3D("Arm")->SetTexture(textures.id["singed-white-metal"]);		// set texture
	world.GetObject3D("Arm")->SetSpecular(0.6f, 0.6f, 0.6f, 1.0f);					// add specular material information
	world.GetObject3D("Arm")->SetSpecLevel(20.0f);									// add specular level
	world.GetObject3D("Arm")->AddChild(world.GetObject3D("Spinner"));				// add child

	// Spinner
	world.GetObject3D("Spinner")->SetTexture(textures.id["hot-metal"]);				// set texture
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("Seats"));				// add child
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("Fire"));				// add the fire effect
	world.GetObject3D("Spinner")->AddChild(world.GetObject3D("FireSmoke"));			// add the fire's smoke effect

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
	world.AddEntity(trees);

	// get vert data
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

	// remove 
	free(treeInstances);
}

// generate copied seats for the ride
void Scene::_GenerateSeats() {

	// these seat copies could be instanced but
	// this shows the object copying capabillities

	Object3D* objRef;
	Object3D* spinner = world.GetObject3D("Spinner");

	objRef = world.DuplicateObject3D("Seats", "seats-1");
	objRef->SetTranslation(-0.16f, 0.0f, 0.095f);
	objRef->SetOrientation(0.0f, 60.0f, 0.0f);
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