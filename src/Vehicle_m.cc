//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Vehicle.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Vehicle_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(Vehicle)

Vehicle::Vehicle(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

Vehicle::Vehicle(const Vehicle& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

Vehicle::~Vehicle()
{
}

Vehicle& Vehicle::operator=(const Vehicle& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Vehicle::copy(const Vehicle& other)
{
    this->srcEndpoint = other.srcEndpoint;
    this->dstEndpoint = other.dstEndpoint;
    this->vehNumber = other.vehNumber;
    this->enterTime = other.enterTime;
    this->totalTransitTime = other.totalTransitTime;
    this->queueEnterTime = other.queueEnterTime;
    this->queueWaitingTime = other.queueWaitingTime;
    this->queueExitTime = other.queueExitTime;
    this->drivingTime = other.drivingTime;
    this->hopCount = other.hopCount;
}

void Vehicle::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcEndpoint);
    doParsimPacking(b,this->dstEndpoint);
    doParsimPacking(b,this->vehNumber);
    doParsimPacking(b,this->enterTime);
    doParsimPacking(b,this->totalTransitTime);
    doParsimPacking(b,this->queueEnterTime);
    doParsimPacking(b,this->queueWaitingTime);
    doParsimPacking(b,this->queueExitTime);
    doParsimPacking(b,this->drivingTime);
    doParsimPacking(b,this->hopCount);
}

void Vehicle::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcEndpoint);
    doParsimUnpacking(b,this->dstEndpoint);
    doParsimUnpacking(b,this->vehNumber);
    doParsimUnpacking(b,this->enterTime);
    doParsimUnpacking(b,this->totalTransitTime);
    doParsimUnpacking(b,this->queueEnterTime);
    doParsimUnpacking(b,this->queueWaitingTime);
    doParsimUnpacking(b,this->queueExitTime);
    doParsimUnpacking(b,this->drivingTime);
    doParsimUnpacking(b,this->hopCount);
}

const char * Vehicle::getSrcEndpoint() const
{
    return this->srcEndpoint.c_str();
}

void Vehicle::setSrcEndpoint(const char * srcEndpoint)
{
    this->srcEndpoint = srcEndpoint;
}

const char * Vehicle::getDstEndpoint() const
{
    return this->dstEndpoint.c_str();
}

void Vehicle::setDstEndpoint(const char * dstEndpoint)
{
    this->dstEndpoint = dstEndpoint;
}

unsigned int Vehicle::getVehNumber() const
{
    return this->vehNumber;
}

void Vehicle::setVehNumber(unsigned int vehNumber)
{
    this->vehNumber = vehNumber;
}

omnetpp::simtime_t Vehicle::getEnterTime() const
{
    return this->enterTime;
}

void Vehicle::setEnterTime(omnetpp::simtime_t enterTime)
{
    this->enterTime = enterTime;
}

omnetpp::simtime_t Vehicle::getTotalTransitTime() const
{
    return this->totalTransitTime;
}

void Vehicle::setTotalTransitTime(omnetpp::simtime_t totalTransitTime)
{
    this->totalTransitTime = totalTransitTime;
}

omnetpp::simtime_t Vehicle::getQueueEnterTime() const
{
    return this->queueEnterTime;
}

void Vehicle::setQueueEnterTime(omnetpp::simtime_t queueEnterTime)
{
    this->queueEnterTime = queueEnterTime;
}

omnetpp::simtime_t Vehicle::getQueueWaitingTime() const
{
    return this->queueWaitingTime;
}

void Vehicle::setQueueWaitingTime(omnetpp::simtime_t queueWaitingTime)
{
    this->queueWaitingTime = queueWaitingTime;
}

omnetpp::simtime_t Vehicle::getQueueExitTime() const
{
    return this->queueExitTime;
}

void Vehicle::setQueueExitTime(omnetpp::simtime_t queueExitTime)
{
    this->queueExitTime = queueExitTime;
}

omnetpp::simtime_t Vehicle::getDrivingTime() const
{
    return this->drivingTime;
}

void Vehicle::setDrivingTime(omnetpp::simtime_t drivingTime)
{
    this->drivingTime = drivingTime;
}

unsigned int Vehicle::getHopCount() const
{
    return this->hopCount;
}

void Vehicle::setHopCount(unsigned int hopCount)
{
    this->hopCount = hopCount;
}

class VehicleDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcEndpoint,
        FIELD_dstEndpoint,
        FIELD_vehNumber,
        FIELD_enterTime,
        FIELD_totalTransitTime,
        FIELD_queueEnterTime,
        FIELD_queueWaitingTime,
        FIELD_queueExitTime,
        FIELD_drivingTime,
        FIELD_hopCount,
    };
  public:
    VehicleDescriptor();
    virtual ~VehicleDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VehicleDescriptor)

VehicleDescriptor::VehicleDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Vehicle)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

VehicleDescriptor::~VehicleDescriptor()
{
    delete[] propertyNames;
}

bool VehicleDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Vehicle *>(obj)!=nullptr;
}

const char **VehicleDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VehicleDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VehicleDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int VehicleDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcEndpoint
        FD_ISEDITABLE,    // FIELD_dstEndpoint
        FD_ISEDITABLE,    // FIELD_vehNumber
        FD_ISEDITABLE,    // FIELD_enterTime
        FD_ISEDITABLE,    // FIELD_totalTransitTime
        FD_ISEDITABLE,    // FIELD_queueEnterTime
        FD_ISEDITABLE,    // FIELD_queueWaitingTime
        FD_ISEDITABLE,    // FIELD_queueExitTime
        FD_ISEDITABLE,    // FIELD_drivingTime
        FD_ISEDITABLE,    // FIELD_hopCount
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *VehicleDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcEndpoint",
        "dstEndpoint",
        "vehNumber",
        "enterTime",
        "totalTransitTime",
        "queueEnterTime",
        "queueWaitingTime",
        "queueExitTime",
        "drivingTime",
        "hopCount",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int VehicleDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcEndpoint") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "dstEndpoint") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "vehNumber") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "enterTime") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "totalTransitTime") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "queueEnterTime") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "queueWaitingTime") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "queueExitTime") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "drivingTime") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *VehicleDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_srcEndpoint
        "string",    // FIELD_dstEndpoint
        "unsigned int",    // FIELD_vehNumber
        "omnetpp::simtime_t",    // FIELD_enterTime
        "omnetpp::simtime_t",    // FIELD_totalTransitTime
        "omnetpp::simtime_t",    // FIELD_queueEnterTime
        "omnetpp::simtime_t",    // FIELD_queueWaitingTime
        "omnetpp::simtime_t",    // FIELD_queueExitTime
        "omnetpp::simtime_t",    // FIELD_drivingTime
        "unsigned int",    // FIELD_hopCount
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **VehicleDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VehicleDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VehicleDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VehicleDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Vehicle'", field);
    }
}

const char *VehicleDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VehicleDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        case FIELD_srcEndpoint: return oppstring2string(pp->getSrcEndpoint());
        case FIELD_dstEndpoint: return oppstring2string(pp->getDstEndpoint());
        case FIELD_vehNumber: return ulong2string(pp->getVehNumber());
        case FIELD_enterTime: return simtime2string(pp->getEnterTime());
        case FIELD_totalTransitTime: return simtime2string(pp->getTotalTransitTime());
        case FIELD_queueEnterTime: return simtime2string(pp->getQueueEnterTime());
        case FIELD_queueWaitingTime: return simtime2string(pp->getQueueWaitingTime());
        case FIELD_queueExitTime: return simtime2string(pp->getQueueExitTime());
        case FIELD_drivingTime: return simtime2string(pp->getDrivingTime());
        case FIELD_hopCount: return ulong2string(pp->getHopCount());
        default: return "";
    }
}

void VehicleDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        case FIELD_srcEndpoint: pp->setSrcEndpoint((value)); break;
        case FIELD_dstEndpoint: pp->setDstEndpoint((value)); break;
        case FIELD_vehNumber: pp->setVehNumber(string2ulong(value)); break;
        case FIELD_enterTime: pp->setEnterTime(string2simtime(value)); break;
        case FIELD_totalTransitTime: pp->setTotalTransitTime(string2simtime(value)); break;
        case FIELD_queueEnterTime: pp->setQueueEnterTime(string2simtime(value)); break;
        case FIELD_queueWaitingTime: pp->setQueueWaitingTime(string2simtime(value)); break;
        case FIELD_queueExitTime: pp->setQueueExitTime(string2simtime(value)); break;
        case FIELD_drivingTime: pp->setDrivingTime(string2simtime(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Vehicle'", field);
    }
}

omnetpp::cValue VehicleDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        case FIELD_srcEndpoint: return pp->getSrcEndpoint();
        case FIELD_dstEndpoint: return pp->getDstEndpoint();
        case FIELD_vehNumber: return (omnetpp::intval_t)(pp->getVehNumber());
        case FIELD_enterTime: return pp->getEnterTime().dbl();
        case FIELD_totalTransitTime: return pp->getTotalTransitTime().dbl();
        case FIELD_queueEnterTime: return pp->getQueueEnterTime().dbl();
        case FIELD_queueWaitingTime: return pp->getQueueWaitingTime().dbl();
        case FIELD_queueExitTime: return pp->getQueueExitTime().dbl();
        case FIELD_drivingTime: return pp->getDrivingTime().dbl();
        case FIELD_hopCount: return (omnetpp::intval_t)(pp->getHopCount());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Vehicle' as cValue -- field index out of range?", field);
    }
}

void VehicleDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        case FIELD_srcEndpoint: pp->setSrcEndpoint(value.stringValue()); break;
        case FIELD_dstEndpoint: pp->setDstEndpoint(value.stringValue()); break;
        case FIELD_vehNumber: pp->setVehNumber(omnetpp::checked_int_cast<unsigned int>(value.intValue())); break;
        case FIELD_enterTime: pp->setEnterTime(value.doubleValue()); break;
        case FIELD_totalTransitTime: pp->setTotalTransitTime(value.doubleValue()); break;
        case FIELD_queueEnterTime: pp->setQueueEnterTime(value.doubleValue()); break;
        case FIELD_queueWaitingTime: pp->setQueueWaitingTime(value.doubleValue()); break;
        case FIELD_queueExitTime: pp->setQueueExitTime(value.doubleValue()); break;
        case FIELD_drivingTime: pp->setDrivingTime(value.doubleValue()); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<unsigned int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Vehicle'", field);
    }
}

const char *VehicleDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr VehicleDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VehicleDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Vehicle *pp = omnetpp::fromAnyPtr<Vehicle>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Vehicle'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

