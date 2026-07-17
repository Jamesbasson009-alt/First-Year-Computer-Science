from datetime import datetime, timedelta
from dotenv import load_dotenv
import os
import pytz
from twilio.rest import Client
from database import get_connection

load_dotenv()

account_sid = os.getenv('TWILIO_ACCOUNT_SID')
auth_token = os.getenv('TWILIO_AUTH_TOKEN')
twilio_number = os.getenv('TWILIO_WHATSAPP_NUMBER')

client = Client(account_sid, auth_token)

SAST = pytz.timezone('Africa/Johannesburg')

def check_reminders():
    conn = get_connection()
    c = conn.cursor()

    now_sast = datetime.now(SAST)
    tomorrow = (now_sast + timedelta(days=1)).strftime('%Y-%m-%d')

    c.execute('''
        SELECT id, practice_name, patient_name, patient_number, appointment_time
        FROM appointments
        WHERE appointment_time LIKE %s AND reminder_sent = 0
    ''', (f'{tomorrow}%',))

    due = c.fetchall()

    if not due:
        print("No reminders due.")

    for appt in due:
        appt_id, practice, patient, number, time = appt
        appt_hour = time.split(' ')[1]
        message_body = f"Hi {patient}, reminder: you have an appointment with {practice} tomorrow at {appt_hour}. Reply 1 to confirm or 2 to reschedule."

        message = client.messages.create(
            from_=twilio_number,
            body=message_body,
            to=f'whatsapp:{number}'
        )

        print(f"Sent to {number}, SID: {message.sid}")

        c.execute('UPDATE appointments SET reminder_sent = 1 WHERE id = %s', (appt_id,))

    conn.commit()

    # Clean up appointments that are clearly in the past. A one-day grace
    # period is kept so a patient can still reply on WhatsApp about an
    # appointment that happened earlier today before it's removed.
    cutoff = (now_sast - timedelta(days=1)).strftime('%Y-%m-%d %H:%M')
    c.execute('''
        DELETE FROM appointments
        WHERE appointment_time < %s
    ''', (cutoff,))
    deleted_count = c.rowcount
    conn.commit()

    if deleted_count:
        print(f"Cleared {deleted_count} past appointment(s).")
    else:
        print("No past appointments to clear.")

    conn.close()

if __name__ == '__main__':
    check_reminders()
