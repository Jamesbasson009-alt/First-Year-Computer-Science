from datetime import datetime, timedelta
import database

# NOTE: this only seeds the in-memory store of whatever process runs it.
# Run it locally against `python app.py` in the same session (or add a
# call to it at the top of app.py) if you want sample data to show up —
# running it as a separate one-off script won't affect the live deploy.

tomorrow = (datetime.now() + timedelta(days=1)).strftime('%Y-%m-%d %H:%M')

database.add_appointment('Dr. Smith Dental', 'John Doe', '+2634450456', tomorrow)

print(f"Added test appointment for tomorrow: {tomorrow}")
