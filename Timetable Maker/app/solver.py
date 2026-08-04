"""Backtracking timetable solver (MVP).

Provides a best-effort solver that, given parsed rows (course, type, group, day, start, end),
builds options per course and searches for a clash-free assignment.
"""
from typing import List, Dict, Optional
from collections import defaultdict

WEEKDAYS = ["Mon","Tue","Wed","Thu","Fri","Sat","Sun"]


def time_to_minutes(t: Optional[str]) -> Optional[int]:
    if not t:
        return None
    t = t.strip()
    if ':' in t:
        h,m = t.split(':')
    else:
        if len(t) <= 2:
            h = t; m = '0'
        else:
            h = t[:-2]; m = t[-2:]
    try:
        return int(h)*60 + int(m)
    except:
        return None


def overlaps(a_start, a_end, b_start, b_end):
    if None in (a_start,a_end,b_start,b_end):
        return False
    return not (a_end <= b_start or b_end <= a_start)


def build_course_options(rows: List[Dict]) -> Dict[str, List[Dict]]:
    """Group parsed rows into course->list of option dicts."""
    opts = defaultdict(list)
    for r in rows:
        course = r.get('course') or 'UNKNOWN'
        day = r.get('day')
        start = time_to_minutes(r.get('start'))
        end = time_to_minutes(r.get('end'))
        opts[course].append({
            'type': r.get('type'),
            'group': r.get('group'),
            'day': day,
            'start': start,
            'end': end,
            'raw': r,
        })
    return dict(opts)


def solve_backtracking(options_by_course: Dict[str, List[Dict]]) -> Optional[Dict[str, Dict]]:
    courses = list(options_by_course.keys())
    n = len(courses)
    assignment = {}

    # Pre-sort options for deterministic behavior
    for k in options_by_course:
        options_by_course[k] = sorted(options_by_course[k], key=lambda o: (o.get('day') or '', o.get('start') or 0))

    def conflict_with_assignment(opt):
        for chosen in assignment.values():
            # if same day and times overlap
            if opt.get('day') and chosen.get('day') and opt.get('day')[:3] == chosen.get('day')[:3]:
                if overlaps(opt.get('start'), opt.get('end'), chosen.get('start'), chosen.get('end')):
                    return True
        return False

    def backtrack(idx=0):
        if idx >= n:
            return True
        course = courses[idx]
        for opt in options_by_course.get(course, []):
            if conflict_with_assignment(opt):
                continue
            assignment[course] = opt
            if backtrack(idx+1):
                return True
            assignment.pop(course, None)
        return False

    ok = backtrack(0)
    return assignment if ok else None


def generate_timetable_from_parsed(rows: List[Dict]) -> Dict:
    opts = build_course_options(rows)
    solution = solve_backtracking(opts)
    return {
        'options_count': {k: len(v) for k,v in opts.items()},
        'solution': solution,
    }

if __name__ == '__main__':
    import json, sys
    if len(sys.argv) < 2:
        print('Usage: solver.py <parsed-json-file>')
        sys.exit(2)
    p = sys.argv[1]
    data = json.load(open(p))
    print(json.dumps(generate_timetable_from_parsed(data), indent=2))
