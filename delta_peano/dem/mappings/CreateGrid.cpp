#include "dem/mappings/CreateGrid.h"

#include "delta/primitives/cubes.h"
#include "delta/primitives/granulates.h"
#include "delta/primitives/properties.h"
#include "delta/primitives/hopper.h"
#include "delta/primitives/blender.h"
#include <delta/primitives/graphite.h>
#include <delta/primitives/surface.h>
#include <delta/primitives/assembly.h>

#include "peano/grid/aspects/VertexStateAnalysis.h"

#include <string>
#include <iostream>

/**
 * @todo Please tailor the parameters to your mapping's properties.
 */
peano::CommunicationSpecification   dem::mappings::CreateGrid::communicationSpecification() {
	return peano::CommunicationSpecification(peano::CommunicationSpecification::ExchangeMasterWorkerData::SendDataAndStateBeforeFirstTouchVertexFirstTime,peano::CommunicationSpecification::ExchangeWorkerMasterData::SendDataAndStateAfterLastTouchVertexLastTime,false);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexLastTimeSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::touchVertexFirstTimeSpecification() { 
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::RunConcurrentlyOnFineGrid,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::enterCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::leaveCellSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidFineGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::ascendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}
peano::MappingSpecification   dem::mappings::CreateGrid::descendSpecification() {
	return peano::MappingSpecification(peano::MappingSpecification::Nop,peano::MappingSpecification::AvoidCoarseGridRaces,true);
}

tarch::logging::Log                  dem::mappings::CreateGrid::_log( "dem::mappings::CreateGrid" );
dem::mappings::CreateGrid::Scenario  dem::mappings::CreateGrid::_scenario;
dem::mappings::CreateGrid::VScheme 	 dem::mappings::CreateGrid::_velocityScheme;
double                               dem::mappings::CreateGrid::_maxH;
double                               dem::mappings::CreateGrid::_minParticleDiam;
double                               dem::mappings::CreateGrid::_maxParticleDiam;
dem::mappings::CreateGrid::GridType  dem::mappings::CreateGrid::_gridType;
double								 dem::mappings::CreateGrid::_epsilon;
int 								 dem::mappings::CreateGrid::_noPointsPerParticle;

void dem::mappings::CreateGrid::setScenario(Scenario scenario, VScheme velocityScheme, double maxH, double particleDiamMin, double particleDiamMax,
											GridType gridType, double epsilon, int noPointsPerGranulate)
{
	_scenario        = scenario;
	_velocityScheme  = velocityScheme;
	_maxH            = maxH;
	_minParticleDiam = particleDiamMin;
	_maxParticleDiam = particleDiamMax;
	_gridType        = gridType;
	_epsilon 		 = epsilon;
	_noPointsPerParticle = noPointsPerGranulate;
}

void dem::mappings::CreateGrid::beginIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "beginIteration(State)", solverState );

	dem::ParticleHeap::getInstance().setName( "particle-heap" );

	logInfo( "beginIteration()", "maxH=" << _maxH );

	srand (time(NULL));

	_numberOfParticles = 0;
	_numberOfTriangles = 0;
	_numberOfObstacles = 0;

	logTraceOutWith1Argument( "beginIteration(State)", solverState);
}

void dem::mappings::CreateGrid::createInnerVertex(
		dem::Vertex&                                  fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
		dem::Vertex * const                           coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&                                    coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createInnerVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	if (fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined && _gridType!=NoGrid)
	{
		fineGridVertex.refine();
	}

	fineGridVertex.init();

	logTraceOutWith1Argument( "createInnerVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::createBoundaryVertex(
		dem::Vertex&                                 fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&  fineGridH,
		dem::Vertex * const                          coarseGridVertices,
		const peano::grid::VertexEnumerator&         coarseGridVerticesEnumerator,
		dem::Cell&                                   coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&     fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createBoundaryVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );

	if (fineGridH(0)>_maxH && fineGridVertex.getRefinementControl()==Vertex::Records::Unrefined && _gridType!=NoGrid)
	{
		fineGridVertex.refine();
	}

	fineGridVertex.init();

	logTraceOutWith1Argument( "createBoundaryVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::createCell(
		dem::Cell&                                fineGridCell,
		dem::Vertex * const                       fineGridVertices,
		const peano::grid::VertexEnumerator&      fineGridVerticesEnumerator,
		dem::Vertex * const                       coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&                                coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&  fineGridPositionOfCell)
{
	logTraceInWith4Arguments( "createCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );

	/*
	 * root cell enviroment creation (mainly for obstacles) or large scenario setups (nuclear SLA)
	 */
	if (coarseGridCell.isRoot())
	{
		std::vector<double>  xCoordinates;
		std::vector<double>  yCoordinates;
		std::vector<double>  zCoordinates;

		dem::Vertex&  vertex  = fineGridVertices[ fineGridVerticesEnumerator(0) ];

		double centreAsArray[] = {fineGridVerticesEnumerator.getCellCenter()(0),
								  fineGridVerticesEnumerator.getCellCenter()(1),
								  fineGridVerticesEnumerator.getCellCenter()(2)};

		switch (_scenario)
		{
			case sla:
			{
				/*
				 *nuclear single layer array;
				 * experiment of seismic shakes
				 *
				 * concept: drop all components in coarse grid then reassign vertex to refined grid
				 */

				/*
				 * create floor
				 *
				 */
				centreAsArray[1] = 0.5;

				double floorHeight = 0.05;
				delta::primitives::generateSurface( centreAsArray, 1, floorHeight, xCoordinates, yCoordinates, zCoordinates);

				double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				double influenceRadius = diameter/2 * 1.1;
				iREAL hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				bool isObstacle = true;
				int material = 2;
				bool friction = true;

				iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 10000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				int newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, _epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
				_numberOfParticles++; _numberOfObstacles++;

				xCoordinates.clear();
				yCoordinates.clear();
				zCoordinates.clear();

				//read nuclear graphite schematics
				std::vector<std::vector<std::string>> componentGrid;
				delta::sys::parseModelGridSchematics("input/nuclear_core", componentGrid);

				///place components of 2d array structure
				int elements = 5;
				double xBoxlength = delta::primitives::getxDiscritizationLength(1, elements);
				double halfXBoxlength = xBoxlength/2;

				double position[3];
				position[0] = halfXBoxlength;
				position[1] = (centreAsArray[1]+(floorHeight/2))+halfXBoxlength;
				position[2] = halfXBoxlength;

				std::vector<std::array<double, 3>> tmp = delta::primitives::array1d(position, 1, elements);

				for(std::vector<std::array<double, 3>>::iterator i=tmp.begin(); i!=tmp.end(); i++)
				{
					std::array<double, 3> ar = *i;

					position[0] = ar[0];
					position[1] = ar[1];
					position[2] = ar[2];

					//delta::primitives::generateSurface( position, xBoxlength*0.98, xBoxlength*0.98, xCoordinates, yCoordinates, zCoordinates);
					delta::primitives::generateBrickFB( position, 1, xCoordinates, yCoordinates, zCoordinates);

					double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
					double influenceRadius = diameter/2 * 1.1;
					double hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);

					iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 10000;

					delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
					delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

					int newParticleNumber = vertex.createNewParticle(position,
											xCoordinates, yCoordinates, zCoordinates,
											centerOfMass, inertia, inverse,
											mass, diameter, influenceRadius, _epsilon,
											hMin, false, 1, false, _numberOfParticles);

					_numberOfParticles++; _numberOfObstacles++;
					_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

					xCoordinates.clear();
					yCoordinates.clear();
					zCoordinates.clear();
				}

				return;
				break;
			}
			case freefallshort:
			{
				/*
				 *freefallshort;
				 * experiment where a small amount of particle freefall on to a table
				 */

				delta::primitives::generateSurface(centreAsArray, 0.3, 0.01, xCoordinates, yCoordinates, zCoordinates);

				double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				double influenceRadius = diameter/2 * 1.1;
				double hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				bool isObstacle = true;
				int material = 1;
				bool friction = false;

				iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 10000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				int newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, _epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++;
				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				return;
				break;
			}
			case hopper:
			{
				/*
				 * hopper experiment;
				 *
				 * particle are placed above the hopper and flow through the hopper structure then are at rest at the bottom
				 *
				 */
				double _hopperWidth = 0.26;
				double _hopperHatch = 0.10;

				delta::primitives::generateHopper( centreAsArray, _hopperWidth, _hopperHatch, xCoordinates, yCoordinates, zCoordinates);

				double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				double influenceRadius = diameter/2 * 1.1;
				double epsilon = _epsilon*1.5;
				double hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				bool isObstacle = true;
				int material = 1;
				bool friction = false;

				iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 10000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				int newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				//delta::primitives::meshDenseMultiplier(5, xCoordinates, yCoordinates, zCoordinates);

				_numberOfParticles++; _numberOfObstacles++;
				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				xCoordinates.clear();
				yCoordinates.clear();
				zCoordinates.clear();

				/**
				 * ******************* flooring creation
				 */

				centreAsArray[1] = 0.25;

				delta::primitives::generateSurface( centreAsArray, 1, 0.1, xCoordinates, yCoordinates, zCoordinates);

				diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				influenceRadius = diameter/2 * 1.1;
				epsilon = _epsilon*3;
				hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				isObstacle = true;
				material = 2;
				friction = true;

				rho = 10000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
				_numberOfParticles++; _numberOfObstacles++;

				return;
				break;
			}
			case frictionStatic:
			{
				/*
				 *
				 */
				delta::primitives::generateSurface(centreAsArray, 1, 0.01, xCoordinates, yCoordinates, zCoordinates);

				double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				double influenceRadius = diameter/2 * 1.1;
				double hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				bool isObstacle = true;
				int material = 1;
				bool friction = false;

				iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 100000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				int newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, _epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++;
				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				return;
				break;
			}
			case frictionSlide:
			{
				/*
				 *
				 */
				delta::primitives::generateSurface( centreAsArray, 1, 0.02, xCoordinates, yCoordinates, zCoordinates);

				double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
				double influenceRadius = diameter/2 * 1.1;
				double hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
				bool isObstacle = true;
				int material = 1;
				bool friction = false;

				iREAL inertia[9], inverse[9], mass = 1, centerOfMass[3], rho = 1000000;

				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);

				int newParticleNumber = vertex.createNewParticle(centreAsArray,
										xCoordinates, yCoordinates, zCoordinates,
										centerOfMass, inertia, inverse,
										mass, diameter, influenceRadius, _epsilon,
										hMin, isObstacle, material, friction, _numberOfParticles);

				_numberOfParticles++; _numberOfObstacles++;
				_numberOfTriangles += xCoordinates.size()/DIMENSIONS;

				return;
				break;
			}
		}
	}

	bool PlaceParticleInCell = true;

	switch (_scenario)
	{
		case TwoParticlesCrash:
			PlaceParticleInCell = tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(0), 2.0/3.0, 1E-4 ) | tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(TWO_POWER_D-1), 1.0/3.0, 1E-4);
			//PlaceParticleInCell = tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(0), 2.0/3.0, 0.0 ) | tarch::la::equals( fineGridVerticesEnumerator.getVertexPosition(TWO_POWER_D-1), 2.0/3.0, 0.0);
			break;
	}

	if (PlaceParticleInCell
		&& peano::grid::aspects::VertexStateAnalysis::doAllNonHangingVerticesCarryRefinementFlag(fineGridVertices, fineGridVerticesEnumerator,Vertex::Records::Unrefined)
		&& !peano::grid::aspects::VertexStateAnalysis::isOneVertexHanging(fineGridVertices,fineGridVerticesEnumerator))
	{
		int particlesInCellPerAxis = std::floor(fineGridVerticesEnumerator.getCellSize()(0) / _maxParticleDiam);

		if (particlesInCellPerAxis==0)
		{
			particlesInCellPerAxis = 1;

			_maxParticleDiam = fineGridVerticesEnumerator.getCellSize()(0);
			_minParticleDiam = std::min(_minParticleDiam,_maxParticleDiam);
			logWarning( "createCell(...)", "particle size has been too small for coarsest prescribed grid. Reduce particle size to " << std::min(_minParticleDiam,_maxParticleDiam) << "-" << fineGridVerticesEnumerator.getCellSize()(0) );

		}else{
			logDebug( "createCell", "particlesInCellPerAxis="<< particlesInCellPerAxis );
		}

		dfor(k,particlesInCellPerAxis)
		{
			const tarch::la::Vector<DIMENSIONS,double> centre = fineGridVerticesEnumerator.getVertexPosition() + _maxParticleDiam * (k.convertScalar<double>()+0.5);

			double particleDiameter = (_minParticleDiam + (_maxParticleDiam-_minParticleDiam) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) ) / std::sqrt( DIMENSIONS );

			dem::Vertex&  vertex = fineGridVertices[ fineGridVerticesEnumerator(0) ];

			double centreAsArray[] = {centre(0),centre(1),centre(2)};

			// We need these temporary guys if we use an aligned vector from Peano.
			// Peano's aligned vector and the default std::vector are not compatible.
			std::vector<double>  xCoordinates;
			std::vector<double>  yCoordinates;
			std::vector<double>  zCoordinates;

			if (_scenario == BlackHoleWithRandomOrientedCubes)
			{
				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateCube( centreAsArray, particleDiameter,
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						xCoordinates, yCoordinates, zCoordinates );
			}
			else if (_scenario == BlackHoleWithCubes)
			{
				if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
				delta::primitives::generateCube( centreAsArray, particleDiameter,
						0.0, // around x-axis 1/8
						1.0/8.0, // around y-axis 1/8
						1.0/8.0, // around z-axis 1/8
						xCoordinates, yCoordinates, zCoordinates );
			}
			else if (_scenario == TwoParticlesCrash || _scenario == RandomWithGranulates || _scenario == RandomWithCubes || _scenario == freefall)
			{
				if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
				{
					delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
				}
				else{
					delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
				}
			}
			else if(_scenario == sla)
			{
				return;
			}
			else if(_scenario == hopper)
			{
				double _hopperWidth = 0.26;

				double upper = 0.5 + (_hopperWidth/2);
				double lower = 0.5 - (_hopperWidth/2);
				if(centreAsArray[0] < upper && centreAsArray[0] > lower &&
				   centreAsArray[1] < upper+0.2 && centreAsArray[1] > upper &&
				   centreAsArray[2] < upper && centreAsArray[2] > lower)
				{
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
					}
					else{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}
			else if(_scenario==freefallshort)
			{
				double _wallWidth = 0.25;

				double upper = 0.5 + (_wallWidth/2);
				double lower = 0.5 - (_wallWidth/2);
				if(centreAsArray[0] < upper && centreAsArray[0] > lower &&
					centreAsArray[1] < upper+0.2 && centreAsArray[1] > upper &&
					centreAsArray[2] < upper && centreAsArray[2] > lower)
				{
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
					}
					else{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}
			else if(_scenario == frictionStatic)
			{
				if(centreAsArray[0] < 0.45 && centreAsArray[0] > 0.30 &&
				   centreAsArray[1] < 1-0.30 && centreAsArray[1] > 1-0.45 &&
				   centreAsArray[2] < 1-0.30 && centreAsArray[2] > 1-0.45)
				{
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
					}else
					{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}
			else if(_scenario == frictionSlide)
			{
				if(centreAsArray[0] < 0.45 && centreAsArray[0] > 0.30 &&
				   centreAsArray[1] < 0.5 && centreAsArray[1] > 0.35 &&
				   centreAsArray[2] < 1-0.30 && centreAsArray[2] > 1-0.45)
				{
					centreAsArray[1] = centreAsArray[1] + (0.16 - _epsilon*2);
					if(dem::mappings::Collision::_collisionModel == dem::mappings::Collision::CollisionModel::Sphere)
					{
						delta::primitives::generateCube(centreAsArray, particleDiameter, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);
						delta::primitives::meshDenseMultiplier(1, xCoordinates, yCoordinates, zCoordinates);
					}else
					{
						delta::primitives::generateParticle( centreAsArray, particleDiameter, xCoordinates, yCoordinates, zCoordinates, _noPointsPerParticle);
					}
				} else {return;}
			}

			int numberOfTtriangles = xCoordinates.size()/DIMENSIONS;

			iREAL inertia[9], inverse[9], centerOfMass[3], mass = 1, rho = 600;

			double diameter = delta::primitives::computeDiagonal(xCoordinates, yCoordinates, zCoordinates);
			double influenceRadius = diameter/2 * 1.1;
			double hMin;
			bool isObstacle = false;
			int material = 1;
			bool friction = true;
			if(dem::mappings::Collision::_collisionModel != dem::mappings::Collision::CollisionModel::Sphere)
			{
				hMin = delta::primitives::computeHMin(xCoordinates, yCoordinates, zCoordinates);
			}else
			{
				hMin = 0;
				delta::primitives::computeInertia(xCoordinates, yCoordinates, zCoordinates, rho, mass, centerOfMass, inertia);
				delta::primitives::computeInverseInertia(inertia, inverse, isObstacle);
			}

			int newParticleNumber = vertex.createNewParticle(centreAsArray,
									xCoordinates, yCoordinates, zCoordinates,
									centerOfMass, inertia, inverse,
									1, diameter, influenceRadius, _epsilon,
									hMin, isObstacle, material, friction, _numberOfParticles);

			#ifdef STATS
			logWarning( "createCell", "create particle at "<< centre << " with diameter " << particleDiameter << " and id: " << particleId);
			#endif
			_numberOfTriangles += xCoordinates.size()/DIMENSIONS;
			_numberOfParticles++;

			/**
			 * Set velocities for particle filling for scenario
			 *
			 */

			switch (_velocityScheme)
			{
			 	case noVScheme:
			 		vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = 0;

					vertex.getParticle(newParticleNumber)._persistentRecords._angular(0) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(1) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(2) = 0;
			 		break;
				case randomLinear:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity =
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;
					break;
				case randomLinearAngular:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity =
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0,
						2.0 * static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) - 1.0;

					vertex.getParticle(newParticleNumber)._persistentRecords._angular =
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX),
						static_cast<double>( rand() ) / static_cast<double>(RAND_MAX);
					break;
				case crash:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity = tarch::la::Vector<DIMENSIONS,double>(0.5) - fineGridVerticesEnumerator.getVertexPosition(k);
					break;
				case crashAB:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity = tarch::la::Vector<DIMENSIONS,double>(0.5) - fineGridVerticesEnumerator.getVertexPosition(k);
					vertex.getParticle(newParticleNumber)._persistentRecords._angular = tarch::la::Vector<DIMENSIONS,double>(0.0);
					break;
				case CrashSlideWithAngle:
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(0) = 0.5;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(1) = 0;
					vertex.getParticle(newParticleNumber)._persistentRecords._velocity(2) = 0;

					vertex.getParticle(newParticleNumber)._persistentRecords._angular(0) = 5;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(1) = 5;
					vertex.getParticle(newParticleNumber)._persistentRecords._angular(2) = 0;
					break;
			}
		}
	}
	logTraceOutWith1Argument( "createCell(...)", fineGridCell );
}

void dem::mappings::CreateGrid::endIteration(
		dem::State&  solverState
) {
	logTraceInWith1Argument( "endIteration(State)", solverState );

	solverState.incNumberOfParticles(_numberOfParticles);
	solverState.incNumberOfObstacles(_numberOfObstacles);

	logInfo( "endIteration(State)", "created "
			<< _numberOfParticles << " particles with "
			<< _numberOfObstacles << " obstacles "
			<< _numberOfTriangles << " triangles");

	//delta::sys::Sys::saveScenario(_numberOfParticles, _numberOfObstacles);

	logTraceOutWith1Argument( "endIteration(State)", solverState);
}

dem::mappings::CreateGrid::CreateGrid() {
	logTraceIn( "CreateGrid()" );
	logTraceOut( "CreateGrid()" );
}

dem::mappings::CreateGrid::~CreateGrid() {
	logTraceIn( "~CreateGrid()" );
	// @todo Insert your code here
	logTraceOut( "~CreateGrid()" );
}

#if defined(SharedMemoryParallelisation)
dem::mappings::CreateGrid::CreateGrid(const CreateGrid&  masterThread) {
	logTraceIn( "CreateGrid(CreateGrid)" );

	_numberOfParticles = 0;
	_numberOfTriangles = 0;
	_numberOfObstacles = 0;

	logTraceOut( "CreateGrid(CreateGrid)" );
}


void dem::mappings::CreateGrid::mergeWithWorkerThread(const CreateGrid& workerThread) {
	logTraceIn( "mergeWithWorkerThread(CreateGrid)" );

	_numberOfParticles += workerThread._numberOfParticles;
	_numberOfTriangles += workerThread._numberOfTriangles;
	_numberOfObstacles += workerThread._numberOfObstacles;

	logTraceOut( "mergeWithWorkerThread(CreateGrid)" );
}
#endif

void dem::mappings::CreateGrid::createHangingVertex(
		dem::Vertex&     fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                fineGridH,
		dem::Vertex * const   coarseGridVertices,
		const peano::grid::VertexEnumerator&      coarseGridVerticesEnumerator,
		dem::Cell&       coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                   fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "createHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "createHangingVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyHangingVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyHangingVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyHangingVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyVertex(
		const dem::Vertex&   fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "destroyVertex(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyVertex(...)", fineGridVertex );
}

void dem::mappings::CreateGrid::destroyCell(
		const dem::Cell&           fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "destroyCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "destroyCell(...)", fineGridCell );
}

#ifdef Parallel
void dem::mappings::CreateGrid::mergeWithNeighbour(
		dem::Vertex&  vertex,
		const dem::Vertex&  neighbour,
		int                                           fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&   fineGridH,
		int                                           level
) {
	logTraceInWith6Arguments( "mergeWithNeighbour(...)", vertex, neighbour, fromRank, fineGridX, fineGridH, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareSendToNeighbour(
		dem::Vertex&  vertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith3Arguments( "prepareSendToNeighbour(...)", vertex, toRank, level );
	// @todo Insert your code here
	logTraceOut( "prepareSendToNeighbour(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Vertex&  localVertex,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localVertex, toRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::CreateGrid::prepareCopyToRemoteNode(
		dem::Cell&  localCell,
		int                                           toRank,
		const tarch::la::Vector<DIMENSIONS,double>&   cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&   cellSize,
		int                                           level
) {
	logTraceInWith5Arguments( "prepareCopyToRemoteNode(...)", localCell, toRank, cellCentre, cellSize, level );
	// @todo Insert your code here
	logTraceOut( "prepareCopyToRemoteNode(...)" );
}

void dem::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
		dem::Vertex&  localVertex,
		const dem::Vertex&  masterOrWorkerVertex,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&  x,
		const tarch::la::Vector<DIMENSIONS,double>&  h,
		int                                       level
) {
	logTraceInWith6Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localVertex, masterOrWorkerVertex, fromRank, x, h, level );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

void dem::mappings::CreateGrid::mergeWithRemoteDataDueToForkOrJoin(
		dem::Cell&  localCell,
		const dem::Cell&  masterOrWorkerCell,
		int                                       fromRank,
		const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
		int                                       level
) {
	logTraceInWith3Arguments( "mergeWithRemoteDataDueToForkOrJoin(...)", localCell, masterOrWorkerCell, fromRank );
	// @todo Insert your code here
	logTraceOut( "mergeWithRemoteDataDueToForkOrJoin(...)" );
}

bool dem::mappings::CreateGrid::prepareSendToWorker(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker
) {
	logTraceIn( "prepareSendToWorker(...)" );
	// @todo Insert your code here
	logTraceOutWith1Argument( "prepareSendToWorker(...)", true );
	return true;
}

void dem::mappings::CreateGrid::prepareSendToMaster(
		dem::Cell&                       localCell,
		dem::Vertex *                    vertices,
		const peano::grid::VertexEnumerator&       verticesEnumerator,
		const dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&       coarseGridVerticesEnumerator,
		const dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&   fineGridPositionOfCell
) {
	logTraceInWith2Arguments( "prepareSendToMaster(...)", localCell, verticesEnumerator.toString() );
	// @todo Insert your code here
	logTraceOut( "prepareSendToMaster(...)" );
}


void dem::mappings::CreateGrid::mergeWithMaster(
		const dem::Cell&           workerGridCell,
		dem::Vertex * const        workerGridVertices,
		const peano::grid::VertexEnumerator& workerEnumerator,
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell,
		int                                                                  worker,
		const dem::State&          workerState,
		dem::State&                masterState
) {
	logTraceIn( "mergeWithMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "mergeWithMaster(...)" );
}


void dem::mappings::CreateGrid::receiveDataFromMaster(
		dem::Cell&                        receivedCell,
		dem::Vertex *                     receivedVertices,
		const peano::grid::VertexEnumerator&        receivedVerticesEnumerator,
		dem::Vertex * const               receivedCoarseGridVertices,
		const peano::grid::VertexEnumerator&        receivedCoarseGridVerticesEnumerator,
		dem::Cell&                        receivedCoarseGridCell,
		dem::Vertex * const               workersCoarseGridVertices,
		const peano::grid::VertexEnumerator&        workersCoarseGridVerticesEnumerator,
		dem::Cell&                        workersCoarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&    fineGridPositionOfCell
) {
	logTraceIn( "receiveDataFromMaster(...)" );
	// @todo Insert your code here
	logTraceOut( "receiveDataFromMaster(...)" );
}


void dem::mappings::CreateGrid::mergeWithWorker(
		dem::Cell&           localCell,
		const dem::Cell&     receivedMasterCell,
		const tarch::la::Vector<DIMENSIONS,double>&  cellCentre,
		const tarch::la::Vector<DIMENSIONS,double>&  cellSize,
		int                                          level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localCell.toString(), receivedMasterCell.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localCell.toString() );
}


void dem::mappings::CreateGrid::mergeWithWorker(
		dem::Vertex&        localVertex,
		const dem::Vertex&  receivedMasterVertex,
		const tarch::la::Vector<DIMENSIONS,double>&   x,
		const tarch::la::Vector<DIMENSIONS,double>&   h,
		int                                           level
) {
	logTraceInWith2Arguments( "mergeWithWorker(...)", localVertex.toString(), receivedMasterVertex.toString() );
	// @todo Insert your code here
	logTraceOutWith1Argument( "mergeWithWorker(...)", localVertex.toString() );
}
#endif

void dem::mappings::CreateGrid::touchVertexFirstTime(
		dem::Vertex&               fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                          fineGridH,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexFirstTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "touchVertexFirstTime(...)", fineGridVertex );
}


void dem::mappings::CreateGrid::touchVertexLastTime(
		dem::Vertex&         fineGridVertex,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridX,
		const tarch::la::Vector<DIMENSIONS,double>&                    fineGridH,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                       fineGridPositionOfVertex
) {
	logTraceInWith6Arguments( "touchVertexLastTime(...)", fineGridVertex, fineGridX, fineGridH, coarseGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfVertex );
	// @todo Insert your code here
	logTraceOutWith1Argument( "touchVertexLastTime(...)", fineGridVertex );
}


void dem::mappings::CreateGrid::enterCell(
		dem::Cell&                 fineGridCell,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&                             fineGridPositionOfCell
) {
	logTraceInWith4Arguments( "enterCell(...)", fineGridCell, fineGridVerticesEnumerator.toString(), coarseGridCell, fineGridPositionOfCell );
	// @todo Insert your code here
	logTraceOutWith1Argument( "enterCell(...)", fineGridCell );
}


void dem::mappings::CreateGrid::leaveCell(
		dem::Cell&           fineGridCell,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell,
		const tarch::la::Vector<DIMENSIONS,int>&      fineGridPositionOfCell
) {
}


void dem::mappings::CreateGrid::descend(
		dem::Cell * const          fineGridCells,
		dem::Vertex * const        fineGridVertices,
		const peano::grid::VertexEnumerator&                fineGridVerticesEnumerator,
		dem::Vertex * const        coarseGridVertices,
		const peano::grid::VertexEnumerator&                coarseGridVerticesEnumerator,
		dem::Cell&                 coarseGridCell
) {
}


void dem::mappings::CreateGrid::ascend(
		dem::Cell * const    fineGridCells,
		dem::Vertex * const  fineGridVertices,
		const peano::grid::VertexEnumerator&          fineGridVerticesEnumerator,
		dem::Vertex * const  coarseGridVertices,
		const peano::grid::VertexEnumerator&          coarseGridVerticesEnumerator,
		dem::Cell&           coarseGridCell
) {
}
