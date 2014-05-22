#ifndef _GATEWAYCONF_H_
#define _GATEWAYCONF_H_
#include <list>
#include "gwSharedPtr.hxx"

class CppSQLite3Table;
class CppSQLite3DB;
struct ChannelInfo
{
	
	char ID[20];
	int type;
	char name[20];
	char manufacturer[20];


};

struct GateWayCfg
{

	char ID[20];
	int type;
};


class GateWayConfig
{
public:
	GateWayConfig(CppSQLite3DB & db);
	//GateWayConfig(const char *info);
	~GateWayConfig() { };

	void parse(CppSQLite3Table * table);
    //void QueryAllChannelInfo() const;
private:
	void setChannelInfo(ChannelInfo* );
	std::list<SmartPtr::SharedPtr<ChannelInfo>> chnInfoList;
};



#endif 
