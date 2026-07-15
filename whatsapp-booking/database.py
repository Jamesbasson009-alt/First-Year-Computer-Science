import os
import psycopg2
from dotenv import load_dotenv

def get_connection():
    return psycopg2.connect(os.getenv('DATABASE_URL'))

def init_db():
    conn = get_connection()
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS appointments (
            id SERIAL PRIMARY KEY,
            practice_name TEXT NOT NULL,
            patient_name TEXT NOT NULL,
            patient_number TEXT NOT NULL,
            appointment_time TEXT NOT NULL,
            reminder_sent INTEGER DEFAULT 0
        )
    ''')
    conn.commit()
    conn.close()

if __name__ == '__main__':
    init_db()
    print("Database created.")
