# parking_stats.py
import sqlite3

conn = sqlite3.connect("parking.db")
cur = conn.cursor()

print("⏱️ 최근 6시간 시간대별 주차 대수 (PARKED 기준)")

cur.execute("""
SELECT strftime('%Y-%m-%d %H:00', timestamp) AS hour_block,
       COUNT(*) AS num_parked
FROM parking_log
WHERE action = 'PARKED'
GROUP BY hour_block
ORDER BY hour_block DESC
LIMIT 6
""")

rows = cur.fetchall()
for row in rows:
    print(f"{row[0]} → {row[1]}대 주차됨")

conn.close()
