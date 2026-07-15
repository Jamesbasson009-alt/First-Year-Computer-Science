from flask import Flask, request, render_template_string
from twilio.twiml.messaging_response import MessagingResponse
import sqlite3

app = Flask(__name__)

FORM_HTML = '''
<!DOCTYPE html>
<html>
<head>
    <title>Add Appointment</title>
    <style>
        body { font-family: sans-serif; max-width: 400px; margin: 50px auto; }
        input { width: 100%; padding: 8px; margin: 6px 0 16px 0; box-sizing: border-box; }
        label { font-weight: bold; }
        button { padding: 10px 20px; background: #25D366; color: white; border: none; border-radius: 4px; cursor: pointer; }
    </style>
</head>
<body>
    <h2>Add New Appointment</h2>
    {% if success %}
        <p style="color: green;">Appointment added successfully!</p>
    {% endif %}
    <form method="POST">
        <label>Practice Name</label>
        <input type="text" name="practice_name" required>

        <label>Patient Name</label>
        <input type="text" name="patient_name" required>

        <label>Patient WhatsApp Number (with +27...)</label>
        <input type="text" name="patient_number" required placeholder="+27821234567">

        <label>Appointment Date & Time</label>
        <input type="datetime-local" name="appointment_time" required>

        <button type="submit">Add Appointment</button>
    </form>
</body>
</html>
'''

@app.route('/add', methods=['GET', 'POST'])
def add_appointment():
    success = False
    if request.method == 'POST':
        practice_name = request.form['practice_name']
        patient_name = request.form['patient_name']
        patient_number = request.form['patient_number']
        appointment_time = request.form['appointment_time'].replace('T', ' ')

        conn = sqlite3.connect('appointments.db')
        c = conn.cursor()
        c.execute('''
            INSERT INTO appointments (practice_name, patient_name, patient_number, appointment_time)
            VALUES (?, ?, ?, ?)
        ''', (practice_name, patient_name, patient_number, appointment_time))
        conn.commit()
        conn.close()
        success = True

    return render_template_string(FORM_HTML, success=success)

@app.route('/whatsapp', methods=['POST'])
def whatsapp_reply():
    incoming_msg = request.values.get('Body', '').strip()
    from_number = request.values.get('From', '').replace('whatsapp:', '')

    resp = MessagingResponse()
    msg = resp.message()

    conn = sqlite3.connect('appointments.db')
    c = conn.cursor()

    c.execute('''
        SELECT id, practice_name, appointment_time
        FROM appointments
        WHERE patient_number = ?
        ORDER BY id DESC LIMIT 1
    ''', (from_number,))
    appt = c.fetchone()

    if not appt:
        msg.body("We couldn't find an appointment linked to this number. Please contact the practice directly.")
        conn.close()
        return str(resp)

    appt_id, practice, appt_time = appt

    if incoming_msg == '1':
        msg.body(f"Great, your appointment with {practice} on {appt_time} is confirmed. See you then!")
    elif incoming_msg == '2':
        msg.body(f"No problem — please call {practice} directly to reschedule your appointment.")
    else:
        msg.body("Sorry, I didn't understand that. Reply 1 to confirm or 2 to reschedule your appointment.")

    conn.close()
    return str(resp)

@app.route('/run-reminders')
def run_reminders():
    from check_reminders import check_reminders
    check_reminders()
    return "Reminders checked!"

if __name__ == '__main__':
    app.run(debug=True, port=5000)
