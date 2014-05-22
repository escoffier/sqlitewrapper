////////////////////////////////////////////////////////////////////////////////
// CppSQLite3 - A C++ wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2004..2007 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
// 
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice, 
// this paragraph and the following two paragraphs appear in all copies, 
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
// V3.0		03/08/2004	-Initial Version for sqlite3
//
// V3.1		16/09/2004	-Implemented getXXXXField using sqlite3 functions
//						-Added CppSQLiteDB3::tableExists()
// V3.2		12/07/2007	-Added tests for new functionality
////////////////////////////////////////////////////////////////////////////////
#include "CppSQLite3.h"
#include <ctime>
#include <iostream>
#include <list>
#include <vector>
#include "gwSharedPtr.hxx"
#include "GateWayConf.h"
#include "GateWayDB.h"
using namespace std;

const char* gszFile = "D:\\test.db";

CppSQLite3DB g_gwdb;

static int createTable(const char * name)
{
	try
	{
		CppSQLite3Buffer bufSQL;
		//bufSQL.format("insert into emp (empname) values (%Q);", "He's bad");
		bufSQL.format("create table %Q(ID char(20), type int, name char[20], manufacturer char[20]);", name);
		return g_gwdb.execDML(bufSQL);
	}
	catch(CppSQLite3Exception & e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
	}
}

static int insertChannel(ChannelInfo * chInfo)
{
	try
	{
		CppSQLite3Buffer bufSQL;
		//bufSQL.format("insert into channel values ('11111', 1, 'David Beckham, 'netposa');");
		bufSQL.format("insert into channel values (%Q, %d, %Q, %Q);",chInfo->ID, chInfo->type, chInfo->name, chInfo->manufacturer);
		return g_gwdb.execDML(bufSQL);
	}
	catch (CppSQLite3Exception& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
	}
}


int main(int argc, char** argv)
{

    try
    {


		{
			remove("D:/gateway.db");
			GateWayCfg cfg;
			GateWayCfg cfg1;
			strncpy(cfg.ID, "12345", 19);
			cfg.type = 100;
			GateWayDB* gateWayDB = GateWayDB::getDBInstance();
	        
			gateWayDB->SetGateWayCfg(&cfg);
			gateWayDB->SetGateWayCfg(&cfg);
			gateWayDB->GetGatewayCfg(&cfg1);

			cout<<cfg1.ID<<" | "<<cfg1.type<<endl;

            cfg.type = 3;
            gateWayDB->ModGateWayCfg(&cfg);

			gateWayDB->GetGatewayCfg(&cfg1);
            
			assert(cfg.type == cfg1.type);
			//**********************************************
			//CppSQLite3Statement st = gateWayDB.compileStatement("insert into channel values (?, ?, ?, ?);");

			for(int i = 0; i< 10; ++i)
			{
				ChannelInfo ch;
				char buf[20] = {0};
				sprintf(buf, "%d", i+ 100);
				strncpy(ch.ID,buf, 19);
				ch.type = 1;

				//->.strncpy(ch.name, "chnname", 19);
				//strncat(ch.name, buf, 19);
				sprintf(ch.name, "chaname-%04d",i);
				strncpy(ch.manufacturer, "netposa", 19);

			    gateWayDB->AddChannelInfo(&ch);
			}

            ChannelInfo ch;
			std::vector<SmartPtr::SharedPtr<ChannelInfo>> pchs;
            cout<<"before modify"<<endl;
			gateWayDB->QueryChannelInfoFromId(pchs,"105");

			for (int i = 0; i < pchs.size(); ++i)
			{
				cout<<pchs[i]->name<<endl;
			}

			strncpy(ch.ID, "105", 19);
			ch.type = 1;
			strncpy(ch.name, "xxxxxx", 19);
			strncpy(ch.manufacturer, "netposa", 19);
			gateWayDB->ModChannelInfo(&ch);
            cout<<"modify chn 105"<<endl;

			
			gateWayDB->QueryChannelInfoFromId(pchs,"105");

			for (int i = 0; i< pchs.size(); ++i)
			{
				cout<<pchs[i]->name<<endl;
			}

			gateWayDB->DelChannelInfobyId("105");

			gateWayDB->QueryChannelInfoFromId(pchs, "105");

			if (pchs.empty())
			{
				cout<<"delete 105 success"<<endl;
			}

		}
		#if 0
		ChannelInfo ch;
		strncpy(ch.ID, "12345", 19);
		ch.type = 1;
		strncpy(ch.name, "chnname-2", 19);
		strncpy(ch.manufacturer, "netposa", 19);
		g_gwdb.open(gszFile);
        //createTable("channel");
        
		//insertChannel(&ch);
        
		CppSQLite3Query query = g_gwdb.execQuery("select * from channel where name = 'chnname-1';");

		cout<<query.numFields()<<endl;
		
		cout<<query.fieldName(0)<<endl;

		//cout<<query.fieldValue("name")<<endl;
		cout << query.fieldValue(0) << "|";
		cout << query.fieldValue(1) << "|" << endl;
		//q.nextRow();
		//cout<<query.fieldValue(0)<<endl;

		CppSQLite3Table table = g_gwdb.getTable("select * from channel;");

		cout<<"row num: "<<table.numRows()<<endl;
        cout<<"feilds num: "<<table.numFields()<<endl;

		for (int i = 0; i< table.numRows();++i)
		{
			table.setRow(i);
			cout<<table.fieldValue("name")<<endl;
		}
		
		CppSQLite3Statement st = g_gwdb.compileStatement("insert into channel values (?, ?, ?, ?);");

		for(int i = 0; i< 100; ++i)
		{
			char buf[32];
			sprintf(buf, "chnname-%d", i);
			st.bind(1, "dddd");
			st.bind(2,i);
			st.bind(3, buf);
			st.bind(4,"netposa");
			st.execDML();
			st.reset();
		}

        GateWayConfig cofig(g_gwdb);

		//cofig.QueryAllChannelInfo();

        int i, fld;A
        time_t tmStart, tmEnd;
        CppSQLite3DB db;

        cout << "SQLite Header Version: " << CppSQLite3DB::SQLiteHeaderVersion() << endl;
        cout << "SQLite Library Version: " << CppSQLite3DB::SQLiteLibraryVersion() << endl;
        cout << "SQLite Library Version Number: " << CppSQLite3DB::SQLiteLibraryVersionNumber() << endl;

        remove(gszFile);
        db.open(gszFile);
    
        cout << endl << "emp table exists=" << (db.tableExists("emp") ? "TRUE":"FALSE") << endl;
        cout << endl << "Creating emp table" << endl;
        db.execDML("create table emp(empno int, empname char(20));");

		cout << endl << "emp table exists=" << (db.tableExists("emp") ? "TRUE":"FALSE") << endl;
        ////////////////////////////////////////////////////////////////////////////////
        // Execute some DML, and print number of rows affected by each one
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "DML tests" << endl;
        int nRows = db.execDML("insert into emp values (7, 'David Beckham');");
        cout << nRows << " rows inserted" << endl;

        nRows = db.execDML("update emp set empname = 'Christiano Ronaldo' where empno = 7;");
        cout << nRows << " rows updated" << endl;

        nRows = db.execDML("delete from emp where empno = 7;");
        cout << nRows << " rows deleted" << endl;

        ////////////////////////////////////////////////////////////////////////////////
        // Transaction Demo
        // The transaction could just as easily have been rolled back
        ////////////////////////////////////////////////////////////////////////////////
        int nRowsToCreate(25000);
        cout << endl << "Transaction test, creating " << nRowsToCreate;
        cout << " rows please wait..." << endl;
        tmStart = time(0);
		cout << "PRE-TXN AUTO COMMIT=" << (db.IsAutoCommitOn() ? "Y" : "N") << endl;
        db.execDML("begin transaction;");
		cout << "IN-TXN AUTO COMMIT=" << (db.IsAutoCommitOn() ? "Y" : "N") << endl;

        for (i = 0; i < nRowsToCreate; i++)
        {
            char buf[128];
            sprintf(buf, "insert into emp values (%d, 'Empname%06d');", i, i);
            db.execDML(buf);
        }

        db.execDML("commit transaction;");
		cout << "POST-TXN AUTO COMMIT=" << (db.IsAutoCommitOn() ? "Y" : "N") << endl;
        tmEnd = time(0);

        ////////////////////////////////////////////////////////////////////////////////
        // Demonstrate CppSQLiteDB::execScalar()
        ////////////////////////////////////////////////////////////////////////////////
        cout << db.execScalar("select count(*) from emp;") << " rows in emp table in ";
        cout << tmEnd-tmStart << " seconds (that was fast!)" << endl;

        ////////////////////////////////////////////////////////////////////////////////
        // Re-create emp table with auto-increment field
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "Auto increment test" << endl;
        db.execDML("drop table emp;");
        db.execDML("create table emp(empno integer primary key, empname char(20));");
        cout << nRows << " rows deleted" << endl;

        for (i = 0; i < 5; i++)
        {
            char buf[128];
            sprintf(buf, "insert into emp (empname) values ('Empname%06d');", i+1);
            db.execDML(buf);
            cout << " primary key: " << (int)db.lastRowId() << endl;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Query data and also show results of inserts into auto-increment field
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "Select statement test" << endl;
        CppSQLite3Query q = db.execQuery("select * from emp order by 1;");

        for (fld = 0; fld < q.numFields(); fld++)
        {
            cout << q.fieldName(fld) << "(" << q.fieldDeclType(fld) << ")|";
        }
        cout << endl;

        while (!q.eof())
        {
            cout << q.fieldValue(0) << "|";
            cout << q.fieldValue(1) << "|" << endl;
            q.nextRow();
        }


        ////////////////////////////////////////////////////////////////////////////////
        // SQLite's printf() functionality. Handles embedded quotes and NULLs
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "SQLite sprintf test" << endl;
        CppSQLite3Buffer bufSQL;
        bufSQL.format("insert into emp (empname) values (%Q);", "He's bad");
        cout << (const char*)bufSQL << endl;
        db.execDML(bufSQL);

        bufSQL.format("insert into emp (empname) values (%Q);", NULL);
        cout << (const char*)bufSQL << endl;
        db.execDML(bufSQL);

        ////////////////////////////////////////////////////////////////////////////////
        // Fetch table at once, and also show how to use CppSQLiteTable::setRow() method
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "getTable() test" << endl;
        CppSQLite3Table t = db.getTable("select * from emp order by 1;");

        for (fld = 0; fld < t.numFields(); fld++)
        {
            cout << t.fieldName(fld) << "|";
        }
        cout << endl;
        for (int row = 0; row < t.numRows(); row++)
        {
            t.setRow(row);
            for (int fld = 0; fld < t.numFields(); fld++)
            {
                if (!t.fieldIsNull(fld))
                    cout << t.fieldValue(fld) << "|";
                else
                    cout << "NULL" << "|";
            }
            cout << endl;
        }


        ////////////////////////////////////////////////////////////////////////////////
        // Test CppSQLiteBinary by storing/retrieving some binary data, checking
        // it afterwards to make sure it is the same
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "Binary data test" << endl;
        db.execDML("create table bindata(desc char(10), data blob);");
        
        unsigned char bin[256];
        CppSQLite3Binary blob;

        for (i = 0; i < sizeof bin; i++)
        {
            bin[i] = i;
        }

        blob.setBinary(bin, sizeof bin);

        bufSQL.format("insert into bindata values ('testing', %Q);", blob.getEncoded());
        db.execDML(bufSQL);
        cout << "Stored binary Length: " << sizeof bin << endl;

        q = db.execQuery("select data from bindata where desc = 'testing';");

        if (!q.eof())
        {
            blob.setEncoded((unsigned char*)q.fieldValue("data"));
            cout << "Retrieved binary Length: " << blob.getBinaryLength() << endl;
        }
		q.finalize();

        const unsigned char* pbin = blob.getBinary();
        for (i = 0; i < sizeof bin; i++)
        {
            if (pbin[i] != i)
            {
                cout << "Problem: i: ," << i << " bin[i]: " << pbin[i] << endl;
            }
        }


		////////////////////////////////////////////////////////////////////////////////
        // Pre-compiled Statements Demo
        ////////////////////////////////////////////////////////////////////////////////
        cout << endl << "Transaction test, creating " << nRowsToCreate*2;
        cout << " rows please wait..." << endl;
        db.execDML("drop table emp;");
        db.execDML("create table emp(empno int, empname char(20));");
        tmStart = time(0);
        db.execDML("begin transaction;");

        cout << endl << "Creating with bind by number" << endl;
        CppSQLite3Statement stmt = db.compileStatement("insert into emp values (?, ?);");
        for (i = 0; i < nRowsToCreate; i++)
        {
            char buf[16];
            sprintf(buf, "EmpName%06d", i);
            stmt.bind(1, i);
            stmt.bind(2, buf);
            stmt.execDML();
            stmt.reset();
        }

        cout << endl << "Creating with bind by name" << endl;
		CppSQLite3Statement stmt2 = db.compileStatement("insert into emp values (:p1, @p2);");
        for (i = 0; i < nRowsToCreate; i++)
        {
            char buf[16];
            sprintf(buf, "EmpName%06d", i);
			stmt2.bind(":p1", i);
			stmt2.bind("@p2", buf);
            stmt2.execDML();
            stmt2.reset();
        }

		db.execDML("commit transaction;");
        tmEnd = time(0);

        cout << db.execScalar("select count(*) from emp;") << " rows in emp table in ";
        cout << tmEnd-tmStart << " seconds (that was even faster!)" << endl;
        cout << endl << "End of tests" << endl;
#endif
    }
    catch (CppSQLite3Exception& e)
    {
        cerr << e.errorCode() << ":" << e.errorMessage() << endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Loop until user enters q or Q
    ////////////////////////////////////////////////////////////////////////////////
    char c(' ');

    while (c != 'q' && c != 'Q')
    {
        cout << "Press q then enter to quit: ";
        cin >> c;
    }
    return 0;
}

