
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: Water.cpp
 * Last Change: 
 */
 

#include "Water.h"
#include "sceneElements.h"
#include "Terrain.h"
#include "../External/imgui/imgui.h"
#include "../Utils/utils.h"

namespace chif::Graphics {
	Water::Water(glm::vec2 position, float scale, float height): scale(scale), height(height){

		//shad = new Shader("shaders/waterVertexShader.vert", "shaders/waterFragmentShader.frag");
		shad = new chif::Graphics::Renderer::Shader::Shader("WaterShader");
		shad->attachShader("shaders/water.vert")
			->attachShader("shaders/water.frag")
			->linkPrograms();


		glm::mat4 identity;
		glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
		glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position.x, height, position.y));
		this->modelMatrix = transMatrix * scaleMatrix;

		reflectionFBO = new chif::Graphics::Renderer::Buffer::FrameBufferObject(FBOw, FBOh);
		refractionFBO = new chif::Graphics::Renderer::Buffer::FrameBufferObject(FBOw, FBOh);

		initializePlaneVAO(2, chif::Graphics::Terrain::tileW, &planeVAO, &planeVBO, &planeEBO);
	}
	
	void Water::bindReflectionFBO() {
		reflectionFBO->bind();
	}

	void Water::bindRefractionFBO() {
		refractionFBO->bind();
	}

	const int res = 2;

	void Water::drawVertices() {
		glBindVertexArray(planeVAO);
		//shader.use();
		shad->use();
		glDrawElements(GL_TRIANGLES, (res - 1)*(res - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


	void Water::setGui()
	{
		ImGui::Begin("Water controls");
		ImGui::SliderFloat("Water height", &height, 0.0f, 1000.f);
		ImGui::End();
	}

	void Water::draw() {
		shad->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sceneElements * se = drawableObject::scene;

		//this->setPosition(glm::vec2(se->cam.Position[0], se->cam.Position[2]), scale, height);
		this->setHeight(height);

		shad->setMat4("modelMatrix", modelMatrix);
		shad->setMat4("gVP", se->projMatrix*se->cam->GetViewMatrix());

		shad->setVec3("u_LightColor", se->lightColor);
		shad->setVec3("u_LightPosition", se->lightPos);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, reflectionFBO->tex);
		shad->setInt("reflectionTex", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, refractionFBO->tex);
		shad->setInt("refractionTex", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudvMap);
		shad->setInt("waterDUDV", 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		shad->setInt("normalMap", 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, refractionFBO->depthTex);
		shad->setInt("depthMap", 4);

		float waveSpeed = 0.25;
		float time = sdlUtilGetTime();

		float moveFactor = waveSpeed * time;
		shad->setFloat("moveFactor", moveFactor);

		shad->setVec3("cameraPosition", se->cam->Position);

		//waterPlane->Draw(*shad);
		this->drawVertices();
		glDisable(GL_BLEND);
	}

	void Water::unbindFBO() {
		chif::Graphics::Renderer::Buffer::unbindCurrentFrameBuffer(chif::Platform::Window::SCR_WIDTH, chif::Platform::Window::SCR_WIDTH);
	}



	Water::~Water()
	{
	}
} // NAMESPACE chif::Graphics