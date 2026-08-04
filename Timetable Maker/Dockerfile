FROM python:3.11-slim

# set workdir
WORKDIR /app

# install system deps for WeasyPrint (optional) and fonts if needed
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    libpango1.0-0 \
    libgdk-pixbuf2.0-0 \
    libffi-dev \
    libxml2 \
    libxml2-dev \
    libxslt1-dev \
    libssl-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# copy requirements and project
COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . /app

# ensure sample dir exists
RUN mkdir -p /app/samples

ENV PYTHONUNBUFFERED=1

CMD ["uvicorn","app.main:app","--host","0.0.0.0","--port","10000"]
