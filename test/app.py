#!/usr/bin/env streamlit run
import streamlit as st
import pandas as pd
import os, requests, json

API_KEY=os.environ.get('AIRNOW')

st.title('Data Visualization')

st.sidebar.header('')
mainPage = st.sidebar.selectbox(
    'Choose from some examples...',
    ['Weather map', 'Growth', 'Adjustment'])


if mainPage == 'Weather map':
    BASE_URL = 'https://www.airnowapi.org/aq/observation/zipCode/historical/?format=application/json'
    startDate = st.date_input('Choose a date...')
    zipCode = int(st.number_input('Enter your zip code...', min_value=0, max_value=99999, value=75019, step=1))
    distance = st.slider('Choose a radius...',0,100,10,5)
    ARGS = f'&zipCode={zipCode}&date={startDate.year}-{startDate.month}-{startDate.day-2}T00-0000&distance={distance}&API_KEY={API_KEY}'
    QUERY_URL = f'{BASE_URL}{ARGS}'
    r = requests.get(QUERY_URL)
    if r.status_code != 200:
        st.write('Couldn\'t get data!')

    ozone = json.loads(r.text)[0]
    date = ozone["DateObserved"]
    aqi = ozone["AQI"]
    description = ozone["Category"]["Name"]
    lat, lon = (ozone["Latitude"], ozone["Longitude"])
    st.write(f'Date: {date}, AQI: {aqi}, Level: {description}')
    mapData = pd.DataFrame({'lat': [lat, lat+10], 'lon': [lon, lon+10]})
    st.map(mapData)
