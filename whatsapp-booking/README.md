# WhatsApp Booking

Live demo: https://whatsapp-booking-osmf.onrender.com

A small scheduling app for creating and sending booking confirmations via WhatsApp links. Built with Flask and designed to store appointments and users in a PostgreSQL database. Includes helper scripts for creating users, adding test data, sending test messages, and checking/sending reminders.

## Features
- Create and view appointments
- Send booking confirmations via WhatsApp (Twilio/WhatsApp or URL-based links)
- Simple login system with roles (admin, receptionist)
- Scripts to bootstrap test data and send reminders

## Stack
- Language: Python 3.11+ (see requirements.txt)
- Framework: Flask
- Notable libraries: Flask, psycopg2 (Postgres), python-dotenv, twilio, gunicorn

## Repo layout (important files)