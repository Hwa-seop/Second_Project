# db_setup.py
import sqlite3

conn = sqlite3.connect("parking.db")
cur = conn.cursor()

cur.execute("""
CREATE TABLE IF NOT EXISTS parking_log (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    zone TEXT,
    action TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
)
""")

conn.commit()
conn.close()
