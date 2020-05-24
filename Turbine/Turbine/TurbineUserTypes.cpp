#include "TurbineUserTypes.h"

std::map<std::string, std::string> TurbineUsertypeDefiner::descriptions;
TurbineDictionary TurbineUsertypeDefiner::dictionary;


void TurbineUsertypeDefiner::BuildTestOnes(sol::state& luaState) {

	// test 1 class

	BUILD_NEW_USERTYPE_START(luaState, Thing1, "TestType class")
		sol::constructors<Thing1(), Thing1(int, int)>(),
		BUILD_USERTYPE_VARIABLE(Thing1, a),
		BUILD_USERTYPE_VARIABLE(Thing1, b),
		BUILD_USERTYPE_VARIABLE(Thing1, slammer)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, Thing1, combineab, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, Thing1, setupVec, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, Thing1, gimmeY, "TestType function");

	BUILD_OVERLOADED_METHOD_START(luaState, Thing1, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Thing1, gimmeX, int()),
		BUILD_OVERLOADED_METHOD_FORMAT(Thing1, gimmeX, int(int))
	BUILD_OVERLOADED_METHOD_END();


	// test 1 class

	BUILD_NEW_USERTYPE_START(luaState, Thing2, "TestType class")
		BUILD_USERTYPE_BASE_CLASS(Thing1),
		BUILD_USERTYPE_VARIABLE(Thing2, c),
		BUILD_USERTYPE_VARIABLE(Thing2, d)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, Thing2, combinecd, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, Thing2, gimmeY, "TestType function");

	BUILD_OVERLOADED_METHOD_START(luaState, Thing2, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Thing2, gimmeX, int()),
		BUILD_OVERLOADED_METHOD_FORMAT(Thing2, gimmeX, int(int))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::Define(sol::state& luaState) {

	auto turbine = luaState["Turbine"].get_or_create<sol::table>();

	_SetupGLM(luaState);

	_Structs(turbine);

	_MoveableOrientation(turbine);
	_MoveablePoint(turbine);
	_Moveable(turbine);

	_Material(turbine);

	_Entity(turbine);

	_ObjectData(turbine);
	_Object3D(turbine);

	// definitey needs better implementation
	if (dictionary.size() == 0)
		MakeDictionary();
}

void TurbineUsertypeDefiner::_Structs(sol::table& luaState) {

	// ==========================
	// color4

	BUILD_NEW_USERTYPE_START(luaState, color4, "Manages the orientation of an Entity")
		sol::constructors<color4(), color4(json), color4(float, float, float, float)>(),
		BUILD_USERTYPE_VARIABLE(color4, rgba)
	BUILD_NEW_USERTYPE_END();

	BUILD_USERTYPE_METHOD(luaState, color4, Scale, "Get the orientation of the Entity as a quat");

	BUILD_OVERLOADED_METHOD_START(luaState, color4, R, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(color4, R, void(float)),
		BUILD_OVERLOADED_METHOD_FORMAT(color4, R, float())
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, color4, G, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(color4, G, void(float)),
		BUILD_OVERLOADED_METHOD_FORMAT(color4, G, float())
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, color4, B, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(color4, B, void(float)),
		BUILD_OVERLOADED_METHOD_FORMAT(color4, B, float())
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, color4, A, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(color4, A, void(float)),
		BUILD_OVERLOADED_METHOD_FORMAT(color4, A, float())
	BUILD_OVERLOADED_METHOD_END();

	// ==========================
	// EntityType

	// luaState.new_usertype<EntityType>("EntityType");
	BUILD_NEW_ENUM_START(luaState, EntityType)
		BUILD_ENUM_VALUE(EntityType, OBJ),
		BUILD_ENUM_VALUE(EntityType, OBJ_PRIMITIVE),
		BUILD_ENUM_VALUE(EntityType, OBJ_INSTANCED),
		BUILD_ENUM_VALUE(EntityType, OBJ_PARTICLE_SYS),
		BUILD_ENUM_VALUE(EntityType, LIGHT),
		BUILD_ENUM_VALUE(EntityType, CAMERA),
		BUILD_ENUM_VALUE(EntityType, MODEL),
		BUILD_ENUM_VALUE(EntityType, EMPTY)
	BUILD_NEW_ENUM_END()

}

void TurbineUsertypeDefiner::_MoveableOrientation(sol::table& luaState) {

	// BUILD_NEW_USERTYPE(luaState, MoveableOrientation, "Manages the orientation of an Entity");

	BUILD_NEW_USERTYPE_START(luaState, MoveableOrientation, "Manages the orientation of an Entity")
		sol::constructors<MoveableOrientation(), MoveableOrientation(MoveableOrientation*)>(),
		BUILD_USERTYPE_VARIABLE(MoveableOrientation, useTarget)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, GetOrientationQuat, "Get the orientation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, GetRotationQuat, "Get the rotation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, SetTarget, "Set the target of the blah blah");
	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, GetTarget, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, SetFront, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, MoveableOrientation, GetFront, "TestType function");

	BUILD_OVERLOADED_METHOD_START(luaState, MoveableOrientation, SetOrientation, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetOrientation, void(byte*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetOrientation, void(float*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetOrientation, void(float, float, float, bool)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetOrientation, void(glm::quat))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, MoveableOrientation, SetRotation, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetRotation, void(float, float, float, bool)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, SetRotation, void(glm::quat))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, MoveableOrientation, ShiftRotation, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, ShiftRotation, void(float*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveableOrientation, ShiftRotation, void(float, float, float))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::_MoveablePoint(sol::table& luaState) {

	BUILD_NEW_USERTYPE_START(luaState, MoveablePoint, "Manages the orientation of an Entity")
		sol::constructors<MoveablePoint(), MoveablePoint(MoveablePoint*)>()
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, GetWorldPosVec, "Get the orientation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, GetLocalPosVec, "Get the rotation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, GetTranlationVec, "Set the target of the blah blah");
	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, SetUp, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, GetUpVec, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, MoveablePoint, GetScaleVec, "TestType function");

	BUILD_OVERLOADED_METHOD_START(luaState, MoveablePoint, SetWorldPos, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetWorldPos, void(byte*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetWorldPos, void(glm::vec3))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, MoveablePoint, SetLocalPos, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetLocalPos, void(byte*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetLocalPos, void(float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetLocalPos, void(glm::vec3))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, MoveablePoint, SetTranslation, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetTranslation, void(byte*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetTranslation, void(float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetTranslation, void(glm::vec3))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, MoveablePoint, SetScale, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetScale, void(byte*)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetScale, void(float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetScale, void(float)),
		BUILD_OVERLOADED_METHOD_FORMAT(MoveablePoint, SetScale, void(glm::vec3))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::_Moveable(sol::table& luaState) {

	BUILD_NEW_USERTYPE_START(luaState, Moveable, "Manages the orientation of an Entity")
		sol::constructors<Moveable(), Moveable(Moveable*)>(),
		sol::base_classes, sol::bases<MoveablePoint, MoveableOrientation>()
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, Moveable, PointAt, "Get the orientation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Moveable, LookAt, "Get the rotation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Moveable, LookAtTarget, "Set the target of the blah blah");
	BUILD_USERTYPE_METHOD(luaState, Moveable, Move, "TestType function");
}

void TurbineUsertypeDefiner::_Material(sol::table& luaState) {

	BUILD_NEW_USERTYPE_START(luaState, Material, "Manages the orientation of an Entity")
		sol::constructors<Material(), Material(Material*)>(),
		BUILD_USERTYPE_VARIABLE(Material, isHighlighted)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(luaState, Material, SetMaterial, "Get the orientation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Material, GetAmbient, "Get the rotation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Material, GetDiffuse, "Set the target of the blah blah");
	BUILD_USERTYPE_METHOD(luaState, Material, GetSpecular, "TestType function");
	BUILD_USERTYPE_METHOD(luaState, Material, SetGlossiness, "Get the orientation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Material, GetGlossiness, "Get the rotation of the Entity as a quat");
	BUILD_USERTYPE_METHOD(luaState, Material, SetSpecLevel, "Set the target of the blah blah");
	BUILD_USERTYPE_METHOD(luaState, Material, GetSpecLevel, "TestType function");

	BUILD_OVERLOADED_METHOD_START(luaState, Material, SetAmbient, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetAmbient, void(float, float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetAmbient, void(color4)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetAmbient, void(byte*))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, Material, SetDiffuse, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetDiffuse, void(float, float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetDiffuse, void(color4)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetDiffuse, void(byte*))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_OVERLOADED_METHOD_START(luaState, Material, SetSpecular, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetSpecular, void(float, float, float, float)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetSpecular, void(color4)),
		BUILD_OVERLOADED_METHOD_FORMAT(Material, SetSpecular, void(byte*))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::_Entity(sol::table& luaState) {

	BUILD_NEW_USERTYPE_START(luaState, Entity, "Manages the orientation of an Entity")
		sol::constructors<Entity(Entity*), Entity(std::string, EntityType)>(),
		BUILD_USERTYPE_BASE_CLASS(Moveable),
		BUILD_USERTYPE_VARIABLE(Entity, children),
		BUILD_USERTYPE_VARIABLE(Entity, parent),
		BUILD_USERTYPE_VARIABLE(Entity, isDirty),
		BUILD_USERTYPE_VARIABLE(Entity, isActive),
		BUILD_USERTYPE_VARIABLE(Entity, isLocallyActive)
	BUILD_NEW_USERTYPE_END()

	BUILD_VARIABLE_DESCRIPTION(Entity, children);
	BUILD_VARIABLE_DESCRIPTION(Entity, parent);
	BUILD_VARIABLE_DESCRIPTION(Entity, isDirty);
	BUILD_VARIABLE_DESCRIPTION(Entity, isActive);
	BUILD_VARIABLE_DESCRIPTION(Entity, isLocallyActive);

	BUILD_USERTYPE_METHOD(luaState, Entity, OnRender, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, OnPick, "Gt");

	BUILD_USERTYPE_METHOD(luaState, Entity, AddChild, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, SetParent, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, GetChildren, "Gt");

	BUILD_USERTYPE_METHOD(luaState, Entity, GetName, "Gt");

	BUILD_OVERLOADED_METHOD_START(luaState, Entity, SetName, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Entity, SetName, void(const char*)),
		BUILD_OVERLOADED_METHOD_FORMAT(Entity, SetName, void(std::string))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, Entity, GetWorldTranslation, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, GetWorldPosition, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, GetEntityType, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, SetEntityType, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, RemoveFromWorld, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, FlaggedForRemoval, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Entity, Serialize, "Gt");

	BUILD_OVERLOADED_METHOD_START(luaState, Entity, ConvertEntityType, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Entity, ConvertEntityType, std::string(EntityType)),
		BUILD_OVERLOADED_METHOD_FORMAT(Entity, ConvertEntityType, EntityType(std::string))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::_ObjectData(sol::table& luaState) {


	// PointUV

	BUILD_NEW_USERTYPE_START(luaState, PointUV, "Internal handles for rendering an Object3D")
		BUILD_USERTYPE_VARIABLE(PointUV, vertex),
		BUILD_USERTYPE_VARIABLE(PointUV, normal),
		BUILD_USERTYPE_VARIABLE(PointUV, uv)
	BUILD_NEW_USERTYPE_END()


	// Point

	BUILD_NEW_USERTYPE_START(luaState, Point, "Internal handles for rendering an Object3D")
		BUILD_USERTYPE_VARIABLE(Point, vertex),
		BUILD_USERTYPE_VARIABLE(Point, normal)
	BUILD_NEW_USERTYPE_END()


	// PointData

	BUILD_NEW_USERTYPE(luaState, PointData, "Internal handles for rendering an Object3D");

	BUILD_OVERLOADED_METHOD_START(luaState, PointData, SetData, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(PointData, SetData, void(PointData)),
		BUILD_OVERLOADED_METHOD_FORMAT(PointData, SetData, void(byte*, int, int)),
		BUILD_OVERLOADED_METHOD_FORMAT(PointData, SetData, void(float*, int, bool))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, PointData, SetUV, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, Size, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, GetPointUV, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, GetPoint, "Gt");

	BUILD_OVERLOADED_METHOD_START(luaState, PointData, AddPoint, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(PointData, AddPoint, bool(Point)),
		BUILD_OVERLOADED_METHOD_FORMAT(PointData, AddPoint, bool(PointUV))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, PointData, DeletePoint, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, DataSize, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, HasUV, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PointData, ScaleUV, "Gt");


	// Poly

	BUILD_NEW_USERTYPE_START(luaState, Poly, "Internal handles for rendering an Object3D")
		BUILD_USERTYPE_VARIABLE(Poly, point)
	BUILD_NEW_USERTYPE_END();


	// PolygonData

	BUILD_NEW_USERTYPE(luaState, PolygonData, "Internal handles for rendering an Object3D")

	BUILD_OVERLOADED_METHOD_START(luaState, PolygonData, SetData, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(PolygonData, SetData, void(PolygonData)),
		BUILD_OVERLOADED_METHOD_FORMAT(PolygonData, SetData, void(byte*, int)),
		BUILD_OVERLOADED_METHOD_FORMAT(PolygonData, SetData, void(unsigned short*, int))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, PolygonData, AddPolygon, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, GetVector, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, GetData, "Gt");

	BUILD_USERTYPE_METHOD(luaState, PolygonData, Size, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, ElementCount, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, DataSize, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, ElementType, "Gt");
	BUILD_USERTYPE_METHOD(luaState, PolygonData, Reserve, "Gt");


	// Instance

	BUILD_NEW_USERTYPE_START(luaState, Instance, "Internal handles for rendering an Object3D")
		BUILD_USERTYPE_VARIABLE(Instance, position)
	BUILD_NEW_USERTYPE_END();


	// InstanceData

	BUILD_NEW_USERTYPE(luaState, InstanceData, "Internal handles for rendering an Object3D")

	BUILD_OVERLOADED_METHOD_START(luaState, InstanceData, SetData, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(InstanceData, SetData, void(InstanceData)),
		BUILD_OVERLOADED_METHOD_FORMAT(InstanceData, SetData, void(std::vector<Instance>))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, InstanceData, AddInstance, "Gt");
	BUILD_USERTYPE_METHOD(luaState, InstanceData, GetVector, "Gt");
	BUILD_USERTYPE_METHOD(luaState, InstanceData, GetData, "Gt");

	BUILD_USERTYPE_METHOD(luaState, InstanceData, Size, "Gt");
	BUILD_USERTYPE_METHOD(luaState, InstanceData, ElementCount, "Gt");
	BUILD_USERTYPE_METHOD(luaState, InstanceData, DataSize, "Gt");
	BUILD_USERTYPE_METHOD(luaState, InstanceData, Reserve, "Gt");
}

void TurbineUsertypeDefiner::_Object3D(sol::table& luaState) {

	BUILD_NEW_USERTYPE_START(luaState, Object3DHandles, "Internal handles for rendering an Object3D")
		BUILD_USERTYPE_VARIABLE(Object3DHandles, point_vbo),
		BUILD_USERTYPE_VARIABLE(Object3DHandles, polygon_vbo),
		BUILD_USERTYPE_VARIABLE(Object3DHandles, instance_vbo),
		BUILD_USERTYPE_VARIABLE(Object3DHandles, object_vao),
		BUILD_USERTYPE_VARIABLE(Object3DHandles, initialised),
		BUILD_USERTYPE_VARIABLE(Object3DHandles, dirty)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_VARIABLE(Object3DHandles, point_vbo);
	BUILD_USERTYPE_VARIABLE(Object3DHandles, polygon_vbo);
	BUILD_USERTYPE_VARIABLE(Object3DHandles, instance_vbo);
	BUILD_USERTYPE_VARIABLE(Object3DHandles, object_vao);
	BUILD_USERTYPE_VARIABLE(Object3DHandles, initialised);
	BUILD_USERTYPE_VARIABLE(Object3DHandles, dirty);

	BUILD_NEW_USERTYPE_START(luaState, Object3D, "Manages the rendering data of an Entity")
		sol::constructors<
			Object3D(std::string),
			Object3D(Object3D*, std::string),
			Object3D(std::string, std::string)
		>(),
			sol::base_classes, sol::bases<
			Material,
			Entity
		>(),
		BUILD_USERTYPE_VARIABLE(Object3D, useLight),
		BUILD_USERTYPE_VARIABLE(Object3D, useTexture),
		BUILD_USERTYPE_VARIABLE(Object3D, isTransparent),
		BUILD_USERTYPE_VARIABLE(Object3D, isWireframe),
		BUILD_USERTYPE_VARIABLE(Object3D, vertices),
		BUILD_USERTYPE_VARIABLE(Object3D, polygons)
	BUILD_NEW_USERTYPE_END()

	BUILD_VARIABLE_DESCRIPTION(Object3D, useLight);
	BUILD_VARIABLE_DESCRIPTION(Object3D, useTexture);
	BUILD_VARIABLE_DESCRIPTION(Object3D, isTransparent);
	BUILD_VARIABLE_DESCRIPTION(Object3D, isWireframe);
	BUILD_VARIABLE_DESCRIPTION(Object3D, vertices);
	BUILD_VARIABLE_DESCRIPTION(Object3D, polygons);

	BUILD_USERTYPE_METHOD(luaState, Object3D, OnRender, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Object3D, OnPick, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Object3D, Draw, "Gt");

	BUILD_OVERLOADED_METHOD_START(luaState, Object3D, SetVertexData, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetVertexData, void(byte*, int, int)),
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetVertexData, void(float*, int, bool)),
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetVertexData, void(PointData))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, Object3D, GetVertData, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Object3D, GetVertCount, "Gt");

	BUILD_OVERLOADED_METHOD_START(luaState, Object3D, SetTriangles, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetTriangles, void(byte*, int)),
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetTriangles, void(unsigned short*, int)),
		BUILD_OVERLOADED_METHOD_FORMAT(Object3D, SetTriangles, void(PolygonData))
	BUILD_OVERLOADED_METHOD_END();

	BUILD_USERTYPE_METHOD(luaState, Object3D, SetTexture, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Object3D, GetTexture, "Gt");
	BUILD_USERTYPE_METHOD(luaState, Object3D, IsInstanced, "Gt");
}

void TurbineUsertypeDefiner::_LuaUsertypeBuilder(sol::table& luaState, json data) {

	// luaState.new_usertype<Thing1>("Thing1");
	// BUILD_NEW_USERTYPE(luaState, Thing1, "Test thingie class");

	// BUILD_USERTYPE_MEMBER(luaState, Thing1, a, "This is a thing");
}

void TurbineUsertypeDefiner::_SetupGLM(sol::state& luaState) {

	auto glm = luaState["glm"].get_or_create<sol::table>();

	auto mult_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
		[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
		[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
	);

	glm.new_usertype<glm::vec4>("vec4",
		sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
		"w", &glm::vec4::w,
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z
	);

	glm.new_usertype<glm::vec3>("vec3",
		sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::multiplication, mult_overloads
	);

	glm.new_usertype<glm::vec2>("vec2",
		sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
	);

	//glm.new_usertype<glm::quat>("quat",
	//	sol::constructors<glm::quat(), glm::quat(float), glm::quat(float, float, float, float)>(),
	//	"w", &glm::quat::w,
	//	"x", &glm::quat::x,
	//	"y", &glm::quat::y,
	//	"z", &glm::quat::z
	//);
}

void TurbineUsertypeDefiner::MakeDictionary() {

	dictionary["Entity"] = DictionaryContainer();

	dictionary["Entity"].methods.push_back("Hehe");
	dictionary["Entity"].methods.push_back("Haaaaagas");
	dictionary["Entity"].methods.push_back("Haadagas");

	dictionary["Entity"].variables.push_back("Hooble");
	dictionary["Entity"].variables.push_back("Honguas");
}