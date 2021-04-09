// Termm--Fall 2020

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

using namespace glm;

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}

	vec3 minCoords = vec3(0.0,0.0,0.0);
	vec3 maxCoords = vec3(0.0,0.0,0.0);
	bool first = true;
	for (vec3 & vert : m_vertices) {
		if (first) {
			minCoords.x = vert.x;
			minCoords.y = vert.y;
			minCoords.z = vert.z;
			maxCoords.x = vert.x;
			maxCoords.y = vert.y;
			maxCoords.z = vert.z;
			first = false;
			continue;
		}

		minCoords.x  = glm::min(minCoords.x , vert.x);
		maxCoords.x = glm::max(maxCoords.x, vert.x);
		minCoords.y  = glm::min(minCoords.y , vert.y);
		maxCoords.y = glm::max(maxCoords.y, vert.y);
		minCoords.z  = glm::min(minCoords.z , vert.z);
		maxCoords.z = glm::max(maxCoords.z, vert.z);
	}

	vec3 center = (minCoords + maxCoords)/2;
	double radius = distance(minCoords, maxCoords)/2;

	boundingSphere = new NonhierSphere(center, radius);
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

Mesh::~Mesh() {
	if (boundingSphere != nullptr) delete boundingSphere;
}

bool Mesh::check(vec3 & rayOrigin, vec3 & rayDirection, double & result) {

#ifdef RENDER_BOUNDING_VOLUMES
	return boundingSphere->check(rayOrigin,rayDirection,result);
#endif
	if (!boundingSphere->check(rayOrigin,rayDirection,result)) return false;

	bool isHit = false;
    double minResult;
    vec3 resultVec;
    vec3 selected = vec3(0,0,0);
    //cout << m_vertices.size() << ", " << m_faces.size() << endl;
	for (auto & triangle : m_faces) {
		//cout << m_vertices.size() << ", " << m_faces.size() << endl;cout << "check" << endl;
		resultVec = getTriangleIntercept(rayOrigin, rayDirection, m_vertices[triangle.v1], m_vertices[triangle.v2], m_vertices[triangle.v3]);
		//cout << "Beta: " << resultVec.x << " Theta: " << resultVec.y << " t: " << resultVec.z << endl;
		if(resultVec.x >= 0 && resultVec.y >= 0 && (resultVec.y + resultVec.x) <= 1) {
	        if ((!isHit || resultVec.z < minResult) && resultVec.z > 0) {
	          minResult = resultVec.z;
	          selected.x = triangle.v1;
	          selected.y = triangle.v2;
	          selected.z = triangle.v3;
	          isHit = true;
	        }
	    }
	}
	if(isHit) lastNormal = normalize(cross(m_vertices[selected.y] - m_vertices[selected.x], m_vertices[selected.z] - m_vertices[selected.y]));
    result = minResult;
    return isHit;
 }

 vec3 Mesh::normal(vec3 & intersect) {
 #ifdef RENDER_BOUNDING_VOLUMES
	return boundingSphere->normal(intersect);
#endif
 	return lastNormal;
 } 