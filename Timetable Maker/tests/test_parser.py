import os
from pathlib import Path
import pytest
from app import parser

SAMPLE = Path(__file__).resolve().parents[1] / 'samples' / 'UP_MOD_XLS_9.pdf'

@pytest.mark.skipif(not SAMPLE.exists(), reason='Sample PDF not present in samples/')
def test_parse_sample():
    rows = parser.parse_pdf(str(SAMPLE))
    assert isinstance(rows, list)
    # Expect at least one parsed row for a real timetable PDF
    assert len(rows) > 0


def test_parse_missing():
    with pytest.raises(FileNotFoundError):
        parser.parse_pdf('nonexistent.pdf')
