#include "GateWayConf.h"
#include "CppSQLite3.h"
#include <iostream>

void GateWayConfig::parse(CppSQLite3Table * table)
{
	if (!chnInfoList.empty())
	{
		chnInfoList.clear();
	}

    for (int i = 0; i< table->numRows(); ++i)
    {
		SmartPtr::SharedPtr<ChannelInfo> chn(new ChannelInfo);
		//chn->ID = table->fieldValue(0);
		chn->type = table->getIntField(1, 0);//table->fieldValue(1);
		std::cout<<chn->type<<std::endl;
		//chn->name = table->fieldValue(2);
		//chn->manufacturer = table->fieldValue(3);

		chnInfoList.push_back(chn);
    }

	return;
}

GateWayConfig::GateWayConfig(CppSQLite3DB & db)
{
	CppSQLite3Table table = db.getTable("select * from channel;");
	parse(&table);
}

