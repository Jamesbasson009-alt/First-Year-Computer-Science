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
- Add a new Web Service, select "Docker" as Environment and use the provided Dockerfile.
- Set the public port to 10000 (or leave default and adjust Dockerfile CMD port).
- Add render.yaml to repository and enable "Create a new service from file" when configuring on Render for automated setup.

Notes
- Parser depends on pdfplumber/pypdf; if unavailable on the host, use the UI and provide parsed JSON to /generate.
- samples/ is gitignored to avoid committing user files; move test PDFs into samples/ locally to run parser.

Contact
- For a live deployment, connect the repo to Render and deploy the timetable-maker service; I can help with steps or automate if you provide Render/GitHub access.
