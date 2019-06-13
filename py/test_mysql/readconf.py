#!/usr/bin/python
import configparser
import os

class ReadConf():
    def __init__(self, filename):
        self.filename = filename
    def readcnf(self):
        cf = configparser.ConfigParser()
        cf.read(self.filename)
        my_db = []
        my_db.append(cf.get("db", "db_host"))
        my_db.append(cf.get("db", "db_user"))
        my_db.append(cf.get("db", "db_pass"))
        my_db.append(cf.get("db", "db_name"))
        return my_db

