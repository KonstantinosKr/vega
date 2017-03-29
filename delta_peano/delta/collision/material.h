/*
 * material.h
 *
 *  Created on: 23 Mar 2017
 *      Author: konstantinos
 */

#ifndef DELTA_COLLISION_MATERIAL_H_
#define DELTA_COLLISION_MATERIAL_H_

#include <map>

namespace delta {
  namespace collision {
    namespace material {

      enum class MaterialType: int{
         GOLD=1,
         GRAPHITE=2,
         WOOD=3
       };

      enum class MaterialDensity: int{
        GOLD=20000,
        GRAPHITE=1000,
        WOOD=1000
      };

      enum class MaterialInteraction: char{
        WOODWOOD,//6
        GOLDGOLD,//2
        GRAPHITEGRAPHITE,//4
        WOODGOLD,//4
        GRAPHITEWOOD,
        GRAPHITEGOLD
      };

      static struct InterfaceTri{
          double SFRICTION;
          double DAMPER;
      }tri;

      static struct InterfaceSph{
          double SFRICTION;
          double ROLLFRICTION;
          double DAMPER;
      }sph;

      static std::map<int, InterfaceSph> interfaceSphmap;
      static std::map<int, InterfaceTri> interfaceTrimap;
      static std::map<int, int> densitymap;

      int getInterfaceType(int materialA, int materialB);

      void initMaterial();
      int getCollisionInterface();
    }
  }
}

#endif /* DELTA_COLLISION_MATERIAL_H_ */
