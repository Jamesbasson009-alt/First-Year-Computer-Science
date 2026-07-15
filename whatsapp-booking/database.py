import sqlite3

def init_db():
    conn = sqlite3.connect('appointments.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS appointments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
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
