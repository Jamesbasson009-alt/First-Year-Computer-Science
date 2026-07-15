import os
from dotenv import load_dotenv
from twilio.rest import Client

load_dotenv()

account_sid = os.getenv('TWILIO_ACCOUNT_SID')
auth_token = os.getenv('TWILIO_AUTH_TOKEN')
twilio_number = os.getenv('TWILIO_WHATSAPP_NUMBER')

client = Client(account_sid, auth_token)

message = client.messages.create(
    from_=twilio_number,
    body='Hi! This is a test message from your booking bot.',
    to='whatsapp:+27634450456'  # replace with your real number, no spaces, with country code
)

print(f"Message sent! SID: {message.sid}")
