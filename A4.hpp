// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "polyroots.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);

bool getSphereIntercept(glm::vec3 & rayOrigin, glm::vec3 & rayDirection, glm::vec3 & c, double radius, double & result);
glm::vec3 getTriangleIntercept(glm::vec3 & rayOrigin, glm::vec3 & rayDirection, glm::vec3 & p1, glm::vec3 & p2, glm::vec3 & p3);