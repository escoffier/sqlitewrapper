#ifndef _GateWayDB_H_
#define _GateWayDB_H_

#include <vector>
#include "gwSharedPtr.hxx"
#include "GateWayConf.h"

class CppSQLite3DB;
struct GateWayCfg;
class vector;
class GateWayDB
{
public:

	static GateWayDB *getDBInstance();
	~GateWayDB();

	void GetGatewayCfg(GateWayCfg *cfg) const;
	void SetGateWayCfg(GateWayCfg *cfg);
    void ModGateWayCfg(GateWayCfg *cfg);

	void QueryAllChannelInfo(std::vector<SmartPtr::SharedPtr<ChannelInfo>> &ChannelInfoList);
	void QueryChannelInfoFromId(std::vector<SmartPtr::SharedPtr<ChannelInfo>> &ChannelInfoList, const char *ID);

	int AddChannelInfo(ChannelInfo* chnInfo);
	int ModChannelInfo(ChannelInfo* chnInfo);
    int DelChannelInfobyId(const char* Id);
	
private:
	GateWayDB(const char *dbFileName);
	GateWayDB(const GateWayDB& db);

	SmartPtr::SharedPtr<CppSQLite3DB> db;
	std::string dbName;

	static GateWayDB *instance;
};

#endif
