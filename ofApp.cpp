#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	vector<glm::vec3> base_location = { glm::vec3() };
	auto noise_location = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	ofColor face_color;
	for (auto& location : base_location) {

		for (int radius = 50; radius <= 300; radius += 30) {

			this->setRingToMesh(this->face, this->frame, location, noise_location, radius, 15, face_color);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, glm::vec3 noise_location, float radius, float height, ofColor face_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 120) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 120) * DEG_TO_RAD), radius * sin((deg + 120) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 120) * DEG_TO_RAD), radius * sin((deg + 120) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value_x = ofNoise(
				noise_location.x, radius * 0.0005 - ofGetFrameNum() * 0.005);
			auto noise_value_y = ofNoise(
				noise_location.y, radius * 0.0005 - ofGetFrameNum() * 0.005);
			auto noise_value_z = ofNoise(
				noise_location.z, radius * 0.0005 - ofGetFrameNum() * 0.005);

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_value_x, 0, 1, -PI * 1, PI * 1), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value_y, 0, 1, -PI * 1, PI * 1), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(noise_value_z, 0, 1, -PI * 1, PI * 1), glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_z * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_color.setHsb(ofMap(deg, 0, 360, 0, 255), 180, 255);
		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
		}

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}