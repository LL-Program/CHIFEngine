
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
 * File: GUI.h
 * Last Change: Added FPS-History Ability
 */
 

#pragma once

#include "../Engine/Window.h"
#include "drawableObject.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../Engine/utils.h"

#include <list>
#include <vector>
#include <format> 

namespace chif::gui{
	class GUI : public chif::drawableObject
	{
	public:
		GUI(Window& w);
		~GUI();

		virtual void draw();
		virtual void update();

		GUI& subscribe(chif::drawableObject* subscriber);

		// FPS Values
		static std::vector<float> fps_history;
		int index = 0;

	private:
		std::list<chif::drawableObject*> subscribers;

	};
} // CHIF::GUI