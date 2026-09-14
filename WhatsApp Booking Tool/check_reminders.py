from datetime import datetime, timedelta
from dotenv import load_dotenv
import os
import pytz
from twilio.rest import Client
import database

load_dotenv()

account_sid = os.getenv('TWILIO_ACCOUNT_SID')
auth_token = os.getenv('TWILIO_AUTH_TOKEN')
twilio_number = os.getenv('TWILIO_WHATSAPP_NUMBER')

client = Client(account_sid, auth_token)

SAST = pytz.timezone('Africa/Johannesburg')

def check_reminders():
    now_sast = datetime.now(SAST)
    tomorrow = (now_sast + timedelta(days=1)).strftime('%Y-%m-%d')

    due = database.get_due_reminders(tomorrow)

    if not due:
        print("No reminders due.")

    for appt in due:
        appt_hour = appt['appointment_time'].split(' ')[1]
        message_body = (
            f"Hi {appt['patient_name']}, reminder: you have an appointment with "
            f"{appt['practice_name']} tomorrow at {appt_hour}. Reply 1 to confirm or 2 to reschedule."
        )

        message = client.messages.create(
            from_=twilio_number,
            body=message_body,
            to=f"whatsapp:{appt['patient_number']}"
        )

        print(f"Sent to {appt['patient_number']}, SID: {message.sid}")
        database.mark_reminder_sent(appt['id'])

    cutoff = (now_sast - timedelta(days=1)).strftime('%Y-%m-%d %H:%M')
    deleted_count = database.delete_appointments_before(cutoff)

    if deleted_count:
        print(f"Cleared {deleted_count} past appointment(s).")
    else:
        print("No past appointments to clear.")

if __name__ == '__main__':
    # Running this file standalone (e.g. from a GitHub Actions cron) no
    # longer works: appointment data now lives only in the memory of the
    # running Flask process on Render, not in an external database a
    # separate script can reach. Point your GitHub Actions workflow at the
    # deployed endpoint instead, e.g.:
    #   curl "https://whatsapp-booking-osmf.onrender.com/run-reminders?key=$REMINDER_SECRET"
    check_reminders()
