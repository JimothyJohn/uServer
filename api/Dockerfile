FROM python:3.8

RUN apt-get update

WORKDIR /code

COPY ./requirements.txt .
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

ADD . /code

CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
