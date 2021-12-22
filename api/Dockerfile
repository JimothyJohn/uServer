FROM python:3.8

RUN apt-get update

WORKDIR /code
ADD . /code

RUN pip install --upgrade pip
RUN pip install -r requirements.txt

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
