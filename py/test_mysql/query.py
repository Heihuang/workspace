#!/usr/bin/python

import threading
import pymysql

class MyQuery(threading.Thread):
    def __init__(self, host, user, passwd, dbname, tid):
        threading.Thread.__init__(self)
        self.host = host
        self.user = user
        self.passwd = passwd
        self.dbname = dbname
        self.tid = tid

    def addsql(self, sql):
        self.sql = sql

    def run(self):
        self._opendb()
        self._execsql(self.sql)
        self._closedb()

    def _opendb(self):
        self.db = pymysql.connect(self.host, self.user, self.passwd, self.dbname)

    def _execsql(self,sql):
        try:
            cursor = self.db.cursor()
            cursor.execute(sql)
            results = cursor.fetchall()
            for row in results:
                print "tid:%d  row:%s" %(self.tid,row)
        except:
            print "query error! sql:%s" %sql

    def _closedb(self):
        self.db.close()



