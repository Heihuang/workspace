#ifndef MYSQL_DAO_H
#define MYSQL_DAO_H
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

class MySqlDao
{
public:
	~MySqlDao();
	void Init()
	{
		//conn_ = mysql_init((MYSQL*)0);
		try
		{
			conn_ = mysql_init(conn_);
		}
		catch (...)
		{
			printf("mysql init failed!\n");
		}
	}

    bool RealConnect(const char* db, const char* host, int port, const char* user, const char* password)
	{
		if (conn_ == NULL)
		{
            printf("conn_ is NULL\n");
            return false;
		}
		try
		{
			if (!mysql_real_connect(conn_, host, user, password, db, port, NULL, 0))
			{
				printf("connetc failed\n");
				return false;
			}
			if (!mysql_select_db(conn_, db))
			{
				printf("Select successfully the database\n");
				conn_->reconnect = 1;
				char * query = "set names \'GBK\'";
				int rt = mysql_real_query(conn_, query, strlen(query));
				if (rt)
				{
					printf("Error making query\n");
				}
				else
				{
					printf("executed\n");
				}
			}
		}
		catch (...)
		{
			printf("mysql_real_connect exception\n");
			return false;
		}
        return true;
	}

    bool SqlExecute(const char* sql)
	{
		try
		{
			int rt = mysql_real_query(conn_, sql, strlen(sql));
			if (rt)
			{
				printf("Error making query = %d\n", mysql_errno(conn_));
				printf("sql = %s\n", sql);
				return false;
			}
			res_ = mysql_store_result(conn_);//将结果保存在res结构体中
		}
		catch (...)
		{
			printf("mysql_real_query exception \n");
			return false;
		}
        return true;
	}

	MYSQL_ROW Next()
	{
		if (!res_)
		{
            printf("Empty set!\n");
            return NULL;
		}
		try
		{
			row_ = mysql_fetch_row(res_);
		}
		catch (...)
		{
			printf("mysql_fetch_row exception\n");
		}
		
		return row_;
	}
	char* getRes(const int& index)
	{
        char * temp = NULL;
		try
		{
			if (mysql_num_fields(res_) == 0)
			{
				return NULL;
			}
			int i = mysql_num_fields(res_);
			if (index < mysql_num_fields(res_) && row_[index] != NULL)
			{
				temp = row_[index];
			}
		}
		catch (...)
		{
			printf("mysql_num_fields exception\n");
		}
		return temp;
	}

	void FlushDataSet()
	{
		try
		{
			mysql_free_result(res_);
		}
		catch (...)
		{
			printf("mysql_free_result exception");
		}
	}
	void UnInit()
	{
		try
		{
			mysql_close(conn_);
		}
		catch (...)
		{
			printf("mysql_close exception\n");
		}
		
	}

	static MySqlDao& getInstance()
	{
		static MySqlDao dao;
		return dao;
	}

private:
	MySqlDao(){}
	MySqlDao(const MySqlDao& other);
	MySqlDao& operator=(const MySqlDao& other);
private:
	MYSQL *conn_;
	MYSQL_RES *res_;
	MYSQL_ROW row_;
};


MySqlDao::~MySqlDao()
{
}


#endif
