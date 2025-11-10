//
// ExtendedPhysicalEnvironment.h
//

#ifndef __INET_EXTENDEDPHYSICALENVIRONMENT_H
#define __INET_EXTENDEDPHYSICALENVIRONMENT_H

#include "inet/common/IVisitor.h"
#include "inet/common/geometry/base/ShapeBase.h"
#include "inet/common/geometry/common/GeographicCoordinateSystem.h"
#include "inet/common/geometry/common/RotationMatrix.h"
#include "inet/common/geometry/object/LineSegment.h"
#include "inet/environment/common/MaterialRegistry.h"
#include "inet/environment/common/PhysicalObject.h"
#include "inet/environment/contract/IObjectCache.h"
#include "inet/environment/contract/IPhysicalEnvironment.h"
#include "inet/common/INETDefs.h"

#include "inet/mobility/contract/IMobility.h"

#include <map>
#include <vector>

namespace inet {
namespace physicalenvironment {

class INET_API ExtendedPhysicalEnvironment : public cModule, public IPhysicalEnvironment, public cListener
{
  public:
    simsignal_t traciModuleAddedSignalId = -1;
    simsignal_t traciModuleRemovedSignalId = -1;

  protected:
    // Parameters
    IGeographicCoordinateSystem *coordinateSystem = nullptr;
    K temperature;
    Coord spaceMin;
    Coord spaceMax;

    // Submodules
    IObjectCache *objectCache = nullptr;
    IGround *ground = nullptr;

    // Internal state
    std::vector<const ShapeBase *> shapes;
    std::vector<const Material *> materials;
    std::vector<const PhysicalObject *> objects;

    // Vehicle handling
    std::map<int, PhysicalObject*> movingVehicles;

    // Cache maps
    std::map<int, const ShapeBase *> idToShapeMap;
    std::map<int, const Material *> idToMaterialMap;
    std::map<int, const PhysicalObject *> idToObjectMap;
    std::map<std::string, const Material *> nameToMaterialMap;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) { delete msg; }

    virtual void convertPoints(std::vector<Coord>& points);
    virtual void parseShapes(cXMLElement *xml);
    virtual void parseMaterials(cXMLElement *xml);
    virtual void parseObjects(cXMLElement *xml);

  public:
    ExtendedPhysicalEnvironment() {}
    virtual ~ExtendedPhysicalEnvironment();

    // Vehicle handling
    void addMovingVehicle(int id, const Coord& position, const Coord& size);
    void updateVehiclePosition(int id, const Coord& position, const Coord& size);


    // Overrides
    virtual IObjectCache *getObjectCache() const override { return objectCache; }
    virtual IGround *getGround() const override { return ground; }

    virtual K getTemperature() const { return temperature; }
    virtual const Coord& getSpaceMin() const override { return spaceMin; }
    virtual const Coord& getSpaceMax() const override { return spaceMax; }

    virtual const IMaterialRegistry *getMaterialRegistry() const override { return &MaterialRegistry::getInstance(); }

    virtual int getNumObjects() const override { return objects.size() + movingVehicles.size(); }
    virtual const PhysicalObject *getObject(int index) const override;
    virtual const PhysicalObject *getObjectById(int id) const override;

    virtual void visitObjects(const IVisitor *visitor, const LineSegment& lineSegment) const override;

    virtual void receiveSignal(cComponent *src, simsignal_t id, cObject *obj, cObject *details) override;

    void registerAllVehicles();
    void registerVehicle(cModule *vehicle);
};

} // namespace physicalenvironment
} // namespace inet

#endif
