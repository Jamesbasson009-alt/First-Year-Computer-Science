# WhatsApp Booking

Live demo: https://whatsapp-booking-osmf.onrender.com (WhatsApp messages won't work for any users other than me due to using Twilio's free plan)

A small scheduling app for creating and sending booking confirmations via WhatsApp links. Built with Flask. Appointments and users are stored in memory — no external database to set up or pay for, and no free-tier database expiring every 30 days. The trade-off: appointment data resets whenever the app restarts or redeploys. Login credentials are fixed (see below).

## Features
- Create and view appointments
- Send booking confirmations via WhatsApp (Twilio/WhatsApp or URL-based links)
- Simple login system with roles (admin, receptionist)
- Scripts to add test data and send reminders

## Stack
- Language: Python 3.11+ (see requirements.txt)
- Framework: Flask
- Notable libraries: Flask, python-dotenv, twilio, gunicorn

## Data storage
Appointments live in an in-memory list inside `database.py` — no `DATABASE_URL` needed. This resets on every restart/redeploy, which is fine for a demo but not for anything you need to persist. If you outgrow this, swap `database.py` for a real database again.

## Daily reminders
`/run-reminders?key=...` and the admin dashboard's "Run reminders" button both trigger reminder-checking inside the running app, so they see the live in-memory data. Running `check_reminders.py` as a standalone script (e.g. from a GitHub Actions cron) no longer works, since it isn't the same process — point the cron job at the `/run-reminders` URL with `curl` instead.

## Credentials
ADMIN
- Username - admin
- Password - admin

USER
- Username - user
- Password - user

