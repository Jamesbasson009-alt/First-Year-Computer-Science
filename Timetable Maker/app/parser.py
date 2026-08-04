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

TIME_RE = re.compile(r"(\d{1,2}:?\d{0,2})\s*(?:-|to|–)\s*(\d{1,2}:?\d{0,2})")
ROW_LIKE = re.compile(r"(?P<course>[A-Z]{2,}\s*\d{3,})\s+(?P<type>LEC|TUT|PRA|LAB|LECTURE|TUTORIAL|PRACTICAL)\s*(?P<group>\w+)?", re.I)
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
    """Heuristic parsing: split on lines and look for patterns that indicate rows.
    This will need tuning for your specific PDF layout.
    """
    results = []
    lines = [l.strip() for l in text.splitlines() if l.strip()]
    for i, line in enumerate(lines):
        # look for a course code + type
        m = ROW_LIKE.search(line)
        if m:
            course = m.group('course').strip()
            typ = m.group('type').strip()
            group = (m.group('group') or '').strip()
            # try to find day/time/location in the remainder of the line or next lines
            rest = line[m.end():].strip()
            combined = rest
            # include next line if it looks like time/day info
            if i+1 < len(lines):
                combined += ' ' + lines[i+1]
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
            # location heuristic: uppercase words or room numbers after time
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
