import os
import re
from datetime import datetime
from functools import wraps
import pytz
from flask import Flask, request, render_template_string, jsonify, session, redirect, url_for
from twilio.twiml.messaging_response import MessagingResponse
from twilio.rest import Client
from dotenv import load_dotenv
from werkzeug.security import check_password_hash
import database

load_dotenv()

app = Flask(__name__)
app.secret_key = os.getenv('FLASK_SECRET_KEY', 'dev-only-fallback-change-me')

twilio_client = Client(os.getenv('TWILIO_ACCOUNT_SID'), os.getenv('TWILIO_AUTH_TOKEN'))
TWILIO_WHATSAPP_NUMBER = os.getenv('TWILIO_WHATSAPP_NUMBER')
SAST = pytz.timezone('Africa/Johannesburg')

def login_required(f):
    @wraps(f)
    def wrapped(*args, **kwargs):
        if 'user_id' not in session:
            return redirect(url_for('login', next=request.path))
        return f(*args, **kwargs)
    return wrapped

def admin_required(f):
    @wraps(f)
    def wrapped(*args, **kwargs):
        if 'user_id' not in session:
            return redirect(url_for('login', next=request.path))
        if session.get('role') != 'admin':
            return "Forbidden — admin access only.", 403
        return f(*args, **kwargs)
    return wrapped

LOGIN_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bookly — Log in</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
<style>
  :root {
    --ink: #16302B; --teal: #0F6B5C; --teal-dark: #0B5045;
    --bg: #FAF8F3; --card: #FFFFFF; --line: #E4E0D6; --muted: #6B7570; --error: #C0503E;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0; min-height: 100vh; background: var(--bg);
    background-image: radial-gradient(circle at 1px 1px, #00000008 1px, transparent 0);
    background-size: 22px 22px;
    font-family: 'Inter', sans-serif; color: var(--ink);
    display: flex; align-items: center; justify-content: center; padding: 24px;
  }
  .wrap { width: 100%; max-width: 380px; }
  .eyebrow {
    font-family: 'Space Grotesk', sans-serif; font-size: 12px; font-weight: 600;
    letter-spacing: 0.14em; text-transform: uppercase; color: var(--teal); margin-bottom: 8px; text-align: center;
  }
  h1 {
    font-family: 'Space Grotesk', sans-serif; font-size: 24px; font-weight: 700;
    margin: 0 0 24px 0; letter-spacing: -0.01em; text-align: center;
  }
  .card {
    background: var(--card); border: 1px solid var(--line); border-radius: 16px; padding: 28px;
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06);
  }
  label { display: block; font-size: 12px; font-weight: 600; margin-bottom: 6px; margin-top: 16px; }
  label:first-of-type { margin-top: 0; }
  input {
    width: 100%; padding: 11px 12px; border: 1.5px solid var(--line); border-radius: 9px;
    font-family: 'Inter', sans-serif; font-size: 14.5px; background: #FCFBF8;
  }
  input:focus { outline: none; border-color: var(--teal); background: #fff; }
  button {
    width: 100%; margin-top: 22px; padding: 13px; background: var(--teal); color: #fff; border: none;
    border-radius: 9px; font-family: 'Space Grotesk', sans-serif; font-size: 14.5px; font-weight: 600; cursor: pointer;
  }
  button:hover { background: var(--teal-dark); }
  .error-msg {
    background: #FBEEEC; border: 1px solid #EBCFC8; color: var(--error);
    font-size: 13px; padding: 10px 12px; border-radius: 8px; margin-top: 16px;
  }
</style>
</head>
<body>
  <div class="wrap">
    <div class="eyebrow">Bookly</div>
    <h1>Log in</h1>
    <div class="card">
      <form method="POST">
        <label>Username</label>
        <input type="text" name="username" required autofocus>
        <label>Password</label>
        <input type="password" name="password" required>
        <button type="submit">Log in</button>
      </form>
      {% if error %}
        <div class="error-msg">{{ error }}</div>
      {% endif %}
    </div>
  </div>
</body>
</html>
'''

@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        username = request.form.get('username', '').strip()
        password = request.form.get('password', '')

        user = database.get_user(username)

        if user and check_password_hash(user['password_hash'], password):
            session['user_id'] = user['id']
            session['username'] = username
            session['role'] = user['role']
            next_url = request.args.get('next')
            if user['role'] == 'admin':
                return redirect(next_url or url_for('admin_dashboard'))
            return redirect(next_url or url_for('home'))
        else:
            error = 'Incorrect username or password.'

    return render_template_string(LOGIN_HTML, error=error)

@app.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('login'))

FORM_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bookly — Add Appointment</title>
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
    <div class="eyebrow"><a href="/" style="color:inherit;text-decoration:none;">&larr;</a> Bookly</div>
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

HOME_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bookly</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
<style>
  :root {
    --ink: #16302B; --teal: #0F6B5C; --teal-dark: #0B5045;
    --bg: #FAF8F3; --card: #FFFFFF; --line: #E4E0D6; --muted: #6B7570;
    --success: #25D366;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0; min-height: 100vh; background: var(--bg);
    background-image: radial-gradient(circle at 1px 1px, #00000008 1px, transparent 0);
    background-size: 22px 22px;
    font-family: 'Inter', sans-serif; color: var(--ink);
    display: flex; align-items: center; justify-content: center; padding: 24px;
  }
  .wrap { width: 100%; max-width: 440px; text-align: center; }
  .eyebrow {
    font-family: 'Space Grotesk', sans-serif; font-size: 12px; font-weight: 600;
    letter-spacing: 0.14em; text-transform: uppercase; color: var(--teal); margin-bottom: 8px;
  }
  h1 {
    font-family: 'Space Grotesk', sans-serif; font-size: 28px; font-weight: 700;
    margin: 0 0 6px 0; letter-spacing: -0.01em;
  }
  .sub { color: var(--muted); font-size: 14px; margin: 0 0 32px 0; }
  .nav-grid { display: flex; flex-direction: column; gap: 14px; }
  .nav-card {
    display: flex; align-items: center; gap: 16px; text-align: left;
    background: var(--card); border: 1px solid var(--line); border-radius: 14px;
    padding: 20px; text-decoration: none; color: var(--ink);
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06);
    transition: transform 0.12s ease, border-color 0.12s ease;
  }
  .nav-card:hover { transform: translateY(-2px); border-color: var(--teal); }
  .nav-icon {
    flex: 0 0 auto; width: 44px; height: 44px; border-radius: 11px;
    display: flex; align-items: center; justify-content: center;
    font-size: 20px;
  }
  .nav-icon.add { background: #EAFBF1; }
  .nav-icon.list { background: #EAF3FB; }
  .nav-title {
    font-family: 'Space Grotesk', sans-serif; font-weight: 600; font-size: 15px; margin-bottom: 2px;
  }
  .nav-desc { color: var(--muted); font-size: 12.5px; }
  .footer-note { margin-top: 28px; color: var(--muted); font-size: 11.5px; }
  .logout-link { display: block; margin-top: 18px; color: var(--muted); text-decoration: none; font-size: 12.5px; font-weight: 600; }
  .logout-link:hover { color: var(--error, #C0503E); }
  .nav-icon.admin { background: #FDF1E6; }
</style>
</head>
<body>
  <div class="wrap">
    <div class="eyebrow">Bookly &middot; {{ username }}</div>
    <h1>What would you like to do?</h1>
    <p class="sub">WhatsApp reminders, handled automatically.</p>

    <div class="nav-grid">
      <a class="nav-card" href="/add">
        <div class="nav-icon add">+</div>
        <div>
          <div class="nav-title">Add appointment</div>
          <div class="nav-desc">Book a new patient in, WhatsApp confirmation sent instantly.</div>
        </div>
      </a>
      <a class="nav-card" href="/appointments">
        <div class="nav-icon list">&#9776;</div>
        <div>
          <div class="nav-title">View appointments</div>
          <div class="nav-desc">See what's coming up and cancel if needed.</div>
        </div>
      </a>
      {% if role == 'admin' %}
      <a class="nav-card" href="/admin">
        <div class="nav-icon admin">&#9881;</div>
        <div>
          <div class="nav-title">Admin tools</div>
          <div class="nav-desc">Run reminders manually, view stats.</div>
        </div>
      </a>
      {% endif %}
    </div>

    <p class="footer-note">Reminders go out automatically the day before each appointment.</p>
    <a class="logout-link" href="/logout">Log out</a>
  </div>
</body>
</html>
'''

ADMIN_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bookly — Admin</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
<style>
  :root {
    --ink: #16302B; --teal: #0F6B5C; --teal-dark: #0B5045;
    --bg: #FAF8F3; --card: #FFFFFF; --line: #E4E0D6; --muted: #6B7570;
    --success: #25D366; --success-bg: #EAFBF1; --error: #C0503E;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0; min-height: 100vh; background: var(--bg);
    font-family: 'Inter', sans-serif; color: var(--ink); padding: 32px 20px;
  }
  .wrap { max-width: 780px; margin: 0 auto; }
  .top-row { display: flex; justify-content: space-between; align-items: flex-end; margin-bottom: 22px; flex-wrap: wrap; gap: 12px; }
  .eyebrow {
    font-family: 'Space Grotesk', sans-serif; font-size: 12px; font-weight: 600;
    letter-spacing: 0.14em; text-transform: uppercase; color: var(--teal); margin-bottom: 6px;
  }
  h1 { font-family: 'Space Grotesk', sans-serif; font-size: 24px; font-weight: 700; margin: 0; letter-spacing: -0.01em; }
  .logout-link {
    color: var(--muted); text-decoration: none; font-size: 13px; font-weight: 600;
  }
  .logout-link:hover { color: var(--error); }
  .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(160px, 1fr)); gap: 14px; margin-bottom: 24px; }
  .stat-card {
    background: var(--card); border: 1px solid var(--line); border-radius: 14px; padding: 18px;
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06);
  }
  .stat-num { font-family: 'Space Grotesk', sans-serif; font-size: 26px; font-weight: 700; }
  .stat-label { font-size: 12px; color: var(--muted); margin-top: 2px; }
  .card {
    background: var(--card); border: 1px solid var(--line); border-radius: 16px; padding: 22px; margin-bottom: 16px;
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06);
  }
  .card h2 { font-family: 'Space Grotesk', sans-serif; font-size: 15px; margin: 0 0 6px 0; }
  .card p { color: var(--muted); font-size: 13px; margin: 0 0 14px 0; }
  .btn {
    display: inline-flex; align-items: center; gap: 8px;
    background: var(--teal); color: #fff; border: none; text-decoration: none;
    padding: 10px 16px; border-radius: 8px; font-family: 'Space Grotesk', sans-serif;
    font-weight: 600; font-size: 13.5px; cursor: pointer;
  }
  .btn:hover { background: var(--teal-dark); }
  .btn.secondary { background: transparent; color: var(--teal); border: 1.5px solid var(--teal); }
  .btn.secondary:hover { background: var(--teal); color: #fff; }
  .btn-row { display: flex; gap: 10px; flex-wrap: wrap; }
  .result-box {
    margin-top: 14px; padding: 10px 12px; border-radius: 8px; font-size: 13px; display: none;
  }
  .result-box.ok { background: var(--success-bg); color: #0C8C50; display: block; }
  .result-box.err { background: #FBEEEC; color: var(--error); display: block; }
  .spinner {
    width: 13px; height: 13px; border: 2px solid rgba(255,255,255,0.4); border-top-color: #fff;
    border-radius: 50%; animation: spin 0.7s linear infinite; display: none;
  }
  .btn.loading .spinner { display: inline-block; }
  @keyframes spin { to { transform: rotate(360deg); } }
</style>
</head>
<body>
  <div class="wrap">
    <div class="top-row">
      <div>
        <div class="eyebrow">Bookly Admin &middot; {{ username }}</div>
        <h1>Testing tools</h1>
      </div>
      <a class="logout-link" href="/logout">Log out</a>
    </div>

    <div class="grid">
      <div class="stat-card">
        <div class="stat-num">{{ total }}</div>
        <div class="stat-label">Total appointments</div>
      </div>
      <div class="stat-card">
        <div class="stat-num">{{ pending }}</div>
        <div class="stat-label">Reminders pending</div>
      </div>
      <div class="stat-card">
        <div class="stat-num">{{ sent }}</div>
        <div class="stat-label">Reminders sent</div>
      </div>
    </div>

    <div class="card">
      <h2>Run reminders now</h2>
      <p>Manually triggers the same check GitHub Actions runs daily — sends any due reminders and clears old appointments.</p>
      <div class="btn-row">
        <button class="btn" id="runRemindersBtn" onclick="runReminders()">
          <span class="spinner"></span>
          <span>Run reminders</span>
        </button>
      </div>
      <div class="result-box" id="reminderResult"></div>
    </div>

    <div class="card">
      <h2>Go to</h2>
      <div class="btn-row">
        <a class="btn secondary" href="/add">Add appointment</a>
        <a class="btn secondary" href="/appointments">View appointments</a>
      </div>
    </div>
  </div>

<script>
async function runReminders() {
  const btn = document.getElementById('runRemindersBtn');
  const result = document.getElementById('reminderResult');
  btn.classList.add('loading');
  btn.disabled = true;
  result.style.display = 'none';

  try {
    const res = await fetch('/admin/run-reminders', { method: 'POST' });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || 'Something went wrong');
    result.textContent = data.message;
    result.className = 'result-box ok';
  } catch (err) {
    result.textContent = err.message;
    result.className = 'result-box err';
  } finally {
    btn.classList.remove('loading');
    btn.disabled = false;
  }
}
</script>
</body>
</html>
'''

@app.route('/admin')
@admin_required
def admin_dashboard():
    total, pending, sent = database.count_appointments()
    return render_template_string(ADMIN_HTML, username=session.get('username'), total=total, pending=pending, sent=sent)

@app.route('/admin/run-reminders', methods=['POST'])
@admin_required
def admin_run_reminders():
    from check_reminders import check_reminders
    try:
        check_reminders()
        return jsonify({'message': 'Reminders checked and past appointments cleared.'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/')
@login_required
def home():
    return render_template_string(HOME_HTML, username=session.get('username'), role=session.get('role'))

LIST_HTML = r'''
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bookly — Appointments</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&display=swap" rel="stylesheet">
<style>
  :root {
    --ink: #16302B; --teal: #0F6B5C; --teal-dark: #0B5045;
    --bg: #FAF8F3; --card: #FFFFFF; --line: #E4E0D6; --muted: #6B7570;
    --success: #25D366; --success-bg: #EAFBF1; --error: #C0503E; --error-bg: #FBEEEC;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0; min-height: 100vh; background: var(--bg);
    font-family: 'Inter', sans-serif; color: var(--ink); padding: 32px 20px;
  }
  .wrap { max-width: 780px; margin: 0 auto; }
  .top-row { display: flex; justify-content: space-between; align-items: flex-end; margin-bottom: 22px; flex-wrap: wrap; gap: 12px; }
  .eyebrow {
    font-family: 'Space Grotesk', sans-serif; font-size: 12px; font-weight: 600;
    letter-spacing: 0.14em; text-transform: uppercase; color: var(--teal); margin-bottom: 6px;
  }
  h1 { font-family: 'Space Grotesk', sans-serif; font-size: 24px; font-weight: 700; margin: 0; letter-spacing: -0.01em; }
  .add-link {
    background: var(--teal); color: #fff; text-decoration: none;
    font-family: 'Space Grotesk', sans-serif; font-weight: 600; font-size: 13.5px;
    padding: 10px 16px; border-radius: 8px; white-space: nowrap;
  }
  .add-link:hover { background: var(--teal-dark); }
  .card { background: var(--card); border: 1px solid var(--line); border-radius: 16px; overflow: hidden;
    box-shadow: 0 1px 2px rgba(22,48,43,0.04), 0 8px 24px rgba(22,48,43,0.06); }
  table { width: 100%; border-collapse: collapse; }
  th {
    text-align: left; font-size: 11px; font-weight: 600; letter-spacing: 0.06em; text-transform: uppercase;
    color: var(--muted); padding: 12px 16px; border-bottom: 1px solid var(--line); background: #FBFAF6;
  }
  td { padding: 13px 16px; border-bottom: 1px solid var(--line); font-size: 13.5px; vertical-align: middle; }
  tr:last-child td { border-bottom: none; }
  tr.past td { color: var(--muted); }
  .patient { font-weight: 600; }
  .badge {
    display: inline-block; font-size: 11px; font-weight: 600; padding: 3px 9px; border-radius: 999px;
    font-family: 'Space Grotesk', sans-serif;
  }
  .badge.sent { background: var(--success-bg); color: #0C8C50; }
  .badge.pending { background: #FFF4E0; color: #9A6B12; }
  .del-btn {
    background: transparent; border: 1.5px solid var(--error); color: var(--error);
    border-radius: 7px; padding: 5px 11px; font-size: 12px; font-weight: 600; cursor: pointer;
    font-family: 'Inter', sans-serif;
  }
  .del-btn:hover { background: var(--error); color: #fff; }
  .empty { padding: 48px 16px; text-align: center; color: var(--muted); font-size: 14px; }
</style>
</head>
<body>
  <div class="wrap">
    <div class="top-row">
      <div>
        <div class="eyebrow"><a href="/" style="color:inherit;text-decoration:none;">&larr;</a> Bookly</div>
        <h1>Upcoming appointments</h1>
      </div>
      <a class="add-link" href="/add">+ Add appointment</a>
    </div>

    <div class="card">
      {% if appointments %}
      <table>
        <thead>
          <tr>
            <th>Patient</th>
            <th>Practice</th>
            <th>When</th>
            <th>Reminder</th>
            <th></th>
          </tr>
        </thead>
        <tbody>
          {% for a in appointments %}
          <tr class="{{ 'past' if a.is_past else '' }}" id="row-{{ a.id }}">
            <td class="patient">{{ a.patient_name }}</td>
            <td>{{ a.practice_name }}</td>
            <td>{{ a.display_time }}</td>
            <td>
              {% if a.reminder_sent %}
                <span class="badge sent">Sent</span>
              {% else %}
                <span class="badge pending">Pending</span>
              {% endif %}
            </td>
            <td><button class="del-btn" onclick="deleteAppt({{ a.id }})">Cancel</button></td>
          </tr>
          {% endfor %}
        </tbody>
      </table>
      {% else %}
        <div class="empty">No appointments yet. <a href="/add">Add the first one</a>.</div>
      {% endif %}
    </div>
  </div>

<script>
async function deleteAppt(id) {
  if (!confirm('Cancel this appointment?')) return;
  const res = await fetch('/appointments/' + id, { method: 'DELETE' });
  if (res.ok) {
    document.getElementById('row-' + id).remove();
  } else {
    alert('Could not cancel that appointment.');
  }
}
</script>
</body>
</html>
'''

@app.route('/appointments')
@login_required
def list_appointments():
    rows = database.get_all_appointments()

    now = datetime.now(SAST).replace(tzinfo=None)
    appointments = []
    for row in rows:
        appt_time = row['appointment_time']
        try:
            dt = datetime.strptime(appt_time, '%Y-%m-%d %H:%M')
            display_time = dt.strftime('%a %d %b, %H:%M')
            is_past = dt < now
        except ValueError:
            display_time = appt_time
            is_past = False
        appointments.append({
            'id': row['id'],
            'practice_name': row['practice_name'],
            'patient_name': row['patient_name'],
            'display_time': display_time,
            'reminder_sent': bool(row['reminder_sent']),
            'is_past': is_past,
        })

    return render_template_string(LIST_HTML, appointments=appointments)

@app.route('/appointments/<int:appt_id>', methods=['DELETE'])
@login_required
def delete_appointment(appt_id):
    database.delete_appointment(appt_id)
    return jsonify({'status': 'deleted'})

@app.route('/add', methods=['GET', 'POST'])
@login_required
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

        database.add_appointment(practice_name, patient_name, patient_number, appointment_time)

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

    appt = database.get_latest_appointment_for_number(from_number)

    if not appt:
        msg.body("We couldn't find an appointment linked to this number. Please contact the practice directly.")
        return str(resp)

    practice, appt_time = appt['practice_name'], appt['appointment_time']

    if incoming_msg == '1':
        msg.body(f"Great, your appointment with {practice} on {appt_time} is confirmed. See you then!")
    elif incoming_msg == '2':
        msg.body(f"No problem — please call {practice} directly to reschedule your appointment.")
    else:
        msg.body("Sorry, I didn't understand that. Reply 1 to confirm or 2 to reschedule your appointment.")

    return str(resp)

@app.route('/run-reminders')
def run_reminders():
    provided_key = request.args.get('key', '')
    expected_key = os.getenv('REMINDER_SECRET', '')
    if not expected_key or provided_key != expected_key:
        return "Forbidden", 403

    from check_reminders import check_reminders
    check_reminders()
    return "Reminders checked!"

if __name__ == '__main__':
    app.run(debug=True, port=5000)
