
#include "Matrix.h"
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Serialize {

public:

	static void SerializeVec3(json& json, std::string name, glm::vec3 vec);
	static void SerializeVec4(json& json, std::string name, glm::vec4 vec);
	static void SerializeQuat(json& json, std::string name, glm::quat quat);
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