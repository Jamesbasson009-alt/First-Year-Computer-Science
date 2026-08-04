# Timetable Maker

MVP web app to parse university PDF timetables and generate a clash-free timetable.

What’s included
- Minimal FastAPI scaffold (app/main.py) with an upload endpoint
- Simple frontend (templates/index.html) for uploading PDFs and previewing results
- samples/ folder for uploaded or sample PDFs
- requirements.txt listing initial dependencies

Next steps
- Improve PDF parsing (pdfplumber) and add parser tests
- Implement solver and timetable renderer
- Dockerize and deploy to Render

Notes
- Folder name contains a space; consider renaming to `Timetable-Maker` for automation tooling.
