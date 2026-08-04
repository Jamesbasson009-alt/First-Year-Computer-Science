"""
Timetable PDF parser utilities.

This module provides parse_pdf(path) that attempts to use pdfplumber (preferred)
or pypdf as a fallback to extract text and parse common timetable row patterns.

The parser is best-effort and returns a list of dicts with fields:
  {"course":..., "type":..., "group":..., "day":..., "start":..., "end":..., "location":...}

Usage:
  results = parse_pdf('samples/UP_MOD_XLS_9.pdf')

Note: pdfplumber is recommended for more accurate extraction. Run tests with pytest after installing dependencies:
  python -m venv venv
  source venv/bin/activate
  pip install -r ../requirements.txt
  pytest -q
"""

import re
from pathlib import Path
from typing import List, Dict

TIME_RE = re.compile(r"(\d{1,2}:?\d{0,2})\s*(?:-|to|–|\u2013|\u2014)\s*(\d{1,2}:?\d{0,2})")
ROW_LIKE = re.compile(r"(?P<course>[A-Z]{2,}\s*\d{3,})[\s,:-]+(?P<type>LEC|TUT|PRA|LAB|LECTURE|TUTORIAL|PRACTICAL|TUTORIAL)\s*(?P<group>\w+)?", re.I)
DAY_WORDS = ["Mon","Tue","Wed","Thu","Fri","Sat","Sun","Monday","Tuesday","Wednesday","Thursday","Friday"]


def extract_text(path: Path) -> str:
    """Extract text from PDF using available backends. Returns raw text (may be imperfect)."""
    try:
        import pdfplumber
        with pdfplumber.open(path) as pdf:
            pages = [p.extract_text() or '' for p in pdf.pages]
            return "\n\n".join(pages)
    except Exception:
        try:
            from pypdf import PdfReader
            reader = PdfReader(str(path))
            texts = []
            for p in reader.pages:
                t = p.extract_text() or ''
                texts.append(t)
            return "\n\n".join(texts)
        except Exception as e:
            raise RuntimeError(f"No suitable PDF backend available: {e}")


def normalize_time(s: str) -> str:
    s = s.strip().replace('.', ':')
    if ':' not in s:
        if len(s) in (3,4):
            s = s[:-2] + ':' + s[-2:]
    return s


def parse_rows_from_text(text: str) -> List[Dict]:
    """Heuristic parsing: use line tokenization and table-like detection.

    Strategy:
    - Split pages into lines, group lines with similar column counts.
    - For each line, try regex matches for course/type and time/day tokens.
    - Return best-effort rows.
    """
    results = []
    lines = [l.strip() for l in text.splitlines() if l.strip()]

    # quick pass: try to find contiguous table-like blocks (lines with digits/time)
    candidate_lines = []
    for line in lines:
        if re.search(r"\d{1,2}:?\d{0,2}", line) or ROW_LIKE.search(line):
            candidate_lines.append(line)

    for i, line in enumerate(candidate_lines):
        m = ROW_LIKE.search(line)
        if not m:
            # sometimes course and type are on previous line; try to look back
            if i > 0:
                back = candidate_lines[i-1]
                m = ROW_LIKE.search(back)
                rest = line
            else:
                rest = line
        else:
            rest = line[m.end():].strip()

        if not m:
            # fallback: try to extract any uppercase-with-digits token for course
            course_search = re.search(r"[A-Z]{2,}\s*\d{3,}", line)
            course = course_search.group(0).strip() if course_search else 'UNKNOWN'
            typ = 'GEN'
            group = ''
        else:
            course = m.group('course').strip()
            typ = m.group('type').strip()
            group = (m.group('group') or '').strip()

        # aggregate context lines (this line + next)
        combined = rest
        if i+1 < len(candidate_lines):
            combined += ' ' + candidate_lines[i+1]

        # day
        day = None
        for w in DAY_WORDS:
            if re.search(rf"\b{w}\b", combined, re.I):
                day = w
                break

        # time
        time_m = TIME_RE.search(combined)
        start = end = None
        if time_m:
            start = normalize_time(time_m.group(1))
            end = normalize_time(time_m.group(2))

        # location heuristic
        loc = None
        loc_search = re.search(r"\bRm\.?\s*\d+\b|\bRoom\s*\d+\b|[A-Z]{2,}\-?\d{1,3}\b", combined)
        if loc_search:
            loc = loc_search.group(0)

        results.append({
            "course": course,
            "type": typ,
            "group": group,
            "day": day,
            "start": start,
            "end": end,
            "location": loc,
            "raw": line,
        })

    return results


def parse_pdf(path: str) -> List[Dict]:
    p = Path(path)
    if not p.exists():
        raise FileNotFoundError(path)
    text = extract_text(p)
    rows = parse_rows_from_text(text)
    return rows


if __name__ == '__main__':
    import sys, json
    if len(sys.argv) < 2:
        print('Usage: parser.py <pdf-path>')
        sys.exit(2)
    res = parse_pdf(sys.argv[1])
    print(json.dumps(res, indent=2))
