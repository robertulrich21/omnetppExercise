//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Customer.msg.
//

#ifndef __CUSTOMER_M_H
#define __CUSTOMER_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class Customer;
/**
 * Class generated from <tt>Customer.msg:19</tt> by opp_msgtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet Customer
 * {
 *     int id;
 *     int items;
 *     simtime_t waitingTime;
 * }
 * </pre>
 */
class Customer : public ::omnetpp::cPacket
{
  protected:
    int id = 0;
    int items = 0;
    omnetpp::simtime_t waitingTime = SIMTIME_ZERO;

  private:
    void copy(const Customer& other);

  protected:
    bool operator==(const Customer&) = delete;

  public:
    Customer(const char *name=nullptr, short kind=0);
    Customer(const Customer& other);
    virtual ~Customer();
    Customer& operator=(const Customer& other);
    virtual Customer *dup() const override {return new Customer(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getId() const;
    virtual void setId(int id);

    virtual int getItems() const;
    virtual void setItems(int items);

    virtual omnetpp::simtime_t getWaitingTime() const;
    virtual void setWaitingTime(omnetpp::simtime_t waitingTime);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Customer& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Customer& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline Customer *fromAnyPtr(any_ptr ptr) { return check_and_cast<Customer*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __CUSTOMER_M_H

