#!/usr/bin/python
import os
import query
import readconf

file_name = "mydb.conf"
cf = readconf.ReadConf(file_name)
db_info = cf.readcnf()
if len(db_info) != 4:
    print "db_info error!"
    os._exit()
else:
    print db_info[0]
    print db_info[1]
    print db_info[2]
    print db_info[3]

mydb = query.MyQuery(db_info[0], db_info[1], db_info[2], db_info[3], 1)
sql = "select * from testpy"
mydb.addsql(sql)

mydb.start()
mydb.join()
