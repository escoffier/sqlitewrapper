#include "GateWayDB.h"
#include "CppSQLite3.h"
#include <iostream>

GateWayDB * GateWayDB::instance = NULL;

GateWayDB * GateWayDB::getDBInstance()
{
	if(NULL == instance)
	{
		return new GateWayDB("D:/gateway.db");
	}
    return instance;

}

GateWayDB::GateWayDB(const GateWayDB& db)
{

}

GateWayDB::~GateWayDB()
{

}

GateWayDB::GateWayDB(const char *dbFileName):
db(new CppSQLite3DB)
{
	db->open(dbFileName);
	db->execDML("create table channelinfo (ID char(20), type int, name char[20], manufacturer char[20], primary key (ID));");
	db->execDML("create table gatewaycfg (ID char(20), type int, primary key (ID));");
	db->execDML("create table parentcfg (ID char(20), type int, primary key (ID));");
	if(db->tableExists("channelinfo"))
	{
		std::cout<<"table channelinfo already exist"<<std::endl;
	}
}


void GateWayDB::GetGatewayCfg(GateWayCfg *cfg) const
{
	CppSQLite3Query gateWayCfgRecord = db->execQuery("select * from gatewaycfg");
	if (!gateWayCfgRecord.eof())
	{
		strncpy(cfg->ID, gateWayCfgRecord.fieldValue(0), sizeof(cfg->ID) -1);
		cfg->type = gateWayCfgRecord.getIntField(1, -1);
	}
	return;
}

void GateWayDB::SetGateWayCfg(GateWayCfg *cfg)
{
	CppSQLite3Buffer bufSQL;
	//主键重复不会插入记录
	bufSQL.format("insert or ignore into gatewaycfg values (%Q, %d);", cfg->ID, cfg->type);
	db->execDML(bufSQL);
	return;
}

void GateWayDB::ModGateWayCfg(GateWayCfg *cfg)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("update gatewaycfg set ID=%Q, type=%d where ID=%Q;", cfg->ID, cfg->type, cfg->ID);
	db->execDML(bufSQL);
	return;
}

void GateWayDB::QueryAllChannelInfo(std::vector<SmartPtr::SharedPtr<ChannelInfo>> &ChannelInfoList)
{
	CppSQLite3Query chnInfoRecord = db->execQuery("select * from channelinfo;");
	while (!chnInfoRecord.eof())
	{
		SmartPtr::SharedPtr<ChannelInfo> info(new ChannelInfo);
		strncpy(info->ID, chnInfoRecord.fieldValue(0), sizeof(info->ID) -1);
		info->type = chnInfoRecord.getIntField(1, -1);
		strncpy(info->name, chnInfoRecord.fieldValue(2), sizeof(info->ID) -1);
		strncpy(info->manufacturer, chnInfoRecord.fieldValue(3), sizeof(info->ID) -1);
		ChannelInfoList.push_back(info);
		chnInfoRecord.nextRow();
	}
	return;
}

void GateWayDB::QueryChannelInfoFromId(std::vector<SmartPtr::SharedPtr<ChannelInfo>> &ChannelInfoList, const char *ID)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("select * from channelinfo where ID=%Q;", ID);
	CppSQLite3Query chnInfoRecord = db->execQuery(bufSQL);
	//清空容器
    ChannelInfoList.clear();
	while (!chnInfoRecord.eof())
	{
		SmartPtr::SharedPtr<ChannelInfo> info(new ChannelInfo);
		strncpy(info->ID, chnInfoRecord.fieldValue(0), sizeof(info->ID) -1);
		info->type = chnInfoRecord.getIntField(1, -1);
		strncpy(info->name, chnInfoRecord.fieldValue(2), sizeof(info->ID) -1);
		strncpy(info->manufacturer, chnInfoRecord.fieldValue(3), sizeof(info->ID) -1);

		ChannelInfoList.push_back(info);
		chnInfoRecord.nextRow();
	}
	return;
}

int GateWayDB::AddChannelInfo(ChannelInfo* chnInfo)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("insert into channelinfo values (%Q, %d, %Q, %Q);", 
		          chnInfo->ID, chnInfo->type, chnInfo->name, chnInfo->manufacturer);
	std::cout<<bufSQL<<std::endl;
	return db->execDML(bufSQL);
}

int GateWayDB::ModChannelInfo(ChannelInfo* chnInfo)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("update channelinfo set ID=%Q, type=%d, name=%Q, manufacturer=%Q where ID=%Q;", 
		           chnInfo->ID, chnInfo->type, chnInfo->name, chnInfo->manufacturer, chnInfo->ID);

	return db->execDML(bufSQL);
}

int GateWayDB::DelChannelInfobyId(const char* Id)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("delete from channelinfo where ID=%Q;", Id);
	return db->execDML(bufSQL);
}