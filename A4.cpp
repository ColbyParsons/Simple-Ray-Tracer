// Termm--Fall 2020

#include <glm/ext.hpp>
#include "Primitive.hpp"
#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"

#include "A4.hpp"

using namespace glm;
using namespace std;

vec3 getColor(uint x, uint y, size_t w, size_t h) {
	double dx = x;
	double dy = y;
	double dw = w;
	double dh = h;
	double r = (dx/dw)/3;
	double g = (dy/dh)/3;
	double b = (dx*dy)/(dw*dh);
	return vec3(r,g,b);
}

vec3 getTriangleIntercept(vec3 & rayOrigin, vec3 & rayDirection, vec3 & p1, vec3 & p2, vec3 & p3) {

	mat3 dMat = mat3(
		p1 - p2, 
		p1 - p3,
		rayDirection
	);

	mat3 d1Mat = mat3(
		p1 - rayOrigin, p1 - p3, rayDirection
	);

	mat3 d2Mat = mat3(
		p1 - p2, p1 - rayOrigin, rayDirection
	);

	mat3 d3Mat = mat3(
		p1 - p2, p1 - p3, p1 - rayOrigin
	);

	double D = glm::determinant(dMat);
	double D1 = glm::determinant(d1Mat);
	double D2 = glm::determinant(d2Mat);
	double D3 = glm::determinant(d3Mat);
	vec3 retVec = vec3(D1/D, D2/D, D3/D);
	return retVec;
}

bool getSphereIntercept(vec3 & rayOrigin, vec3 & rayDirection, vec3 & c, double radius, double & result) {
	double A = glm::dot(rayDirection, rayDirection);
	double B = glm::dot(2*rayDirection, rayOrigin - c);
	double C = glm::dot(rayOrigin - c, rayOrigin - c) - radius*radius;

	// double retVal1 = (-1*B + discrim) / 2 * A;
	// double retVal2 = (B + discrim) / 2 * A;
	//cout << "A: " << A << " B: " << B << " C: " << C << endl;

	// *result = (retVal1 > retVal2) ? retVal2 : retVal1;
	double roots[2];
	size_t num = quadraticRoots(A,B,C,roots);

	if(num == 0) {
		//cout << "neg" << endl;
		return false;
	}

	if (num == 1) {
		result = roots[0];
		if (result < 0) return false;
		return true;
	}

	if (roots[0] < 0 && roots[1] < 0){
		return false;
	} else if (roots[0] < 0) {
		result = roots[1];
	} else if (roots[1] < 0) {
		result = roots[0];
	} else {
		result = (roots[0] > roots[1]) ? roots[1] : roots[0];
	}

	return true;
}

double calculateYPos(int frameNum) {
	double dFrame = (double)frameNum;
	double a = -222;
	double v1 = 333;
	if (frameNum < 73) {
		return (24 * 2 * v1 * dFrame + a*dFrame*dFrame)/(24 * 24 * 2);
	} else if (frameNum < 137) {
		v1 = 0.9 * v1;
		dFrame = (double)(frameNum - 72);
		return (24 * 2 * v1 * dFrame + a*dFrame*dFrame)/(24 * 24 * 2);
	} else if (frameNum < 196) {
		v1 = 0.9*0.9 * v1;
		dFrame = (double)(frameNum - 137);
		return (24 * 2 * v1 * dFrame + a*dFrame*dFrame)/(24 * 24 * 2);
	} else {
		v1 = 0.9 * 0.9 * 0.9 * v1;
		dFrame = (double)(frameNum - 196);
		return (24 * 2 * v1 * dFrame + a*dFrame*dFrame)/(24 * 24 * 2);
	}
}

void A4_Render(
		// What to render  
		SceneNode * root,

		// Image to write to, set to a given width and height  
		Image & image,

		// Viewing parameters  
		glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters  
		const glm::vec3 & ambient,
		const std::list<Light *> & lights,
		int frameNum
) {
	std::cout << "Frame: " << frameNum << std::endl;
	
	vec3 newEye = eye;
	newEye.z -= (double)(frameNum/2);
	newEye.x -= (double)frameNum;
  // Fill in raytracing code here...

 //  std::cout << "F20: Calling A4_Render(\n" <<
	// 	  "\t" << *root <<
 //          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
 //          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
	// 	  "\t" << "view: " << glm::to_string(view) << std::endl <<
	// 	  "\t" << "up:   " << glm::to_string(up) << std::endl <<
	// 	  "\t" << "fovy: " << fovy << std::endl <<
 //          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
	// 	  "\t" << "lights{" << std::endl;

	// for(const Light * light : lights) {
	// 	std::cout << "\t\t" <<  *light << std::endl;
	// }
	// std::cout << "\t}" << std::endl;
	// std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();
	double windowY = tan(radians(fovy/2)) * 2;
	double windowX = windowY * (((double)w)/((double)h));
	// cout << "h: " << h << endl;
	// cout << "w: " << w << endl;
	// cout << "windowY: " << windowY << endl;
	// cout << "windowX: " << windowX << endl;
	// transform pixel coordinate to world coords
	// we use the algorithm from the supplemental information of lesson 20
	mat4 T1 = glm::translate(mat4(1), vec3(-((double)(w))/2, -((double)(h))/2, 1.0)); // TODO: WHAT IS D AND HOW TO GET IT
	mat4 S2 = glm::scale(mat4(1), vec3(-windowX/w, -windowY/h, 1.0)); //TODO: figure out if w/h is different than nx/ny
	vec3 wVec = normalize(view - newEye); // TODO: figure out if LookFrom = eye and LookAt = view or not 
														// ALSO WE MAY NEED TO NORMALIZE LOOKAT AND (MAYBE)LOOKFROM
	vec3 uVec = normalize(glm::cross(up, wVec));
	vec3 vVec = normalize(glm::cross(wVec, uVec));

	// cout << "wVec: " << wVec.x << ", " << wVec.y << ", " << wVec.z << endl;
	// cout << "uVec: " << uVec.x << ", " << uVec.y << ", " << uVec.z << endl;
	// cout << "vVec: " << vVec.x << ", " << vVec.y << ", " << vVec.z << endl;

	mat4 R3 = mat4(
		uVec.x, uVec.y, uVec.z, 0,
		vVec.x, vVec.y, vVec.z, 0,
		wVec.x, wVec.y, wVec.z, 0,
		0,      0,      0,      1
	);
	mat4 T4 = glm::translate(mat4(1), newEye);

	mat4 T = T4*R3*S2*T1;

	// cout << R3[0][0] << ", " << R3[1][0] << ", " << R3[2][0] << ", " << R3[3][0] << endl;
	// cout << R3[0][1] << ", " << R3[1][1] << ", " << R3[2][1] << ", " << R3[3][1] << endl;
	// cout << R3[0][2] << ", " << R3[1][2] << ", " << R3[2][2] << ", " << R3[3][2] << endl;
	// cout << R3[0][3] << ", " << R3[1][3] << ", " << R3[2][3] << ", " << R3[3][3] << endl;
	double dFrame = (double)frameNum;
	double yYy = calculateYPos(frameNum);
	cout << yYy << endl;
	vec3 ballTranslate = vec3(-dFrame, yYy , dFrame);

	for (SceneNode * child : root->children) {
		if (child->m_nodeType != NodeType::GeometryNode) continue;
		GeometryNode * geometryNode = static_cast<GeometryNode *>(child);
		geometryNode->m_primitive->update(ballTranslate);
	}

	vec3 rayOrigin = newEye;
	vec3 colorForPixel;
	double sampleStartX;
	double sampleStartY;
	uint sampleSize = 9;
	uint colorIndex;
	vec3 colours[9];
	size_t step = h/10;
	size_t stepCounter = 0;
	// for all pixels
	for (uint y = 0; y < h; ++y) {
		if (y % step == 0) {
			//cout << "Progress: " << stepCounter << "%" << endl;
			stepCounter += 10;
		}
		for (uint x = 0; x < w; ++x) {
			sampleStartX = ((double)x) - 0.5;
			sampleStartY = ((double)y) - 0.5;
			//cout << sampleSize << endl;
			colorIndex = 0;
			for (double xPrime = sampleStartX; xPrime <= sampleStartX + 1; xPrime += 0.34) {
				for (double yPrime = sampleStartY; yPrime <= sampleStartY + 1; yPrime += 0.34) {
					vec4 worldCoord = vec4((double)xPrime, (double)yPrime, 0.0, 1.0);
					worldCoord = T*worldCoord;
					//cout << "worldCoord1: " << worldCoord.x << ", " << worldCoord.y << ", " << worldCoord.z << endl;
					vec3 threeCoord = vec3(worldCoord.x, worldCoord.y, worldCoord.z);
					vec3 rayDirection = threeCoord - rayOrigin;
					//cout << "dir: " << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << endl;
					colours[colorIndex] = getColor(x, y, w, h);

					// now we compute intersections
					double minResult;
					GeometryNode * hit = nullptr;
					PhongMaterial * material;
					bool first = true;
					for (SceneNode * child : root->children) {
						if (child->m_nodeType != NodeType::GeometryNode) continue;
						GeometryNode * geometryNode = static_cast<GeometryNode *>(child);
						double result;
						if(geometryNode->m_primitive->check(rayOrigin, rayDirection, result)) {
							if (first || result < minResult) {
								first = false;
								minResult = result;
								hit = geometryNode;
								//cout << result << endl;
							}
						}
					}
					if (hit != nullptr) {
						//cout << hit->m_name << endl;
						material = static_cast<PhongMaterial *>(hit->m_material);

						if (hit->m_primitive->colour == 1) {
							material = new PhongMaterial(vec3(0.9,0.9,0.9), vec3(0.1,0.1,0.1), 10);
						}

						vec3 sum = vec3(0.0,0.0,0.0);
						// l is light direction
						// n is surface normal
						// v is view direction
						// h is half vector (bisector between v and l)
						// r is the distance between tne light source and the hit point
						// algorithms taken from chapter 4.5.2 of Fundamentals of computer graphics
						vec3 hitPoint = rayOrigin + minResult*rayDirection;
						vec3 v = rayDirection;
						vec3 n = hit->m_primitive->normal(hitPoint);
						for(const Light * light : lights) {
							double r = distance(hitPoint, light->position);
							vec3 l = normalize(light->position - hitPoint);
							vec3 h = normalize(l + v);
							double temp = dot(n,h);
							vec3 specularMult = pow(clamp(temp, 0.0, 1.0), material->m_shininess) * material->m_ks;
							temp = dot(l,n);
							vec3 diffuseMult = clamp(temp, 0.0, 1.0) * material->m_kd;
							vec3 intensities = light->colour /(light->falloff[0] + light->falloff[1] * r + light->falloff[2] * r * r);

							//check shadow rays
							bool isShadowed = false;
							vec3 dir = normalize(l);
							vec3 start = hitPoint + 0.01*dir;
							for (SceneNode * child : root->children) {
								if (child->m_nodeType != NodeType::GeometryNode) continue;
								GeometryNode * geometryNode = static_cast<GeometryNode *>(child);
								double result;
								if(geometryNode->m_primitive->check(start, dir, result)) {
									isShadowed = true;
									break;
								}
							}
							if(!isShadowed) sum = sum + intensities * (diffuseMult + specularMult);
						}
						sum = sum + ambient * material->m_kd;
						colours[colorIndex] = sum;	
					}
					colorIndex++;
				}
			}
			vec3 colorForPixel = vec3(0.0,0.0,0.0);
			for (size_t i = 0; i < sampleSize; i++) {
				colorForPixel = colorForPixel + colours[i];
			}
			colorForPixel = (1/((double)sampleSize)) * colorForPixel;

			// Red
			image(x, y, 0) = (double)colorForPixel.x;
			// Green: 
			image(x, y, 1) = (double)colorForPixel.y;
			// Blue: 
			image(x, y, 2) = (double)colorForPixel.z;
		}
	}
}
