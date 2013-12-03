/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef CockleburServ_H
#define CockleburServ_H

#include <thrift/TDispatchProcessor.h>
#include "leader_election_types.h"



class CockleburServIf {
 public:
  virtual ~CockleburServIf() {}
  virtual void getHostStat(HostStat& _return) = 0;
  virtual void ping(std::string& _return, const std::string& message) = 0;
  virtual void ping2(HBMSG& _return, const HBMSG& message) = 0;
};

class CockleburServIfFactory {
 public:
  typedef CockleburServIf Handler;

  virtual ~CockleburServIfFactory() {}

  virtual CockleburServIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(CockleburServIf* /* handler */) = 0;
};

class CockleburServIfSingletonFactory : virtual public CockleburServIfFactory {
 public:
  CockleburServIfSingletonFactory(const boost::shared_ptr<CockleburServIf>& iface) : iface_(iface) {}
  virtual ~CockleburServIfSingletonFactory() {}

  virtual CockleburServIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(CockleburServIf* /* handler */) {}

 protected:
  boost::shared_ptr<CockleburServIf> iface_;
};

class CockleburServNull : virtual public CockleburServIf {
 public:
  virtual ~CockleburServNull() {}
  void getHostStat(HostStat& /* _return */) {
    return;
  }
  void ping(std::string& /* _return */, const std::string& /* message */) {
    return;
  }
  void ping2(HBMSG& /* _return */, const HBMSG& /* message */) {
    return;
  }
};


class CockleburServ_getHostStat_args {
 public:

  CockleburServ_getHostStat_args() {
  }

  virtual ~CockleburServ_getHostStat_args() throw() {}


  bool operator == (const CockleburServ_getHostStat_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const CockleburServ_getHostStat_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_getHostStat_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class CockleburServ_getHostStat_pargs {
 public:


  virtual ~CockleburServ_getHostStat_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_getHostStat_result__isset {
  _CockleburServ_getHostStat_result__isset() : success(false) {}
  bool success;
} _CockleburServ_getHostStat_result__isset;

class CockleburServ_getHostStat_result {
 public:

  CockleburServ_getHostStat_result() {
  }

  virtual ~CockleburServ_getHostStat_result() throw() {}

  HostStat success;

  _CockleburServ_getHostStat_result__isset __isset;

  void __set_success(const HostStat& val) {
    success = val;
  }

  bool operator == (const CockleburServ_getHostStat_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const CockleburServ_getHostStat_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_getHostStat_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_getHostStat_presult__isset {
  _CockleburServ_getHostStat_presult__isset() : success(false) {}
  bool success;
} _CockleburServ_getHostStat_presult__isset;

class CockleburServ_getHostStat_presult {
 public:


  virtual ~CockleburServ_getHostStat_presult() throw() {}

  HostStat* success;

  _CockleburServ_getHostStat_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _CockleburServ_ping_args__isset {
  _CockleburServ_ping_args__isset() : message(false) {}
  bool message;
} _CockleburServ_ping_args__isset;

class CockleburServ_ping_args {
 public:

  CockleburServ_ping_args() : message() {
  }

  virtual ~CockleburServ_ping_args() throw() {}

  std::string message;

  _CockleburServ_ping_args__isset __isset;

  void __set_message(const std::string& val) {
    message = val;
  }

  bool operator == (const CockleburServ_ping_args & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const CockleburServ_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class CockleburServ_ping_pargs {
 public:


  virtual ~CockleburServ_ping_pargs() throw() {}

  const std::string* message;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_ping_result__isset {
  _CockleburServ_ping_result__isset() : success(false) {}
  bool success;
} _CockleburServ_ping_result__isset;

class CockleburServ_ping_result {
 public:

  CockleburServ_ping_result() : success() {
  }

  virtual ~CockleburServ_ping_result() throw() {}

  std::string success;

  _CockleburServ_ping_result__isset __isset;

  void __set_success(const std::string& val) {
    success = val;
  }

  bool operator == (const CockleburServ_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const CockleburServ_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_ping_presult__isset {
  _CockleburServ_ping_presult__isset() : success(false) {}
  bool success;
} _CockleburServ_ping_presult__isset;

class CockleburServ_ping_presult {
 public:


  virtual ~CockleburServ_ping_presult() throw() {}

  std::string* success;

  _CockleburServ_ping_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _CockleburServ_ping2_args__isset {
  _CockleburServ_ping2_args__isset() : message(false) {}
  bool message;
} _CockleburServ_ping2_args__isset;

class CockleburServ_ping2_args {
 public:

  CockleburServ_ping2_args() {
  }

  virtual ~CockleburServ_ping2_args() throw() {}

  HBMSG message;

  _CockleburServ_ping2_args__isset __isset;

  void __set_message(const HBMSG& val) {
    message = val;
  }

  bool operator == (const CockleburServ_ping2_args & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const CockleburServ_ping2_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_ping2_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class CockleburServ_ping2_pargs {
 public:


  virtual ~CockleburServ_ping2_pargs() throw() {}

  const HBMSG* message;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_ping2_result__isset {
  _CockleburServ_ping2_result__isset() : success(false) {}
  bool success;
} _CockleburServ_ping2_result__isset;

class CockleburServ_ping2_result {
 public:

  CockleburServ_ping2_result() {
  }

  virtual ~CockleburServ_ping2_result() throw() {}

  HBMSG success;

  _CockleburServ_ping2_result__isset __isset;

  void __set_success(const HBMSG& val) {
    success = val;
  }

  bool operator == (const CockleburServ_ping2_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const CockleburServ_ping2_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CockleburServ_ping2_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _CockleburServ_ping2_presult__isset {
  _CockleburServ_ping2_presult__isset() : success(false) {}
  bool success;
} _CockleburServ_ping2_presult__isset;

class CockleburServ_ping2_presult {
 public:


  virtual ~CockleburServ_ping2_presult() throw() {}

  HBMSG* success;

  _CockleburServ_ping2_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class CockleburServClient : virtual public CockleburServIf {
 public:
  CockleburServClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  CockleburServClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getHostStat(HostStat& _return);
  void send_getHostStat();
  void recv_getHostStat(HostStat& _return);
  void ping(std::string& _return, const std::string& message);
  void send_ping(const std::string& message);
  void recv_ping(std::string& _return);
  void ping2(HBMSG& _return, const HBMSG& message);
  void send_ping2(const HBMSG& message);
  void recv_ping2(HBMSG& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class CockleburServProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<CockleburServIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (CockleburServProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getHostStat(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping2(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  CockleburServProcessor(boost::shared_ptr<CockleburServIf> iface) :
    iface_(iface) {
    processMap_["getHostStat"] = &CockleburServProcessor::process_getHostStat;
    processMap_["ping"] = &CockleburServProcessor::process_ping;
    processMap_["ping2"] = &CockleburServProcessor::process_ping2;
  }

  virtual ~CockleburServProcessor() {}
};

class CockleburServProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  CockleburServProcessorFactory(const ::boost::shared_ptr< CockleburServIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< CockleburServIfFactory > handlerFactory_;
};

class CockleburServMultiface : virtual public CockleburServIf {
 public:
  CockleburServMultiface(std::vector<boost::shared_ptr<CockleburServIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~CockleburServMultiface() {}
 protected:
  std::vector<boost::shared_ptr<CockleburServIf> > ifaces_;
  CockleburServMultiface() {}
  void add(boost::shared_ptr<CockleburServIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getHostStat(HostStat& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getHostStat(_return);
    }
    ifaces_[i]->getHostStat(_return);
    return;
  }

  void ping(std::string& _return, const std::string& message) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping(_return, message);
    }
    ifaces_[i]->ping(_return, message);
    return;
  }

  void ping2(HBMSG& _return, const HBMSG& message) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping2(_return, message);
    }
    ifaces_[i]->ping2(_return, message);
    return;
  }

};



#endif
