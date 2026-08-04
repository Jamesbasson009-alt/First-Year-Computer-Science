from fastapi import FastAPI, UploadFile, File
from fastapi.responses import HTMLResponse, JSONResponse
from pathlib import Path

BASE = Path(__file__).resolve().parents[1]
SAMPLES = BASE / "samples"
SAMPLES.mkdir(parents=True, exist_ok=True)

app = FastAPI(title="Timetable Maker")

@app.get("/", response_class=HTMLResponse)
async def index():
    return (BASE / "templates" / "index.html").read_text(encoding="utf-8")

@app.post("/upload")
async def upload(file: UploadFile = File(...)):
    dest = SAMPLES / file.filename
    content = await file.read()
    dest.write_bytes(content)
    return JSONResponse({"filename": file.filename, "path": str(dest)})

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
\n\n# Generate endpoint appended by tooling
from fastapi import Request
from app.solver import generate_timetable_from_parsed


@app.post('/generate')
async def generate(request: Request):
    payload = await request.json() if request.headers.get('content-type','').startswith('application/json') else {}
    filename = payload.get('filename')
    # if filename provided, try to parse using parser, otherwise expect parsed rows in payload
    parsed_rows = payload.get('parsed')
    if not parsed_rows:
        if filename:
            p = SAMPLES / filename
            try:
                from app.parser import parse_pdf
                parsed_rows = parse_pdf(str(p))
            except Exception as e:
                return JSONResponse({'error': 'parser-failed', 'details': str(e)})
        else:
            return JSONResponse({'error': 'no-input', 'message': 'Provide "filename" or "parsed" in JSON payload'})
    result = generate_timetable_from_parsed(parsed_rows)
    return JSONResponse(result)
