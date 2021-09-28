<?php
include_once 'functions.php';
include_once 'resource/database.php';
?>

<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="Bryan Cage">
    <title>Understand Paho MQTT Library</title>

    <!-- Link for connecting to bootstrap -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet"
          integrity="sha384-giJF6kkoqNQ00vy+HMDP7azOuL0xtbfIcaT9wjKHr8RbDVddVHyTfAAsrekwKmP1" crossorigin="anonymous">

    <!-- Link to local copy of jquery library -->
    <script src="js/jquery.js" type="application/javascript"></script>

    <!-- Link to paho-mqtt library -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.js" type="text/javascript"></script>

</head>
<body>
<section class="main-section">
    <div class="container">
        <div class="col-12">
            <div class="container" style="margin-top: 150px;">
                <div class="row">
                    <div class="col-2"></div>
                    <div class="col-8 d-flex justify-content-center">
                        <h5>Toggle Switch to Send MQTT Packet to ESP32</h5>
                    </div>
                    <div class="col-2"></div>
                </div>
                <div class="row">
                    <div class="col-lg-2"></div>
                    <div class="col-lg-8 d-flex justify-content-center mt-4">
                        <label for="toggle_switch"
                               hidden>Toggle Switch</label>
                        <input
                            class="common_selector Toggle_Switch"
                            id="toggle_switch"
                            type="checkbox"

                            data-toggle="toggle"
                            data-size="sm"
                            data-onstyle="primary"
                            data-offstyle="secondary"
                            data-on="On" data-off="Off"/>
                    </div>
                    <div class="col-lg-2"></div>
                </div>
            </div>
        </div>
        <div class="col-12">
            <div class="container" style="margin-top: 25px;">
                <div class="row">
                    <div class="col-2"></div>
                    <div class="col-8 d-flex justify-content-center">
                        <h5>Temperature will display here when this page receives MQTT Message</h5>
                    </div>
                    <div class="col-2"></div>
                </div>
                <div class="row">
                    <div class="col-2"></div>
                    <div class="col-8 d-flex justify-content-center mt-4">
                        <h1 id="tempData">0.0°C</h1>
                    </div>
                    <div class="col-2"></div>
                </div>
            </div>
        </div>
    </div>
</section>
<script> // All JavaScript for Packet Transmission and Reception occur within these script tags


    // ========= 1. VARIABLE DECLARATIONS AND FUNCTION CALLS =============

    // Instantiating a new client for connecting to MQTT Broker; Initializing it to null
    let client = null;

    // Immediately call startConnect() to connect to MQTT Broker (Server)
    startConnect();


    // ========= 2. FUNCTIONS ============================================

    // Anonymous Function to handle toggle switch button
    $(function () {
        $('.Toggle_Switch').change(function () {
            if ($(this).prop('checked')) {
                console.log("Publishing to mqttSubscribe/: Message = true");
                let message = new Paho.MQTT.Message("{\"firstName\":\"Bryan\", \"lastName\":\"Cage\", \"message\":true}");
                console.log(message.payloadString);
                message.destinationName = "mqttSubscribe/";
                try {
                    client.send(message);
                } catch (error) {
                    console.log("Not Connected");
                }
            } else {
                console.log("Publishing to mqttSubscribe/: Message = false");
                let message = new Paho.MQTT.Message("{\"firstName\":\"Bryan\", \"lastName\":\"Cage\", \"message\":false}");
                console.log(message.payloadString);
                message.destinationName = "mqttSubscribe/";
                try {
                    client.send(message);
                } catch (error) {
                    console.log("Not Connected");
                }
            }
        });
    })

    // Function for making the client connection to the MQTT Broker; Called as page loads
    function startConnect() {
        // Generate a random client ID
        clientID = "clientID-" + parseInt(Math.random() * 100);

        const host = "test.mosquitto.org";
        const port = 8081; /* if port 8081 doesn't connect change to 8080; if that doesn't work change back
                              back to 8081 and try again */

        // Initialize new Paho client connection
        client = new Paho.MQTT.Client(host, Number(port), clientID);

        // Set callback handlers
        client.onConnectionLost = onConnectionLost;
        client.onMessageArrived = onMessageArrived;

        // Connect the client, if successful, call onConnect function
        client.connect({
            onSuccess: onConnect,
            mqttVersion: 4,
            timeout: 120
        });
    }

    // This function is automatically called when the client connects to the MQTT Broker
    function onConnect() {
        console.log("Connected to MQTT Test Broker!");

        // Subscribe to the requested topic
        client.subscribe("iotnetworking/temp/tx");
        let connect_message = new Paho.MQTT.Message("{\"Connected\":true}");
        connect_message.destinationName = "iotnetworking/connStat/";
        client.send(connect_message);
    }

    // This function is automatically called when the client loses its connection
    function onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log(responseObject.errorCode)
        }
        console.log("Disconnected from MQTT Broker");
        startConnect();
    }

    // This function is triggered when a NEW MQTT Packet (message) arrives to this page
    function onMessageArrived(message) {

        let topic = message.destinationName;
        let payload = message.payloadString;
        console.log("Topic: ", message.destinationName);
        console.log("Message: ", message.payloadString);
        try {
            let jsonObject = JSON.parse(payload);
            console.log(jsonObject);
            try {
                if (jsonObject.hasOwnProperty('temp')) {
                    // console.log("Object['temp']: ", jsonObject['temp']);
                    let tempReading = document.getElementById("tempData");
                    tempReading.innerHTML = jsonObject['temp'];
                }
            } catch (err) {
                console.log("Problem Handle MQTT Object");
            }
        } catch (err) {
            console.log("JSON Parsing Error: ", err);
        }
    }

    // This function is automatically called when the client disconnects from MQTT Broker
    function startDisconnect() {
        try {
            client.disconnect()
            console.log("Disconnected");
        } catch (err) {
            console.log("Already disconnected");
        }
    }

</script>

<!-- This is a link to a "style sheet" responsible for making the checkbox look like a toggle switch -->
<link href="https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/css/bootstrap4-toggle.min.css"
      rel="stylesheet">

<!-- This is a link to "JavaScript File" responsible for making the checkbox behave like a toggle switch -->
<script src="https://cdn.jsdelivr.net/gh/gitbrent/bootstrap4-toggle@3.6.1/js/bootstrap4-toggle.min.js"></script>

</body>
</html>
