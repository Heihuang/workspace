from  __future__ import unicode_literals
import ipdb,sys

db = ipdb.City("ipipfree.ipdb")
# db.reload("/path/to/city.ipv4.ipdb") # update ipdb database file reload data
print db.is_ipv4(), db.is_ipv6()
print db.languages() # support language
print db.fields() #  support fields
print db.build_time() #  build database time
#print db.find("1.1.1.1", "CN") #  query ip return array
# print(db.find(u"1.1.1.1", "CN")) #  Python 2.7
#print db.find_map("123.124.16.83", "CN") #  query ip return dict
print db.find_info("123.124.16.83", "CN").region_name
print db.find_info("123.124.16.83", "CN").city_name

