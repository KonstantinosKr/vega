/*
 * ParticleRecord.cpp
 *
 *  Created on: 27 Jun 2018
 *      Author: konstantinos
 */

#include "ParticleRecord.h"

delta::core::data::ParticleRecord::ParticleRecord(delta::world::structure::Object& object)
{
  this->_linearVelocity = object.getLinearVelocity();
  this->_angularVelocity = object.getAngularVelocity();
  this->_refAngularVelocity = object.getAngularVelocity();

  this->_centre = object.getCentre();
  this->_centreOfMass = object.getCentreOfMass();
  this->_refCentreOfMass = object.getCentreOfMass();

  this->_orientation[0] = 1.0;
  this->_orientation[1] = 0;
  this->_orientation[2] = 0;
  this->_orientation[3] = 0;
  this->_orientation[4] = 1.0;
  this->_orientation[5] = 0;
  this->_orientation[6] = 0;
  this->_orientation[7] = 0;
  this->_orientation[8] = 1.0;

  this->_material = object.getMaterial();

	if (!object.hasMesh())
	{
		this->_isMesh = false;
		_bbox = { object.getMinBoundaryVertex()[0], object.getMinBoundaryVertex()[1], object.getMinBoundaryVertex()[2],
				  object.getMaxBoundaryVertex()[0], object.getMaxBoundaryVertex()[1], object.getMaxBoundaryVertex()[2]};
	
	}	else {
		this->_isMesh = true;
		
		this->_xCoordinates = object.getMesh().getXCoordinatesAsVector();
		this->_yCoordinates = object.getMesh().getYCoordinatesAsVector();
		this->_zCoordinates = object.getMesh().getZCoordinatesAsVector();

		this->_refxCoordinates = object.getMesh().getXCoordinatesAsVector();
		this->_refyCoordinates = object.getMesh().getYCoordinatesAsVector();
		this->_refzCoordinates = object.getMesh().getZCoordinatesAsVector();

		this->_maxMeshSize = object.getMesh().getMaxMeshSize();
		this->_minMeshSize = object.getMesh().getMinMeshSize();
		this->_avgMeshSize = object.getMesh().getAvgMeshSize();
  	
		std::array<iREAL,3> minVertex = object.getMesh().computeBoundaryMinVertex();
		std::array<iREAL,3> maxVertex = object.getMesh().computeBoundaryMaxVertex();
		
		_bbox = {	minVertex[0], minVertex[1], minVertex[2],
					maxVertex[0], maxVertex[1], maxVertex[2]};
		
		object.computeInertia(_material, _mass, _centreOfMass.data(), _inertia.data());
		object.computeInverseInertia(_inertia.data(), _inverse.data(), object.getIsObstacle());
	} 
	
  this->_globalParticleID = object.getGlobalParticleId();
  this->_localParticleID = object.getLocalParticleId();

  this->_rad = object.getRad();
  this->_haloDiameter = object.getHaloDiameter();
  this->_diameter = object.getDiameter();
  this->_epsilon = object.getEpsilon();

  this->_isObstacle = object.getIsObstacle();
  this->_isFriction = object.getIsFriction();
  this->_isConvex = object.getIsConvex();

  iREAL rad = object.getRad();

  _tree = delta::core::data::OctTree(rad, _bbox);
}

bool delta::core::data::ParticleRecord::getIsMesh()
{
	if (this->_isMesh)
	{
		return true;
	}
	return false;
}
int delta::core::data::ParticleRecord::getGlobalParticleID()
{
  return _globalParticleID;
}

int delta::core::data::ParticleRecord::getLocalParticleID()
{
  return _localParticleID;
}

iREAL delta::core::data::ParticleRecord::getHaloDiameter()
{
  return _haloDiameter;
}

iREAL delta::core::data::ParticleRecord::getRad()
{
  return _rad;
}

iREAL delta::core::data::ParticleRecord::getDiameter()
{
  return _diameter;
}

iREAL delta::core::data::ParticleRecord::getMass()
{
  return _mass;
}

iREAL delta::core::data::ParticleRecord::getEpsilon()
{
  return _epsilon;
}

delta::geometry::material::MaterialType delta::core::data::ParticleRecord::getMaterial()
{
  return _material;
}

bool delta::core::data::ParticleRecord::getIsObstacle()
{
  return _isObstacle;
}

bool delta::core::data::ParticleRecord::getIsFriction()
{
  return _isFriction;
}

bool delta::core::data::ParticleRecord::getIsConvex()
{
  return _isConvex;
}

std::array<iREAL, 9> delta::core::data::ParticleRecord::getInertia()
{
  return _inertia;
}

std::array<iREAL, 9> delta::core::data::ParticleRecord::getInverse()
{
  return _inverse;
}

int delta::core::data::ParticleRecord::getNumberOfTriangles()
{
  return _xCoordinates.size()/3;
}

iREAL delta::core::data::ParticleRecord::getMaxMeshSize()
{
  return _maxMeshSize;
}

iREAL delta::core::data::ParticleRecord::getMinMeshSize()
{
  return _minMeshSize;
}

iREAL delta::core::data::ParticleRecord::getAvgMeshSize()
{
  return _avgMeshSize;
}

std::vector<iREAL> delta::core::data::ParticleRecord::getClosestXCoordinatesTriangles(iREAL x[3])
{
  std::vector<iREAL> xCoordinates;
  if(_tree.isInDomain(x))
  {
		std::array<iREAL, 6> boundary;
		_tree.getBoundary(x, boundary);

		for(int i=0; i<_xCoordinates.size(); i+=3)
		{
			if(((_xCoordinates[i] > boundary[0]) && (_xCoordinates[i] < boundary[3])) ||
			 ((_xCoordinates[i+1] > boundary[0]) && (_xCoordinates[i+1] < boundary[3])) ||
			 ((_xCoordinates[i+2] > boundary[0]) && (_xCoordinates[i+2] < boundary[3])))
			{
				xCoordinates.push_back(_xCoordinates[i]);
				xCoordinates.push_back(_xCoordinates[i+1]);
				xCoordinates.push_back(_xCoordinates[i+2]);
			}
		}
  }
  return xCoordinates;
}

std::vector<iREAL> delta::core::data::ParticleRecord::getClosestYCoordinatesTriangles(iREAL x[3])
{
  std::vector<iREAL> xCoordinates;
  if(_tree.isInDomain(x))
  {
		std::array<iREAL, 6> boundary;
		_tree.getBoundary(x, boundary);

		for(int i=0; i<_yCoordinates.size(); i+=3)
		{
			if(((_yCoordinates[i] > boundary[1]) && (_yCoordinates[i] < boundary[4])) ||
			 ((_yCoordinates[i+1] > boundary[1]) && (_yCoordinates[i+1] < boundary[4])) ||
			 ((_yCoordinates[i+2] > boundary[1]) && (_yCoordinates[i+2] < boundary[4])))
			{
				xCoordinates.push_back(_yCoordinates[i]);
				xCoordinates.push_back(_yCoordinates[i+1]);
				xCoordinates.push_back(_yCoordinates[i+2]);
			}
		}
  }
  return xCoordinates;
}

std::vector<iREAL> delta::core::data::ParticleRecord::getClosestZCoordinatesTriangles(iREAL x[3])
{
  std::vector<iREAL> xCoordinates;
  if(_tree.isInDomain(x))
  {
		std::array<iREAL, 6> boundary;
		_tree.getBoundary(x, boundary);

		for(int i=0; i<_zCoordinates.size(); i+=3)
		{
			if(((_zCoordinates[i] > boundary[2]) && (_zCoordinates[i] < boundary[5])) ||
				((_zCoordinates[i+1] > boundary[2]) && (_zCoordinates[i+1] < boundary[5])) ||
				((_zCoordinates[i+2] > boundary[2]) && (_zCoordinates[i+2] < boundary[5])))
			{
				xCoordinates.push_back(_zCoordinates[i]);
				xCoordinates.push_back(_zCoordinates[i+1]);
				xCoordinates.push_back(_zCoordinates[i+2]);
			}
		}
  }
  return xCoordinates;
}

delta::core::data::OctTree& delta::core::data::ParticleRecord::getTree()
{
  return _tree;
}

void delta::core::data::ParticleRecord::refineTree(double maxMeshSize)
{
  return _tree.refine(maxMeshSize, _bbox, _xCoordinates, _yCoordinates, _zCoordinates);
}

void delta::core::data::ParticleRecord::getSubsetOfMesh(
	  double x[3], double epsilon,
	  std::vector<iREAL> &xCoordinatesPartial,
	  std::vector<iREAL> &yCoordinatesPartial,
	  std::vector<iREAL> &zCoordinatesPartial)
{
  _tree.getBBOXOverlappedMesh(
	  x,
		epsilon,
	  _xCoordinates,
	  _yCoordinates,
	  _zCoordinates,
	  xCoordinatesPartial, 
    yCoordinatesPartial, 
    zCoordinatesPartial
	);
}

std::array<iREAL, 6> delta::core::data::ParticleRecord::getBbox()
{
  return _bbox;
}

delta::core::data::ParticleRecord::~ParticleRecord() {
  // TODO Auto-generated destructor stub
}
