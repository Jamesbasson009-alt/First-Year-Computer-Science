import os
import re
from datetime import datetime
import pytz
from flask import Flask, request, render_template_string, jsonify
from twilio.twiml.messaging_response import MessagingResponse
from twilio.rest import Client
from dotenv import load_dotenv
from database import get_connection

load_dotenv()

app = Flask(__name__)

twilio_client = Client(os.getenv('TWILIO_ACCOUNT_SID'), os.getenv('TWILIO_AUTH_TOKEN'))
TWILIO_WHATSAPP_NUMBER = os.getenv('TWILIO_WHATSAPP_NUMBER')
SAST = pytz.timezone('Africa/Johannesburg')

FORM_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Add Appointment</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
<style>
  :root {
    --ink: #16302B;
    --teal: #0F6B5C;
    --teal-dark: #0B5045;
    --bg: #FAF8F3;
    --card: #FFFFFF;
    --line: #E4E0D6;
    --muted: #6B7570;
    --success: #25D366;
    --success-bg: #EAFBF1;
    --error: #C0503E;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0;
    min-height: 100vh;
    background: var(--bg);
    background-image: radial-gradient(circle at 1px 1px, #00000008 1px, transparent 0);
    background-size: 22px 22px;
    font-family: 'Inter', sans-serif;
    color: var(--ink);
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 24px;
  }
  .wrap { width: 100%; max-width: 420px; }
  .eyebrow {
    font-family: 'Space Grotesk', sans-serif;
    font-size: 12px;
    font-weight: 600;
    letter-spacing: 0.14em;
    text-transform: uppercase;
    color: var(--teal);
    margin-bottom: 8px;
  }
  h1 {
    font-family: 'Space Grotesk', sans-serif;
    font-size: 26px;
    font-weight: 700;
    margin: 0 0 4px 0;
    letter-spacing: -0.01em;
  }
  .sub {
    color: var(--muted);
    font-size: 14px;
    margin: 0 0 24px 0;
  }
  .card {
    background: var(--card);
    border: 1px solid var(--line);
    border-radius: 16px;
    padding: 28px;
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06);
    position: relative;
    overflow: hidden;
  }
  label {
    display: block;
    font-size: 12px;
    font-weight: 600;
    color: var(--ink);
    margin-bottom: 6px;
    margin-top: 18px;
  }
  label:first-of-type { margin-top: 0; }
  input {
    width: 100%;
    padding: 11px 12px;
    border: 1.5px solid var(--line);
    border-radius: 9px;
    font-family: 'Inter', sans-serif;
    font-size: 14.5px;
    color: var(--ink);
    background: #FCFBF8;
    transition: border-color 0.15s ease, background 0.15s ease;
  }
  input:focus {
    outline: none;
    border-color: var(--teal);
    background: #fff;
  }
  input::placeholder { color: #A8B0AB; }
  button {
    width: 100%;
    margin-top: 24px;
    padding: 13px;
    background: var(--teal);
    color: #fff;
    border: none;
    border-radius: 9px;
    font-family: 'Space Grotesk', sans-serif;
    font-size: 14.5px;
    font-weight: 600;
    letter-spacing: 0.01em;
    cursor: pointer;
    transition: background 0.15s ease, transform 0.1s ease;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
  }
  button:hover { background: var(--teal-dark); }
  button:active { transform: scale(0.99); }
  button:disabled { opacity: 0.6; cursor: default; }
  .spinner {
    width: 14px; height: 14px;
    border: 2px solid rgba(255,255,255,0.4);
    border-top-color: #fff;
    border-radius: 50%;
    animation: spin 0.7s linear infinite;
    display: none;
  }
  button.loading .spinner { display: inline-block; }
  button.loading .btn-label { display: none; }
  @keyframes spin { to { transform: rotate(360deg); } }

  .error-msg {
    display: none;
    background: #FBEEEC;
    border: 1px solid #EBCFC8;
    color: var(--error);
    font-size: 13px;
    padding: 10px 12px;
    border-radius: 8px;
    margin-top: 16px;
  }

  /* Success overlay */
  .success-panel {
    position: absolute;
    inset: 0;
    background: var(--success-bg);
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    text-align: center;
    padding: 28px;
    opacity: 0;
    pointer-events: none;
    transform: scale(0.98);
    transition: opacity 0.25s ease, transform 0.25s ease;
  }
  .success-panel.show {
    opacity: 1;
    pointer-events: all;
    transform: scale(1);
  }
  .check-circle {
    width: 56px; height: 56px;
    border-radius: 50%;
    background: var(--success);
    display: flex; align-items: center; justify-content: center;
    margin-bottom: 16px;
    animation: pop 0.4s cubic-bezier(0.34, 1.56, 0.64, 1);
  }
  @keyframes pop {
    0% { transform: scale(0); }
    100% { transform: scale(1); }
  }
  .check-circle svg { width: 26px; height: 26px; }
  .check-circle path {
    stroke-dasharray: 24;
    stroke-dashoffset: 24;
    animation: draw 0.35s ease 0.15s forwards;
  }
  @keyframes draw { to { stroke-dashoffset: 0; } }
  .success-title {
    font-family: 'Space Grotesk', sans-serif;
    font-size: 18px;
    font-weight: 700;
    margin: 0 0 4px 0;
  }
  .success-detail {
    color: var(--muted);
    font-size: 13.5px;
    margin: 0 0 20px 0;
    line-height: 1.5;
  }
  .success-detail strong { color: var(--ink); }
  .add-another {
    background: transparent;
    color: var(--teal);
    border: 1.5px solid var(--teal);
    width: auto;
    padding: 9px 18px;
    margin-top: 0;
  }
  .phone-row {
    display: flex;
    gap: 8px;
  }
  .phone-prefix {
    flex: 0 0 auto;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 0 12px;
    border: 1.5px solid var(--line);
    border-radius: 9px;
    background: #F0EEE6;
    color: var(--ink);
    font-weight: 600;
    font-size: 14.5px;
    font-family: 'Space Grotesk', sans-serif;
    letter-spacing: 0.01em;
  }
  .phone-row input { flex: 1 1 auto; }
  .field-hint {
    font-size: 11.5px;
    color: var(--muted);
    margin-top: 5px;
  }
  .add-another:hover { background: var(--teal); color: #fff; }
</style>
</head>
<body>
  <div class="wrap">
    <div class="eyebrow">Dr Smith Dental &middot; Booking Desk</div>
    <h1>Add appointment</h1>
    <p class="sub">The patient gets a WhatsApp reminder automatically the day before.</p>

    <div class="card">
      <form id="apptForm">
        <label>Practice name</label>
        <input type="text" name="practice_name" required placeholder="Dr Smith Dental">

        <label>Patient name</label>
        <input type="text" name="patient_name" required placeholder="Jane Dlamini">

        <label>Patient WhatsApp number</label>
        <div class="phone-row">
          <span class="phone-prefix">+27</span>
          <input type="text" name="patient_number_local" id="phoneLocal" required placeholder="82 123 4567" inputmode="numeric">
        </div>
        <div class="field-hint">With or without the leading 0 — either works.</div>

        <label>Appointment date &amp; time</label>
        <input type="datetime-local" name="appointment_time" id="apptTime" required>

        <button type="submit" id="submitBtn">
          <span class="spinner"></span>
          <span class="btn-label">Add appointment</span>
        </button>

        <div class="error-msg" id="errorMsg"></div>
      </form>

      <div class="success-panel" id="successPanel">
        <div class="check-circle">
          <svg viewBox="0 0 24 24" fill="none">
            <path d="M5 13l4 4L19 7" stroke="#fff" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </div>
        <p class="success-title">Appointment added</p>
        <p class="success-detail" id="successDetail"></p>
        <button class="add-another" id="addAnother">Add another</button>
      </div>
    </div>
  </div>

<script>
const form = document.getElementById('apptForm');
const submitBtn = document.getElementById('submitBtn');
const errorMsg = document.getElementById('errorMsg');
const successPanel = document.getElementById('successPanel');
const successDetail = document.getElementById('successDetail');
const apptTime = document.getElementById('apptTime');

// Keep the datetime picker from ever offering a past moment.
function setMinDateTime() {
  const now = new Date();
  now.setMinutes(now.getMinutes() - now.getTimezoneOffset());
  apptTime.min = now.toISOString().slice(0, 16);
}
setMinDateTime();
apptTime.addEventListener('focus', setMinDateTime);

function normalizeNumber(raw) {
  // Strip everything but digits, then drop a single leading 0 if present.
  let digits = raw.replace(/\D/g, '');
  if (digits.startsWith('0')) digits = digits.slice(1);
  return digits;
}

form.addEventListener('submit', async (e) => {
  e.preventDefault();
  errorMsg.style.display = 'none';

  const localNumber = normalizeNumber(document.getElementById('phoneLocal').value);
  if (localNumber.length !== 9) {
    errorMsg.textContent = 'Enter a valid South African number, e.g. 82 123 4567.';
    errorMsg.style.display = 'block';
    return;
  }

  const chosen = new Date(apptTime.value);
  if (chosen < new Date()) {
    errorMsg.textContent = 'That date and time has already passed. Pick a future slot.';
    errorMsg.style.display = 'block';
    return;
  }

  submitBtn.classList.add('loading');
  submitBtn.disabled = true;

  const data = Object.fromEntries(new FormData(form).entries());
  data.patient_number = '+27' + localNumber;

  try {
    const res = await fetch('/add', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(data)
    });
    const result = await res.json();

    if (!res.ok) throw new Error(result.error || 'Something went wrong');

    const dt = new Date(data.appointment_time);
    const niceDate = dt.toLocaleDateString('en-ZA', { weekday: 'long', day: 'numeric', month: 'long' });
    const niceTime = dt.toLocaleTimeString('en-ZA', { hour: '2-digit', minute: '2-digit' });

    const waLine = result.whatsapp_sent
      ? "Confirmation sent via WhatsApp."
      : "Saved, but the WhatsApp confirmation didn't send. Check the number.";

    successDetail.innerHTML = `<strong>${data.patient_name}</strong> &middot; ${niceDate} at ${niceTime}<br>${waLine}`;
    successPanel.classList.add('show');
  } catch (err) {
    errorMsg.textContent = err.message;
    errorMsg.style.display = 'block';
  } finally {
    submitBtn.classList.remove('loading');
    submitBtn.disabled = false;
  }
});

document.getElementById('addAnother').addEventListener('click', () => {
  form.reset();
  successPanel.classList.remove('show');
});
</script>
</body>
</html>
'''

@app.route('/add', methods=['GET', 'POST'])
def add_appointment():
    if request.method == 'POST':
        data = request.get_json(silent=True) or request.form
        practice_name = data.get('practice_name', '').strip()
        patient_name = data.get('patient_name', '').strip()
        patient_number = data.get('patient_number', '').strip()
        appointment_time = data.get('appointment_time', '').replace('T', ' ')

        if not all([practice_name, patient_name, patient_number, appointment_time]):
            return jsonify({'error': 'All fields are required.'}), 400

        if not re.fullmatch(r'\+27\d{9}', patient_number):
            return jsonify({'error': 'Phone number must be a valid South African number.'}), 400

        try:
            appt_dt = datetime.strptime(appointment_time, '%Y-%m-%d %H:%M')
        except ValueError:
            return jsonify({'error': 'Invalid appointment date or time.'}), 400

        if appt_dt < datetime.now(SAST).replace(tzinfo=None):
            return jsonify({'error': "That date and time has already passed."}), 400

        conn = get_connection()
        c = conn.cursor()
        c.execute('''
            INSERT INTO appointments (practice_name, patient_name, patient_number, appointment_time)
            VALUES (%s, %s, %s, %s)
        ''', (practice_name, patient_name, patient_number, appointment_time))
        conn.commit()
        conn.close()

        # Send an instant WhatsApp confirmation, but don't let a messaging
        # failure block the booking itself from succeeding.
        whatsapp_sent = True
        try:
            date_part, time_part = appointment_time.split(' ')
            confirm_body = (
                f"Hi {patient_name}, your appointment with {practice_name} on "
                f"{date_part} at {time_part} has been booked. We'll send you a "
                f"reminder the day before. Reply 2 anytime to reschedule."
            )
            twilio_client.messages.create(
                from_=TWILIO_WHATSAPP_NUMBER,
                body=confirm_body,
                to=f'whatsapp:{patient_number}'
            )
        except Exception as e:
            whatsapp_sent = False
            print(f"Failed to send instant confirmation: {e}")

        return jsonify({'status': 'ok', 'whatsapp_sent': whatsapp_sent})

    return render_template_string(FORM_HTML)

@app.route('/whatsapp', methods=['POST'])
def whatsapp_reply():
    incoming_msg = request.values.get('Body', '').strip()
    from_number = request.values.get('From', '').replace('whatsapp:', '')

    resp = MessagingResponse()
    msg = resp.message()

    conn = get_connection()
    c = conn.cursor()

    c.execute('''
        SELECT id, practice_name, appointment_time
        FROM appointments
        WHERE patient_number = %s
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
