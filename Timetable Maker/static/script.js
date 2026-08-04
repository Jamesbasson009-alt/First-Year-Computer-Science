async function uploadPdf(file) {
  const fd = new FormData();
  fd.append('file', file);
  const res = await fetch('/upload', { method: 'POST', body: fd });
  return res.json();
}

async function generateTimetable(filename) {
  const res = await fetch('/generate', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ filename }),
  });
  return res.json();
}

function renderParsed(parsed) {
  const out = document.getElementById('parsed');
  out.textContent = JSON.stringify(parsed, null, 2);
}

function renderSolution(solution) {
  const container = document.getElementById('timetable');
  container.innerHTML = '';
  if (!solution) {
    container.textContent = 'No solution found.';
    return;
  }
  // simple day columns
  const days = ['Mon','Tue','Wed','Thu','Fri','Sat','Sun'];
  const grid = document.createElement('div');
  grid.className = 'grid';
  days.forEach(d => {
    const col = document.createElement('div');
    col.className = 'col';
    const title = document.createElement('div');
    title.className = 'col-title';
    title.textContent = d;
    col.appendChild(title);
    grid.appendChild(col);
  });
  container.appendChild(grid);

  // place events
  Object.entries(solution).forEach(([course,opt]) => {
    if (!opt || !opt.day) return;
    const dayIdx = days.findIndex(d => opt.day.startsWith(d));
    if (dayIdx < 0) return;
    const col = grid.children[dayIdx];
    const ev = document.createElement('div');
    ev.className = 'event';
    const start = parseTime(opt.start);
    const end = parseTime(opt.end);
    const top = ((start - 8*60) / (10*60)) * 100; // within 8:00-18:00
    const height = ((end - start) / (10*60)) * 100;
    ev.style.top = top + '%';
    ev.style.height = Math.max(4, height) + '%';
    ev.textContent = course + ' ' + (opt.group||'') + ' ' + (opt.type||'');
    col.appendChild(ev);
  });
}

function parseTime(t) {
  if (!t) return 0;
  if (typeof t === 'number') return t;
  const parts = t.split(':');
  if (parts.length === 1) return parseInt(parts[0]) * 60;
  return parseInt(parts[0])*60 + parseInt(parts[1]);
}

window.addEventListener('DOMContentLoaded', () => {
  const form = document.getElementById('upload-form');
  const fileInput = document.getElementById('file');
  const parsedPre = document.getElementById('parsed');
  const resultPre = document.getElementById('result');
  form.addEventListener('submit', async (e) => {
    e.preventDefault();
    const f = fileInput.files[0];
    if (!f) return alert('Choose a PDF first.');
    resultPre.textContent = 'Uploading...';
    const info = await uploadPdf(f);
    resultPre.textContent = JSON.stringify(info, null, 2);
    // attempt generate
    resultPre.textContent = 'Generating timetable...';
    const out = await generateTimetable(info.filename);
    if (out.error) {
      resultPre.textContent = 'Error: ' + JSON.stringify(out);
      return;
    }
    renderParsed(out.parsed || []);
    renderSolution(out.solution);
    resultPre.textContent = 'Done.';
  });
});
