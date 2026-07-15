import sqlite3
from datetime import datetime, timedelta

conn = sqlite3.connect('appointments.db')
c = conn.cursor()

tomorrow = (datetime.now() + timedelta(days=1)).strftime('%Y-%m-%d %H:%M')

c.execute('''
    INSERT INTO appointments (practice_name, patient_name, patient_number, appointment_time)
    VALUES (?, ?, ?, ?)
''', ('Dr. Smith Dental', 'John Doe', '+2634450456', tomorrow))

conn.commit()
conn.close()
print(f"Added test appointment for tomorrow: {tomorrow}")
