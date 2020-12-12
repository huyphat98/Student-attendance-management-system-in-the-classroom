##tao khohang.db
import sqlite3

conn = sqlite3.connect('/home/pi/sqlite/update.db')

print ("Opened database successfully")

#conn.execute('''CREATE TABLE KHOHANG(
#        linh_kien      TEXT PRIMARY KEY NOT NULL,
#        loai_linh_kien TEXT NOT NULL,
#        gia            INTEGER NOT NULL);''')
#print ("Table created successfully")

#conn.execute("INSERT INTO Quanlylop (id, Position) \
#      VALUES ('123a', 'abc')")

#conn.execute("INSERT INTO KHOHANG (linh_kien, loai_linh_kien, gia) \
#      VALUES ('bc337', 'transistor', 150 )")

#conn.execute("INSERT INTO KHOHANG (linh_kien, loai_linh_kien, gia) \
#      VALUES ('2n3904', 'transistor', 200 )")

#conn.commit()
#print ("Records created successfully")
conn.execute("UPDATE Quanlylop SET id = '098123', Position = 'update' WHERE id = 0")
conn.commit()

cursor = conn.execute("SELECT id, Position from Quanlylop")
for row in cursor:
   print ("id = ", row[0])
   print ("Position = ", row[1])
   #print ("Position = ", row[2], "\n")

print ("Operation done successfully")

conn.close()