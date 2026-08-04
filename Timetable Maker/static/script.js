const STATUS = document.getElementById('status');
const DOWNLOAD = document.getElementById('download-json');

async function uploadPdf(file, onProgress) {
  const fd = new FormData();
  fd.append('file', file);
  const res = await fetch('/upload', { method: 'POST', body: fd });
  return res.json();
}

async function generateTimetable(filename, semester) {
  const body = { filename };
  if (semester) body.semester = semester;
  const res = await fetch('/generate', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });
  return res.json();
}

function renderParsed(parsed) {
  const out = document.getElementById('parsed');
  out.textContent = Array.isArray(parsed) ? JSON.stringify(parsed, null, 2) : String(parsed);
}

function renderSolution(solution) {
  const container = document.getElementById('timetable');
  container.innerHTML = '';
  if (!solution) {
    container.textContent = 'No solution found.';
    return;
  }
  const days = ['Mon','Tue','Wed','Thu','Fri','Sat','Sun'];
  const grid = document.createElement('div');
  grid.className = 'grid-days';
  days.forEach(d => {
    const col = document.createElement('div');
    col.className = 'day-col';
    const title = document.createElement('div');
    title.className = 'day-title';
    title.textContent = d;
    col.appendChild(title);
    grid.appendChild(col);
  });
  container.appendChild(grid);

  Object.entries(solution).forEach(([course,opt]) => {
    if (!opt || !opt.day) return;
    const dayIdx = days.findIndex(d => opt.day.startsWith(d));
    if (dayIdx < 0) return;
    const col = grid.children[dayIdx];
    const ev = document.createElement('div');
    ev.className = 'event';
    ev.textContent = course + ' ' + (opt.group||'') + ' ' + (opt.type||'');
    // simple stacking: append in order
    ev.style.position = 'relative';
    ev.style.margin = '6px 0';
    col.appendChild(ev);
  });
}

function enableDownload(data){
  DOWNLOAD.disabled = false;
  DOWNLOAD.onclick = () => {
    const blob = new Blob([JSON.stringify(data, null, 2)], {type:'application/json'});
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a'); a.href = url; a.download = 'timetable_parsed.json'; a.click();
    URL.revokeObjectURL(url);
  }
}

window.addEventListener('DOMContentLoaded', () => {
  const form = document.getElementById('upload-form');
  const fileInput = document.getElementById('file');
  const parsedPre = document.getElementById('parsed');

  form.addEventListener('submit', async (e) => {
    e.preventDefault();
    const f = fileInput.files[0];
    const semesterVal = document.getElementById('semester').value;
    if (!f) return alert('Choose a PDF first.');
    STATUS.textContent = 'Uploading...';
    try{
      const info = await uploadPdf(f);
      STATUS.textContent = 'Uploaded — generating...';
      const out = await generateTimetable(info.filename, semesterVal);
      if (out.error) {
        STATUS.textContent = 'Error: ' + (out.message || out.error);
        renderParsed(out.details || out);
        return;
      }
      STATUS.textContent = 'Done — timetable generated.';
      renderParsed(out.parsed || out);
      renderSolution(out.solution);
      enableDownload(out);
    }catch(err){
      STATUS.textContent = 'Failed: ' + String(err);
    }
  });

  const demoBtn = document.getElementById('demo-btn');
  demoBtn.addEventListener('click', async (e) => {
    STATUS.textContent = 'Loading demo...';
    try{
      const res = await fetch('/demo');
      const out = await res.json();
      if (out.error) { STATUS.textContent = 'Demo error'; return; }
      STATUS.textContent = 'Demo loaded.';
      renderParsed(out.parsed || out);
      renderSolution(out.solution);
      enableDownload(out);
    }catch(err){ STATUS.textContent = 'Demo failed: '+String(err); }
  });
});
