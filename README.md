Project Progress

First Semester

The first semester marked the initial development phase of the project. During this time, I focused on setting up and testing the core sensors that form the foundation of the system. These included the ultrasonic sensor for obstacle detection, the INA-219 power sensor for monitoring voltage and current, and the Adafruit MMA8451 motion sensor for detecting movement and orientation.

The main priority during this stage was ensuring that each sensor functioned correctly and reliably. I tested the necessary code for data collection and communication, and also worked on my website. This website was designed to show sensor data and update in real time which it did.

Overall, the first semester was dedicated to establishing a solid software and sensor foundation, with the intention of focusing more heavily on hardware in the following semester.

Second Semester

In the second semester, I mainly focused on hardware and building the physical car system. While initial progress was promising, I encountered several challenges related to the car chassis, battery pack, and motor driver, which slowed development.

Due to these issues, I made the decision to switch to a DFRobot L298N motor driver which provided a more reliable and compatible solution. After troubleshooting wiring problems and replacing faulty components, I was able to successfully get the system working.

With the hardware in place, I connected all sensors alongside Bluetooth control using the Dabble application. The ultrasonic sensor actively monitors the distance in behind the car and triggers a buzzer when an obstacle is detected within 10cm. The INA-219 power sensor measures the real-time power consumption of the system in watts, as well as estimating the remaining battery level based on the current voltage reading. The MMA8451 accelerometer detects whether the car is stationary or in motion. All of this data is displayed live on a website hosted by the ESP32 which updates every second. The car itself is controlled wirelessly via the Dabble gamepad over Bluetooth.
