from app import solver

def make_row(course, typ, group, day, start, end):
    return {'course': course, 'type': typ, 'group': group, 'day': day, 'start': start, 'end': end}


def test_solver_simple_nonconflict():
    rows = [
        make_row('CSC101','LEC','A','Mon', '09:00','10:00'),
        make_row('CSC101','TUT','1','Tue', '10:00','11:00'),
        make_row('MTH100','LEC','A','Mon', '10:00','11:00'),
    ]
    res = solver.generate_timetable_from_parsed(rows)
    assert res['solution'] is not None


def test_solver_conflict():
    rows = [
        make_row('CSC101','LEC','A','Mon', '09:00','11:00'),
        make_row('MTH100','LEC','A','Mon', '10:00','12:00'),
    ]
    res = solver.generate_timetable_from_parsed(rows)
    # no solution exists
    assert res['solution'] is None
