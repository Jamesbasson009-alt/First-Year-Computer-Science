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
