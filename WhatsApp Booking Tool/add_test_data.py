from datetime import datetime, timedelta
from database import get_connection

conn = get_connection()
c = conn.cursor()

# Use the same paramstyle as the app (Postgres / psycopg2 expects %s)
# Insert a test appointment for tomorrow
tomorrow = (datetime.now() + timedelta(days=1)).strftime('%Y-%m-%d %H:%M')

c.execute('''
    INSERT INTO appointments (practice_name, patient_name, patient_number, appointment_time)
    VALUES (%s, %s, %s, %s)
''', ('Dr. Smith Dental', 'John Doe', '+2634450456', tomorrow))

conn.commit()
conn.close()
print(f"Added test appointment for tomorrow: {tomorrow}")
