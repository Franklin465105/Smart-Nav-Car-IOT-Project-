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
    Welcome to my website displaying the various sensors that my car contains.
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
      <td>Battery Level</td>
      <td><span id="batteryValue"></span></td>
      <td>%</td>
    </tr>

    <tr>
      <td>Power Consumption</td>
      <td><span id="powerUsedValue"></span></td>
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

  fetch("/battery")
    .then(r => r.text())
    .then(t => document.getElementById("batteryValue").innerHTML = t);

  fetch("/powerused")
    .then(r => r.text())
    .then(t => document.getElementById("powerUsedValue").innerHTML = t);

}

setInterval(updateSensors, 1000);

</script>

</body>
</html>
)=====");