"""
In-memory data store — no external database required.

Login credentials match the README: admin/admin and user/user.
Appointment data lives only in this process's memory, so it resets
whenever the app restarts or redeploys. That's the trade-off for
never having to renew an expiring free database again.
"""
from werkzeug.security import generate_password_hash

_users = {
    'admin': {'id': 1, 'password_hash': generate_password_hash('admin'), 'role': 'admin'},
    'user': {'id': 2, 'password_hash': generate_password_hash('user'), 'role': 'receptionist'},
}

_appointments = []
_next_id = 1


def get_user(username):
    return _users.get(username)


def add_appointment(practice_name, patient_name, patient_number, appointment_time):
    global _next_id
    appt = {
        'id': _next_id,
        'practice_name': practice_name,
        'patient_name': patient_name,
        'patient_number': patient_number,
        'appointment_time': appointment_time,
        'reminder_sent': 0,
    }
    _appointments.append(appt)
    _next_id += 1
    return appt['id']


def get_all_appointments():
    return sorted(_appointments, key=lambda a: a['appointment_time'])


def delete_appointment(appt_id):
    global _appointments
    _appointments = [a for a in _appointments if a['id'] != appt_id]


def get_latest_appointment_for_number(patient_number):
    matches = [a for a in _appointments if a['patient_number'] == patient_number]
    return max(matches, key=lambda a: a['id']) if matches else None


def get_due_reminders(date_prefix):
    return [a for a in _appointments
            if a['appointment_time'].startswith(date_prefix) and not a['reminder_sent']]


def mark_reminder_sent(appt_id):
    for a in _appointments:
        if a['id'] == appt_id:
            a['reminder_sent'] = 1


def delete_appointments_before(cutoff):
    global _appointments
    before = len(_appointments)
    _appointments = [a for a in _appointments if a['appointment_time'] >= cutoff]
    return before - len(_appointments)


def count_appointments():
    total = len(_appointments)
    pending = sum(1 for a in _appointments if not a['reminder_sent'])
    return total, pending, total - pending
