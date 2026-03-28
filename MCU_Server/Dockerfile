# Use an official Python runtime as a parent image
FROM python:3.10-slim

# Set environment variables so Python output is sent straight to terminal
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

# Set the working directory in the container
WORKDIR /app

# Install system dependencies (needed for OpenCV and AI models)
RUN apt-get update && apt-get install -y \
    libglib2.0-0 \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
COPY requirements.txt /app/
RUN pip install --upgrade pip && pip install -r requirements.txt

# Copy the rest of the project code into the container
COPY . /app/

# The command to start Django
CMD ["python", "manage.py", "runserver", "0.0.0.0:8000"]