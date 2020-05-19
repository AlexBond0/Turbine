#pragma once

#include "Matrix.h"
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Serialize {

public:

	static void SerializeVec3(json& json, std::string name, glm::vec3 vec);
	static void SerializeVec4(json& json, std::string name, glm::vec4 vec);
	static void SerializeQuat(json& json, std::string name, glm::quat quat);

	static glm::vec3 DeserializeVec3(json& json);
	static glm::vec4 DeserializeVec4(json& json);
	static glm::quat DeserializeQuat(json& json);
};

inline void Serialize::SerializeVec3(json& json, std::string name, glm::vec3 vec) {

	json[name] = { vec.x, vec.y, vec.z };
}

inline void Serialize::SerializeVec4(json& json, std::string name, glm::vec4 vec) {

	json[name] = { vec.w, vec.x, vec.y, vec.z };
}

inline void Serialize::SerializeQuat(json& json, std::string name, glm::quat quat) {

	json[name] = { quat.w, quat.x, quat.y, quat.z};
}

inline glm::vec3 Serialize::DeserializeVec3(json& json) {

	return glm::vec3(json.at(0), json.at(1), json.at(2));
}

inline glm::vec4 Serialize::DeserializeVec4(json& json) {

	return glm::vec4(json.at(0), json.at(1), json.at(2), json.at(3));
}

inline glm::quat Serialize::DeserializeQuat(json& json) {

	return glm::quat(json.at(0), json.at(1), json.at(2), json.at(3));
}