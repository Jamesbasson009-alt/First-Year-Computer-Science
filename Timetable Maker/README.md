# Timetable Maker

MVP web app to parse university PDF timetables and generate a clash-free timetable.

What’s included
- Minimal FastAPI scaffold (app/main.py) with an upload endpoint and /generate API
- Simple frontend (templates/index.html) for uploading PDFs and previewing results
- samples/ folder for uploaded or sample PDFs (ignored by git)
- requirements.txt listing initial dependencies
- Dockerfile and docker-compose.yml for local testing
- render.yaml for easy Render deployment

Quick local run (recommended)
1. With Docker (preferred):
   - docker build -t timetable-maker .
   - docker run -p 10000:10000 timetable-maker
   - Open http://localhost:10000/

2. Without Docker (local venv):
   - python3 -m venv venv && source venv/bin/activate
   - pip install -r requirements.txt
   - uvicorn app.main:app --app-dir . --reload --port 8000
   - Open http://localhost:8000/

Deploy to Render (recommended)
- Connect your GitHub repo to Render.
- Add a new Web Service and choose the Python runtime (avoids Docker system dependency issues):
  - Repository: Jamesbasson009-alt/First-Year-Computer-Science, Branch: main
  - Root Directory: Timetable Maker
  - Build Command: pip install -r requirements.txt
  - Start Command: uvicorn app.main:app --host 0.0.0.0 --port $PORT
  - Health check path: /
- Create the service and watch the build logs. This avoids apt errors from Docker-based builds.

Notes
- We removed WeasyPrint from requirements to avoid system library requirements; if you need PDF export, either use Docker (install system libs) or add WeasyPrint and required libraries on the host.
- Parser depends on pdfplumber/pypdf; if unavailable on the host, use the UI and provide parsed JSON to /generate.
- samples/ is gitignored to avoid committing user files; move test PDFs into samples/ locally to run parser.

Contact
- After creating the Render service, tell me the service URL and I will help verify and iterate on parser/solver behavior.
