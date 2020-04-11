#pragma once

// Lightweight color container
struct color4 {

public:

	float rgba[4];

	color4() {

		rgba[0] = 0.0f;
		rgba[1] = 0.0f;
		rgba[2] = 0.0f;
		rgba[3] = 0.0f;
	}

	color4(float red, float green, float blue, float alpha) {

		rgba[0] = red;
		rgba[1] = green;
		rgba[2] = blue;
		rgba[3] = alpha;
	}

	void Scale(float value);

	void R(float newVal);
	void G(float newVal);
	void B(float newVal);
	void A(float newVal);

	float R();
	float G();
	float B();
	float A();
};

inline void color4::Scale(float value) {

	rgba[0] *= value;
	rgba[1] *= value;
	rgba[2] *= value;
	rgba[3] *= value;
}

inline void color4::R(float newVal) {

	rgba[0] = newVal;
}

inline void color4::G(float newVal) {

	rgba[1] = newVal;
}

inline void color4::B(float newVal) {

	rgba[2] = newVal;
}

inline void color4::A(float newVal) {

	rgba[3] = newVal;
}

inline float color4::R() {

	return rgba[0];
}

inline float color4::G() {

	return rgba[1];
}

inline float color4::B() {

	return rgba[2];
}

inline float color4::A() {

	return rgba[3];
}