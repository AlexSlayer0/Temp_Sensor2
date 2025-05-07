#include <Arduino.h>
#pragma once

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 IoT Dashboard</title>
    <link rel="icon" href="data:,">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f9;
            color: #333;
        }
        header {
            background-color: #007BFF;
            color: white;
            padding: 20px;
            text-align: center;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        header h1 {
            margin: 0;
            font-size: 2.5rem;
        }
        header p {
            margin: 10px 0;
            font-size: 1.2rem;
        }
        nav {
            background-color: #333;
            overflow: hidden;
            text-align: center;
        }
        nav a {
            display: inline-block;
            color: white;
            padding: 14px 20px;
            text-decoration: none;
            text-align: center;
            font-size: 1rem;
        }
        nav a:hover {
            background-color: #555;
            color: white;
        }
        .content {
            padding: 20px;
            margin-top: 20px;
        }
        .section {
            background-color: white;
            margin: 15px 0;
            padding: 25px;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        .section h2 {
            color: #007BFF;
            font-size: 2rem;
            margin-bottom: 15px;
        }
        .button {
            background-color: #28a745;
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }
        .button:hover {
            background-color: #218838;
        }
        .footer {
            background-color: #333;
            color: white;
            padding: 10px;
            text-align: center;
            position: fixed;
            bottom: 0;
            width: 100%;
        }
        .stat-box {
            display: inline-block;
            width: 30%;
            padding: 20px;
            background-color: #f8f9fa;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            margin-right: 5%;
            text-align: center;
            transition: transform 0.3s ease;
        }
        .stat-box:last-child {
            margin-right: 0;
        }
        .stat-box:hover {
            transform: scale(1.05);
        }
        .stat-box h3 {
            font-size: 1.5rem;
            color: #007BFF;
        }
        .stat-box p {
            font-size: 1.2rem;
            margin-top: 10px;
        }
        @media (max-width: 768px) {
            .stat-box {
                width: 48%;
                margin-right: 2%;
            }
        }
        @media (max-width: 480px) {
            .stat-box {
                width: 100%;
                margin-right: 0;
            }
        }
        #temperature-graph {
            margin-top: 30px;
            text-align: center;
        }

        #humidity-graph {
            margin-top: 30px;
            text-align: center;
        }
        
        #pressure-graph {
            margin-top: 30px;
            text-align: center;
        }

        #temperatureChart {
            width: 100%;
            max-width: 600px;
            height: 300px;
            margin: 0 auto;
        }
    </style>
</head>
<body>

    <header>
        <h1>ESP32 IoT Dashboard</h1>
        <p>WLAN-gesteuerte Umweltsensoren und mehr!</p>
    </header>

    <nav>
        <a href="/" onclick="showPage('home')">Home</a>
        <a href="/temperature" onclick="showPage('temperature')">Temperatur</a>
        <a href="/humidity" onclick="showPage('humidity')">Luftfeuchtigkeit</a>
        <a href="/pressure" onclick="showPage('pressure')">Druck</a>
    </nav>

    <div class="content">
        <!-- Home Page -->
        <div class="section" id="home">
            <h2>Willkommen auf deinem Dashboard!</h2>
            <p>Nutze die Navigation, um die aktuellen Messwerte zu sehen.</p>

             <div class="stat-box">
                <h3>Aktuelle Temperatur</h3>
                <p id="stat-temperature">Lädt...</p> <!-- Hier wird die Temperatur angezeigt -->
            </div>
            <div class="stat-box">
                <h3>Aktuelle Luftfeuchtigkeit</h3>
                <p id="stat-humidity">Lädt...</p> <!-- Hier wird die Luftfeuchtigkeit angezeigt -->
            </div>
            <div class="stat-box">
                <h3>Aktueller Luftdruck</h3>
                <p id="stat-pressure">Lädt...</p> <!-- Hier wird der Luftdruck angezeigt -->
            </div>
            
        </div>

        <!-- Temperature Page -->
        <div class="section" id="temperature" style="display:none;">
            <h2>Temperatur Messwerte</h2>
            <p id="temperature-value"></p>
            <button class="button" onclick="location.reload()">Aktualisieren</button>

            <div id="temperature-graph" style="height: 300px; max-width: 100%;">
                <h2>Temperatur im Zeitverlauf</h2>
                <canvas id="temperatureChart"></canvas>
                <button class="button" onclick="location.reload()">Aktualisieren</button>
            </div>
        </div>

        <!-- Humidity Page -->
        <div class="section" id="humidity" style="display:none;">
            <h2>Luftfeuchtigkeit Messwerte</h2>
            <p id="humidity-value"></p>
            <button class="button" onclick="location.reload()">Aktualisieren</button>

            <div id="humidity-graph" style="height: 300px; max-width: 100%;">
                <h2>Luftfeuchtigkeit im Zeitverlauf</h2>
                <canvas id="humidityChart"></canvas>
                <button class="button" onclick="location.reload()">Aktualisieren</button>
            </div>
        </div>

        <!-- Pressure Page -->
        <div class="section" id="pressure" style="display:none;">
            <h2>Druck Messwerte</h2>
            <p id="pressure-value"></p>
            <button class="button" onclick="location.reload()">Aktualisieren</button>

            <div id="pressure-graph" style="height: 300px; max-width: 100%;">
                <h2>Druck im Zeitverlauf</h2>
                <canvas id="pressureChart"></canvas>
                <button class="button" onclick="location.reload()">Aktualisieren</button>
            </div>

        </div>
    </div>

    <div class="footer">
        <p>ESP32 IoT Dashboard | 2025</p>
    </div>

    <script>
        let temperatureData = []; // Array für Temperaturdaten
        let humidityData = []; // Array für Luftfeuchtigkeitsdaten
        let pressureData = []; // Array für Druckdaten
        let timeLabels = []; // Array für Zeitstempel (z.B. Minuten)
        
        // Funktion, um die Sensordaten vom ESP32 zu holen und den Graphen zu aktualisieren
        function fetchData() {
            fetch("/sensor-data") // Deine Endpunkt-URL vom Server
                .then(response => response.json())  // Antwort als JSON
                .then(data => {
                    // Die Daten auf der Seite anzeigen
                    document.getElementById('temperature-value').innerText = data.temperature + " °C";
                    document.getElementById('humidity-value').innerText = data.humidity + " %";
                    document.getElementById('pressure-value').innerText = data.pressure + " hPa";

                    // Daten auf der Startseite auch anzeigen
                    document.getElementById('stat-temperature').innerText = data.temperature + " °C";
                    document.getElementById('stat-humidity').innerText = data.humidity + " %";
                    document.getElementById('stat-pressure').innerText = data.pressure + " hPa";
                    
                    // Temperatur und Zeit zum Graphen hinzufügen
                    let currentTime = new Date().toLocaleTimeString(); // Aktueller Zeitstempel
                    timeLabels.push(currentTime);
                    temperatureData.push(data.temperature);
                    humidityData.push(data.humidity);
                    pressureData.push(data.pressure);

                    if (temperatureData.length > 10) { // Maximal 10 Datenpunkte im Graphen
                        temperatureData.shift(); // Entferne den ältesten Datenpunkt
                        timeLabels.shift(); // Entferne den ältesten Zeitstempel
                    }
                    else if(humidityData.length > 10) { // Maximal 10 Datenpunkte im Graphen
                        humidityData.shift(); // Entferne den ältesten Datenpunkt
                        timeLabels.shift(); // Entferne den ältesten Zeitstempel
                    }
                    else if(pressureData.length > 10) { // Maximal 10 Datenpunkte im Graphen
                        pressureData.shift(); // Entferne den ältesten Datenpunkt
                        timeLabels.shift(); // Entferne den ältesten Zeitstempel
                    }

                    // Den Graphen aktualisieren
                    updateCharts();
                })
                .catch(error => {
                    console.error("Fehler beim Abrufen der Daten:", error);
                });
        }

        // Funktion zum Aktualisieren des Graphen
        let tempChart;
        let humidityChart;
        let pressureChart;

        function updateCharts() {
            const tempCtx = document.getElementById('temperatureChart').getContext('2d');
            const humCtx = document.getElementById('humidityChart')?.getContext('2d');
            const presCtx = document.getElementById('pressureChart')?.getContext('2d');

            if (tempChart) tempChart.destroy();
            if (humidityChart) humidityChart.destroy();
            if (pressureChart) pressureChart.destroy();

            tempChart = new Chart(tempCtx, {
                type: 'line',
                data: {
                    labels: timeLabels,
                    datasets: [{
                        label: 'Temperatur (°C)',
                        data: temperatureData,
                        borderColor: '#007BFF',
                        fill: false,
                        tension: 0.1
                    }]
                },
                options: chartOptions('Temperatur (°C)')
            });

            if (humCtx) {
                humidityChart = new Chart(humCtx, {
                    type: 'line',
                    data: {
                        labels: timeLabels,
                        datasets: [{
                            label: 'Luftfeuchtigkeit (%)',
                            data: humidityData,
                            borderColor: '#28a745',
                            fill: false,
                            tension: 0.1
                        }]
                    },
                    options: chartOptions('Luftfeuchtigkeit (%)')
                });
            }

            if (presCtx) {
                pressureChart = new Chart(presCtx, {
                    type: 'line',
                    data: {
                        labels: timeLabels,
                        datasets: [{
                            label: 'Druck (hPa)',
                            data: pressureData,
                            borderColor: '#ffc107',
                            fill: false,
                            tension: 0.1
                        }]
                    },
                    options: chartOptions('Druck (hPa)')
                });
            }
        }

        function chartOptions(yLabel) {
            return {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: 'Zeit'
                        }
                    },
                    y: {
                        title: {
                            display: true,
                            text: yLabel
                        }
                    }
                }
            };
        }

        // Zeige die angeforderte Seite an
        function showPage(pageId) {
            // Alle Seiten ausblenden
            document.getElementById('home').style.display = 'none';
            document.getElementById('temperature').style.display = 'none';
            document.getElementById('humidity').style.display = 'none';
            document.getElementById('pressure').style.display = 'none';
            document.getElementById('stats').style.display = 'none';

            // Die angeforderte Seite anzeigen
            document.getElementById(pageId).style.display = 'block';
        }

        // Event listener für die Navigation
        document.querySelectorAll('nav a').forEach(function (link) {
            link.addEventListener('click', function (event) {
                event.preventDefault();
                let page = link.getAttribute('href').replace('/', '');
                if (!page) page = 'home';  // Standardmäßig zur Startseite
                showPage(page);
            });
        });

        // Daten beim Laden der Seite holen
        window.onload = function () {
            fetchData();  // Ruft die Sensordaten ab und aktualisiert die Seite
            showPage('home'); // Standardseite anzeigen
            setInterval(fetchData, 5000);  // alle 5 Sekunden automatisch aktualisieren
        };
    </script>

</body>
</html>
)rawliteral";
