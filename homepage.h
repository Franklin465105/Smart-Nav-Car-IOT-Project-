String homePagePart1 = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">

<title>Smart Navigation Car Website</title>

<style>

body {
  background: linear-gradient(135deg, #b3d9ff, #e6f2ff);
  margin: 0;
  font-family: Arial, Helvetica, sans-serif;
  color: #1b2a41;
}

.flex-Container {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 25px;
}

h1 {
  font-size: 38px;
  color: #1f3b73;
  margin-bottom: 10px;
}

p {
  font-size: 20px;
  color: #2b4c7e;
  text-align: center;
}

/* Card-style table */
table {
  border-collapse: collapse;
  width: 90%;
  max-width: 700px;
  margin-top: 20px;
  background: white;
  border-radius: 12px;
  overflow: hidden;
  box-shadow: 0 6px 14px rgba(0,0,0,0.12);
}

th {
  background: #4a90e2;
  color: white;
  font-size: 20px;
  padding: 12px;
  text-align: left;
}

td {
  font-size: 18px;
  padding: 12px;
  border-bottom: 1px solid #e6e6e6;
}

tr:hover {
  background-color: #f2f8ff;
}

span {
  font-weight: bold;
  color: #1f3b73;
}

</style>
</head>

<body>

<div class="flex-Container">

  <h1>Smart Nav Car Website</h1>

  <p>
    Welcome to my website displaying the various sensors that my contains.
  </p>

  <table>
    <tr>
      <th>Sensor</th>
      <th>Result</th>
      <th>Unit</th>
    </tr>

    <tr>
      <td>Ultrasonic</td>
      <td><span id="ultraValue">)=====");

String homePagePart2 = F(R"=====(
      </span></td>
      <td>CM</td>
    </tr>

    <tr>
      <td>Accelerometer</td>
      <td><span id="motionValue">)=====");

String homePagePart3 = F(R"=====(
      </span></td>
      <td></td>
    </tr>

    <tr>
      <td>Voltage</td>
      <td><span id="voltageValue"></span></td>
      <td>V</td>
    </tr>

    <tr>
      <td>Current</td>
      <td><span id="currentValue"></span></td>
      <td>mA</td>
    </tr>

    <tr>
      <td>Power</td>
      <td><span id="powerValue"></span></td>
      <td>W</td>
    </tr>

  </table>

</div>

<script>

function updateSensors() {

  fetch("/ultra")
    .then(r => r.text())
    .then(t => document.getElementById("ultraValue").innerHTML = t);

  fetch("/motion")
    .then(r => r.text())
    .then(t => document.getElementById("motionValue").innerHTML = t);

  fetch("/voltage")
    .then(r => r.text())
    .then(t => document.getElementById("voltageValue").innerHTML = t);

  fetch("/current")
    .then(r => r.text())
    .then(t => document.getElementById("currentValue").innerHTML = t);

  fetch("/power")
    .then(r => r.text())
    .then(t => document.getElementById("powerValue").innerHTML = t);
}

setInterval(updateSensors, 1000);

</script>

</body>
</html>
)=====");