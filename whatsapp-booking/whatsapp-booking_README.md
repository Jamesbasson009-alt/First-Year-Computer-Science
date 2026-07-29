# WhatsApp Booking

Live demo: https://whatsapp-booking-osmf.onrender.com

A small web app to create and manage bookings via WhatsApp links. This repository contains the source and instructions to run the WhatsApp Booking app locally or view the deployed instance.

## Features
- Create bookings and send confirmation via WhatsApp link
- Simple UI for adding / viewing bookings
- Lightweight backend for storing booking data
- Deployed on Render (link above)

## Tech stack
- Frontend: HTML / CSS / JavaScript (or list framework if applicable)
- Backend: Python / Node / (update with actual backend)
- Database: (e.g., SQLite / PostgreSQL — update accordingly)
- Deployment: Render

## Screenshot
![Screenshot of app](./screenshot.png) <!-- add screenshot file in repo -->

## Quick start (local)
1. Clone the repo
   git clone https://github.com/Jamesbasson009-alt/First-Year-Computer-Science.git
2. Change into the project folder
   cd First-Year-Computer-Science/whatsapp-booking
3. Install dependencies
   - For Node:
     npm install
   - For Python:
     pip install -r requirements.txt
4. Create environment file
   cp .env.example .env
   Edit `.env` and fill in the variables described below.
5. Run the app
   - Node: npm start
   - Python: python app.py (or the start command your project uses)
6. Open http://localhost:3000 (or the port shown in logs)

## Environment variables
Create a `.env` file with the following (example):