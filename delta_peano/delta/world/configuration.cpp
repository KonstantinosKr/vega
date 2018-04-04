/*
 * configuration.cpp
 *
 *  Created on: 2 Apr 2018
 *      Author: konstantinos
 */

#include "delta/world/configuration.h"

#include "delta/geometry/properties.h"
#include "delta/geometry/primitive/granulate.h"
#include "delta/geometry/primitive/cube.h"
#include "delta/geometry/body/graphite.h"

 iREAL delta::world::getDiscritization(
    iREAL length,
    int number)
{
	return length/number;
}

 std::vector<std::array<iREAL, 3>> delta::world::array1d(
    iREAL position[3],
    iREAL xAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;
	std::array<iREAL, 3> point = {0.0, 0.0, 0.0};

	iREAL length = getDiscritization(xAxisLength, partsNo);

	point[0] = position[0];
	point[1] = position[1];
	point[2] = position[2];
	array.push_back(point);

	for(int i=0;i<partsNo-1;i++)
	{
		point[0] += length;
		array.push_back(point);
	}
	return array;
}

 std::vector<std::array<iREAL, 3>> delta::world::array2d(
    iREAL position[3],
    iREAL xyAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL length = getDiscritization(xyAxisLength, partsNo);

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::array1d(position, xyAxisLength, partsNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[2] += length;
	}

	return array;
}

 std::vector<std::array<iREAL, 3>> delta::world::array3d(
    iREAL position[3],
    iREAL xyzAxisLength,
    int partsNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL length = getDiscritization(xyzAxisLength, partsNo);

  iREAL resetx = position[0];
	iREAL resetz = position[2];

	for(int i=0;i<partsNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::array2d(position, xyzAxisLength, partsNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[0] = resetx;
		position[1] += length;
		position[2] = resetz;
		if(position[1] > 1.0)
			break;
	}

	return array;
}

 std::vector<std::array<iREAL, 3>> delta::world::array3d(
    iREAL position[3],
    iREAL xyzAxisLength,
    int partsXYZNo,
    iREAL yAxisLength,
    int partsYNo)
{
	std::vector<std::array<iREAL, 3>> array;

	iREAL lengthY =  getDiscritization(yAxisLength, partsYNo);

	for(int i=0;i<partsYNo;i++)
	{
		std::vector<std::array<iREAL, 3>> tmp = delta::world::array2d(position, xyzAxisLength, partsXYZNo);
		for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		{
			array.push_back(*j);
		}
		position[1] += lengthY;
		position[2] -= xyzAxisLength;
		if(position[1] > 1.0)
			break;
	}

	return array;
}

 void delta::world::collapseUniformGrid(
	iREAL position[3],
	std::vector<std::array<iREAL, 3>>& grid,
	int xzcuts,
	int ycuts,
	iREAL elementWidth,
	iREAL elementHeight,
	iREAL epsilon)
{
  int xAxisLoc = 0;
  int zAxisLoc = 0;
  int yAxisLoc = 0;

  elementWidth += epsilon*2;
  elementHeight += epsilon*2;

  double dx = (grid[1][0] - grid[0][0]) - elementWidth;
  //double dy = (grid[xzcuts*xzcuts][1] - grid[xzcuts*xzcuts-1][1]) - elementHeight;
  double dz = dx;

  ////////////////////////////////////////
  //collapse from left to right on X AXIS
  ////////////////////////////////////////
  for(int i=0; i<grid.size(); i++)
  {
    if(xAxisLoc > 0)
    {
      //printf("%f\n", dx);
      grid[i][0] -= dx;
    }
    if(yAxisLoc > 0)
    {
      //printf("%i\n", yAxisLoc);
      //grid[i][1] -= dy;
    }
    if(zAxisLoc > 0)
    {
      grid[i][2] += dz;
    }

    xAxisLoc++;
    /////////////////////////////
    if(xAxisLoc >= xzcuts)
    {
      xAxisLoc = 0;
      zAxisLoc++;
    }
    if(zAxisLoc >= xzcuts)
    {
      zAxisLoc = 0;
    }
    if(xAxisLoc == 0 && zAxisLoc == 0)
    {
      yAxisLoc++;
    }
    //////////////////////////
  }

}

 std::vector<std::array<iREAL, 3>> delta::world::makeGridLayout(
    iREAL position[3],
    int xzcuts,
    int ycuts,
    iREAL gridxyLength)
{
  return delta::world::array3d(position, gridxyLength, xzcuts, gridxyLength, ycuts);
}

 void delta::world::uniformlyDistributedTotalMass(
	  iREAL position[3],
	  int xzcuts,
	  int ycuts,
	  iREAL gridxyLength,
      iREAL totalMass,
	  iREAL hopperWidth,
      int index,
	  iREAL epsilon,
      bool isSphereOrNone,
      int noPointsPerParticle,
      std::vector<delta::geometry::Object> &insitufineObjects)
{
   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<double, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::geometry::Object particles(isSphereOrNone ? "sphere": "granulate", 0, p, delta::geometry::material::MaterialType::WOOD, false, false, epsilon, {0, -1, 0}, {0,0,0});

     insitufineObjects.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;
   iREAL dx = (hopperWidth - gridxyLength)/2;
   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<insitufineObjects.size(); i++)
   {
     std::array<double, 3> position = insitufineObjects[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     insitufineObjects[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::uniSphereRadius(totalMass, index, insitufineObjects);
  } else {
    delta::world::uniMeshGeometry(totalMass, noPointsPerParticle, insitufineObjects, index);

    /*
    delta::world::uniCubeGeometry(
        totalMass,
        material,
        noPointsPerParticle,
        rad,
        particleGrid,
        componentGrid,
        xCoordinatesArray,
        yCoordinatesArray,
        zCoordinatesArray,
        index);
    */
  }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  double maxRad = 0.0;
  double minRad = 1.00;

  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    if(maxRad <= insitufineObjects[i].getRad()) maxRad = insitufineObjects[i].getRad();
    if(minRad >= insitufineObjects[i].getRad()) minRad = insitufineObjects[i].getRad();
  }


  //lift above max radii
  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    std::array<double, 3> pos = insitufineObjects[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};
    insitufineObjects[i].setCentre(p);

    auto yCoordinates = insitufineObjects[i].getyCoordinates();

    if(yCoordinates.size() >= 0)
    {
      for(unsigned i=0; i<yCoordinates.size(); i++)
      {
        yCoordinates[i] += maxRad+epsilon;
      }
    }
  }
}

 void delta::world::nonUniformlyDistributedTotalMass(
		  iREAL position[3],
		  int xzcuts,
		  int ycuts,
		  iREAL gridxyLength,
		  iREAL totalMass,
		  iREAL hopperWidth,
		  int index,
		  iREAL epsilon,
          iREAL isSphereOrNone,
          iREAL subcellx,
          int _noPointsPerParticle,
          std::vector<delta::geometry::Object> &insitufineObjects)
{

   //create xzy cuts above hopper, position starts at left lower inner corner
   std::vector<std::array<iREAL, 3>> grid = delta::world::makeGridLayout(position, xzcuts, ycuts, gridxyLength);

   delta::geometry::material::MaterialType material = delta::geometry::material::MaterialType::WOOD;
   bool isObstacle = false;
   bool isFriction = false;

   iREAL xmin = 1; iREAL xmax = 0;
   for(unsigned i=0; i<grid.size(); i++)
   {
     std::array<double, 3> p = {grid[i][0], grid[i][1], grid[i][2]};

     delta::geometry::Object particles(isSphereOrNone ? "sphere": "granulate", 0, p, delta::geometry::material::MaterialType::WOOD, isObstacle, isFriction, epsilon, {0, -1, 0}, {0,0,0});

     insitufineObjects.push_back(particles);

     if(p[0] < xmin) xmin = p[0];
     if(p[0] > xmax) xmax = p[0];
   }

   //adjustment of xz dimension
   gridxyLength = xmax - xmin;
   //iREAL dx = (hopperWidth - gridxyLength)/2;
   iREAL dx = (hopperWidth - gridxyLength)/2;
   //printf("length1:%f\n", subGridLength);
   //printf("length2:%f\n", _hopperWidth-margin*2);

   for(unsigned i=0; i<insitufineObjects.size(); i++)
   {
     std::array<double, 3> position = insitufineObjects[i].getCentre();
     position[0] += dx;  position[2] += dx;
     iREAL tmp[3] = {position[0], position[1], position[2]};
     insitufineObjects[i].setCentre(tmp);
   }

  if(isSphereOrNone)
  {
    delta::world::nonUniSphereRadius(totalMass, index, subcellx, insitufineObjects);
  } else {

	/*
    delta::world::nonUniMeshGeometry(
        totalMass,
        subcellx,
        _noPointsPerParticle);*/

     }

  //////////////////////////////////////////////////////
  /////MIN AND MAX RADIUS//////////////////////////////
  //////////////////////////////////////////////////////
  double maxRad = 0.0;
  double minRad = 1.00;

  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    if(maxRad <= insitufineObjects[i].getRad()) maxRad = insitufineObjects[i].getRad();
    if(minRad >= insitufineObjects[i].getRad()) minRad = insitufineObjects[i].getRad();
  }


  //lift above max radii
  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    std::array<double, 3> pos = insitufineObjects[i].getCentre();
    iREAL p[3] = {pos[0], pos[1] + maxRad+epsilon, pos[2]};
    insitufineObjects[i].setCentre(p);

    auto yCoordinates = insitufineObjects[i].getyCoordinates();

    if(yCoordinates.size() >= 0)
    {
      for(unsigned i=0; i<yCoordinates.size(); i++)
      {
        yCoordinates[i] += maxRad+epsilon;
      }
    }
  }
  //////////////////////////////////////////////////////
  //////////////////////////////////////////////////////
}

 void delta::world::uniSphereRadius(
    iREAL totalMass,
    int index,
    std::vector<delta::geometry::Object> &insitufineObjects)
{

  if((!insitufineObjects.size()) > 0) return;

  delta::geometry::material::MaterialType material = insitufineObjects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)insitufineObjects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    insitufineObjects[i].generateSphere(radius);
  }
}

 void delta::world::uniMeshGeometry(
    iREAL totalMass,
    int noPointsPerParticle,
    std::vector<delta::geometry::Object> &insitufineObjects,
    int index)
{
  iREAL massPerParticle = totalMass/(iREAL)insitufineObjects.size();
  delta::geometry::material::MaterialType material = insitufineObjects[0].getMaterial();

  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    insitufineObjects[i].setParticleID(index);
    insitufineObjects[i].generateMesh(0,0,0,0,0,0,noPointsPerParticle, radius);
    iREAL mt = insitufineObjects[i].getMass();

    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(material);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  reMassTotal=0;
  for(unsigned j=0; j<insitufineObjects.size(); j++)
  {
    std::array<iREAL, 3> ar = insitufineObjects[j].getCentre(); position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];

    std::vector<double> xCoordinates = insitufineObjects[j].getxCoordinates();
    std::vector<double> yCoordinates = insitufineObjects[j].getyCoordinates();
    std::vector<double> zCoordinates = insitufineObjects[j].getzCoordinates();

    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinates, yCoordinates, zCoordinates);

    //iREAL mt = delta::geometry::properties::getMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], delta::geometry::material::MaterialType::WOOD);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3) * int(delta::geometry::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

 void delta::world::uniCubeGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  for(auto i:particleGrid)
  {
    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

    delta::geometry::primitive::cube::generateHullCube(position, radius*2, radius*2, radius*2, 0, 0, 0, 0, xCoordinates, yCoordinates, zCoordinates);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    iREAL mt = delta::geometry::properties::getMass(xCoordinates, yCoordinates, zCoordinates, material);

    reMassTotal += mt;
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    componentGrid.push_back("granulate");
    rad.push_back(radius);
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
  }
}

 void delta::world::nonUniSphereRadius(
    iREAL totalMass,
    int index,
    iREAL subcellx,
    std::vector<delta::geometry::Object> &insitufineObjects)
{
  if((!insitufineObjects.size()) > 0) return;

  delta::geometry::material::MaterialType material = insitufineObjects[0].getMaterial();

  iREAL massPerParticle = totalMass/(iREAL)insitufineObjects.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;

  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;

  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  std::vector<double> rad;
  for(unsigned i=index; i<insitufineObjects.size(); i++)
  {
    iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));
    rad.push_back(particleDiameter/2);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(particleDiameter/2,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }

  //get total mass
  //printf("TOTAL REMASS:%f\n", reMassTotal);

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);

  reMassTotal=0;
  for(unsigned i=0; i<rad.size(); i++)
  {
    rad[i] = rad[i] * rescale;
    insitufineObjects[index+i].generateSphere(rad[i]);

    reMassTotal += (4.0/3.0) * 3.14 * std::pow(i,3) * int(delta::geometry::material::materialToDensitymap.find(material)->second); //volume * mass
  }
  //printf("RESCALE:%f\n", rescale);
  //printf("TOTAL REREMASS:%f\n", reMassTotal);
}

 void delta::world::nonUniMeshGeometry(
    iREAL totalMass,
    delta::geometry::material::MaterialType material,
    iREAL subcellx, int noPointsPerParticle,
    std::vector<iREAL>  &rad,
    std::vector<std::array<iREAL, 3>> &particleGrid,
    std::vector<std::string> &componentGrid,
    std::vector<std::vector<iREAL>>  &xCoordinatesArray,
    std::vector<std::vector<iREAL>>  &yCoordinatesArray,
    std::vector<std::vector<iREAL>>  &zCoordinatesArray,
	int index)
{
  iREAL massPerParticle = totalMass/(iREAL)particleGrid.size();
  iREAL radius = std::pow((3.0*massPerParticle)/(4.0 * 3.14 * int(delta::geometry::material::materialToDensitymap.find(material)->second)), (1.0/3.0));

  iREAL reMassTotal = 0;
  //iREAL masssphere = 0;

  iREAL position[3];
  //std::vector<iREAL> rad;
  iREAL mindiam = radius;
  iREAL maxdiam = radius*2;
  if(radius*2 > subcellx)
    printf("ERROR:radius bigger than subcellx\n");

  for(auto i:particleGrid)
  {
    std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

    position[0] = i[0]; position[1] = i[1]; position[2] = i[2];

    iREAL particleDiameter = mindiam + (iREAL)(rand()) / ((iREAL) (RAND_MAX/(maxdiam-mindiam)));

    rad.push_back(particleDiameter/2);
    radius = particleDiameter/2;

    delta::geometry::primitive::granulate::generateParticle(position, (radius*2), xCoordinates, yCoordinates, zCoordinates, noPointsPerParticle);

    xCoordinatesArray.push_back(xCoordinates);
    yCoordinatesArray.push_back(yCoordinates);
    zCoordinatesArray.push_back(zCoordinates);

    iREAL mt = delta::geometry::properties::getMass(xCoordinates, yCoordinates, zCoordinates, material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinates, yCoordinates, zCoordinates);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);
    reMassTotal += mt;
    //masssphere += mt;
    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
    xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

    componentGrid.push_back("nonSpherical");
  }

  iREAL rescale = std::pow((totalMass/reMassTotal), 1.0/3.0);
  //printf("MASSSPHERE:%f MASSMESH:%f RESCALE:%f\n", masssphere, reMassTotal, rescale);

  //reMassTotal=0;
  for(unsigned j=0; j<xCoordinatesArray.size(); j++)
  {
    std::array<iREAL, 3> ar = particleGrid[j]; position[0] = ar[0]; position[1] = ar[1]; position[2] = ar[2];
    delta::geometry::properties::scaleXYZ(rescale, position, xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);

    rad[j] = rad[j] * rescale;

    //iREAL mt = delta::geometry::properties::getMass(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j], material);
    //iREAL vt = delta::geometry::properties::getVolume(xCoordinatesArray[j], yCoordinatesArray[j], zCoordinatesArray[j]);
    //iREAL vs = (4.0/3.0) * 3.14 * std::pow(radius,3);
    //iREAL ms = (4.0/3.0) * 3.14 * std::pow(radius,3)*int(delta::geometry::material::MaterialDensity::WOOD);

    //reMassTotal += mt;
    //masssphere += ms;

    //printf("SphereVol:%f SphereMas:%f TriVol:%.10f TriMas:%f\n", vs, ms, vt, mt);
  }
  //printf("MASSSPHERE:%f MASSMESH:%f\n", masssphere, reMassTotal);
}

 void delta::world::loadNuclearGeometry(
    iREAL position[3],
    iREAL width,
    int layers,
	iREAL epsilon,
	std::vector<delta::geometry::Object>& insitufineObjects)
{

  //_particleGrid, _componentGrid, _radArray, _minParticleDiam, _maxParticleDiam

  //measurements
  std::vector<iREAL> xCoordinates, yCoordinates, zCoordinates;
  delta::geometry::body::generateBrickFB(xCoordinates, yCoordinates, zCoordinates);
  iREAL w = delta::geometry::properties::getXZWidth(xCoordinates, yCoordinates, zCoordinates);
  iREAL h = delta::geometry::properties::getYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();

  //read nuclear graphite schematics
  //std::vector<std::vector<std::string>> compoGrid;
  //delta::core::parseModelGridSchematics("input/nuclear_core", compoGrid, componentGrid);

  ///place components of 2d array structure
  int elements = 46;
  iREAL arrayXZlength = width/2;
  iREAL xzWCell = delta::world::getDiscritization(arrayXZlength, elements);

  iREAL scalePercentage = 0;
  if(w > xzWCell)//brick is bigger than grid
  {
    iREAL excess = w - xzWCell;
    scalePercentage = ((w-excess)/w);
    printf("SCALE DOWN:%f w:%f wperc:%f excess:%f boxWidth:%f\n", scalePercentage, w, w/100, xzWCell, excess);
  }

  position[0] -= arrayXZlength/2;
  position[1] -= (h*scalePercentage);
  position[2] -= arrayXZlength/2;

  for(int i=0; i<layers; i++)
  {
    position[1] += (h*scalePercentage)*i;
    std::vector<std::array<iREAL, 3>> tmp = delta::world::array2d(position, arrayXZlength, elements);

    for(unsigned j=0; j<tmp.size(); j++)
    {
      auto material = delta::geometry::material::MaterialType::GRAPHITE;

      delta::geometry::Object obj("FB", 0, tmp[i], material, false, false, epsilon, {0,0,0}, {0,0,0});
      insitufineObjects.push_back(obj);
    }
    //std::cout << tmp.size() << " " << particleGrid.size() << std::endl;
  }

  for(unsigned i=0; i<insitufineObjects.size(); i++) {
    insitufineObjects[i].setRad(scalePercentage);
  }
}

 void delta::world::makeBrickGrid(
    iREAL position[3],
    iREAL arrayXZlength,
    int   xzElements,
    iREAL arrayYlength,
    int   yElements,
	iREAL epsilon,
    std::vector<delta::geometry::Object>& insitufineObjects)
{
  std::vector<iREAL>  xCoordinates, yCoordinates, zCoordinates;

  //////////////////////////MESH///////////////////////////////////////////////////////////////////////////////////
  //measurements
  iREAL pos[3]; pos[0] = pos[1] = pos[2] = 0;
  delta::geometry::body::generateBrickFB(pos, xCoordinates, yCoordinates, zCoordinates);
  iREAL width = delta::geometry::properties::getXZWidth(xCoordinates, yCoordinates, zCoordinates);
  iREAL height = delta::geometry::properties::getYw(xCoordinates, yCoordinates, zCoordinates);
  xCoordinates.clear(); yCoordinates.clear(); zCoordinates.clear();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///place components of 2d array structure
  iREAL xzWCell = delta::world::getDiscritization(arrayXZlength, xzElements);

  iREAL scalePercentage = 0;
  if(xzWCell < width)
  {//brick is bigger than grid
    iREAL excess = width - xzWCell;
    scalePercentage = ((width - excess)/width);
    //printf("SCALE DOWN: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }
  else if (xzWCell < width)
  {
    iREAL excess = width - xzWCell;
    scalePercentage = ((width - excess)/width);
  }
  else if (xzWCell < height)
  {
    iREAL excess = height - xzWCell;
    scalePercentage = ((height - excess)/height);
  } else {
    iREAL excess = xzWCell - height;
    scalePercentage = 1+((xzWCell - excess)/xzWCell);
    //printf("SCALE UP: %f xw:%f boxWidth:%f excess:%f\n", scalePercentage, width, xwCell, excess);
  }

  width *= scalePercentage;
  height *= scalePercentage;

  position[0] = (position[0]-arrayXZlength/2)+xzWCell/2;
  position[1] = position[1]+(height/2);
  position[2] = (position[2]-arrayXZlength/2)+xzWCell/2;

  std::vector<std::array<iREAL, 3>> particleGrid = delta::world::array3d(position, arrayXZlength, xzElements, arrayYlength, yElements);

  delta::world::collapseUniformGrid(position, particleGrid, xzElements, yElements, width, height, 0);

  for(unsigned i=0; i<particleGrid.size(); i++)
  {
    auto material = delta::geometry::material::MaterialType::GRAPHITE;

    delta::geometry::Object obj("FB", i, particleGrid[i], material, false, false, epsilon, {0,0,0}, {0,0,0});
    obj.setRad(scalePercentage);
    insitufineObjects.push_back(obj);
  }
}


 void delta::world::computeBoundary(
	 std::vector<delta::geometry::Object>& coarseObjects,
	 std::vector<delta::geometry::Object>& fineObjects,
	 std::vector<delta::geometry::Object>& insitufineObjects,
	 iREAL& minParticleDiam,
	 iREAL& maxParticleDiam,
	 iREAL *minComputeDomain,
	 iREAL *maxComputeDomain)
 {
   //COMPUTE MIN/MAX XYZ DOMAIN
   iREAL minx = std::numeric_limits<double>::max(), miny = std::numeric_limits<double>::max(), minz = std::numeric_limits<double>::max();
   iREAL maxx = std::numeric_limits<double>::min(), maxy = std::numeric_limits<double>::min(), maxz = std::numeric_limits<double>::min();

   iREAL minDiameter = std::numeric_limits<double>::max();
   iREAL maxDiameter = std::numeric_limits<double>::min();

   for(unsigned i=0; i<coarseObjects.size(); i++)
   {
     iREAL ominx = coarseObjects[i].getMinX();
     iREAL ominy = coarseObjects[i].getMinY();
     iREAL ominz = coarseObjects[i].getMinZ();
     if(ominx < minx) minx = ominx;
     if(ominy < miny) miny = ominy;
     if(ominz < minz) minz = ominz;

     iREAL omaxx = coarseObjects[i].getMaxX();
     iREAL omaxy = coarseObjects[i].getMaxY();
     iREAL omaxz = coarseObjects[i].getMaxZ();
     if(omaxx > maxx) maxx = omaxx;
     if(omaxy > maxy) maxy = omaxy;
     if(omaxz > maxz) maxz = omaxz;

     if(coarseObjects[i].getRad() * 2.0 < minDiameter)
     minDiameter = coarseObjects[i].getRad() * 2.0;
     if(coarseObjects[i].getRad() * 2.0 > maxDiameter)
     maxDiameter = coarseObjects[i].getRad() * 2.0;
   }

   for(unsigned i=0; i<insitufineObjects.size(); i++)
   {
     iREAL ominx = insitufineObjects[i].getMinX();
     iREAL ominy = insitufineObjects[i].getMinY();
     iREAL ominz = insitufineObjects[i].getMinZ();
     if(ominx < minx) minx = ominx;
     if(ominy < miny) miny = ominy;
     if(ominz < minz) minz = ominz;

     iREAL omaxx = insitufineObjects[i].getMaxX();
     iREAL omaxy = insitufineObjects[i].getMaxY();
     iREAL omaxz = insitufineObjects[i].getMaxZ();
     if(omaxx > maxx) maxx = omaxx;
     if(omaxy > maxy) maxy = omaxy;
     if(omaxz > maxz) maxz = omaxz;

     if(insitufineObjects[i].getRad() * 2.0 < minDiameter)
     minDiameter = insitufineObjects[i].getRad() * 2.0;
     if(insitufineObjects[i].getRad() * 2.0 > maxDiameter)
     maxDiameter = insitufineObjects[i].getRad() * 2.0;
   }

   for(unsigned i=0; i<fineObjects.size(); i++)
   {
    iREAL ominx = fineObjects[i].getMinX();
    iREAL ominy = fineObjects[i].getMinY();
    iREAL ominz = fineObjects[i].getMinZ();
    if(ominx < minx) minx = ominx;
    if(ominy < miny) miny = ominy;
    if(ominz < minz) minz = ominz;

    iREAL omaxx = fineObjects[i].getMaxX();
    iREAL omaxy = fineObjects[i].getMaxY();
    iREAL omaxz = fineObjects[i].getMaxZ();
    if(omaxx > maxx) maxx = omaxx;
    if(omaxy > maxy) maxy = omaxy;
    if(omaxz > maxz) maxz = omaxz;

    if(fineObjects[i].getRad() * 2.0 < minDiameter)
    minDiameter = fineObjects[i].getRad() * 2.0;
    if(fineObjects[i].getRad() * 2.0 > maxDiameter)
    maxDiameter = fineObjects[i].getRad() * 2.0;
   }

   maxParticleDiam = maxDiameter;
   minParticleDiam = minDiameter;

   minComputeDomain[0] = minx;
   minComputeDomain[1] = miny;
   minComputeDomain[2] = minz;

   maxComputeDomain[0] = maxx;
   maxComputeDomain[1] = maxy;
   maxComputeDomain[2] = maxz;
 }
