# ESP 8266 Captive Portal GET Authentication Exploit

Creates a Wi-Fi AP which requires users to log in through a Captive Portal. When a user logs in, all of the GET data sent through the form is sent to the Serial Monitor in Arduino

First: the user joins the Free AP: ![WiFi Settings on an iPhone](https://i.imgur.com/qbP5Rok.jpg)

Second: They're prompted with a login screen:![Popup when connecting](https://i.imgur.com/I8RE6Q8.jpg)

Third: They enter their details: ![Data Entered](https://i.imgur.com/fqLalu2.jpg)

Last: The details are sent through serial monitor![Example of Serial Monitor](https://i.imgur.com/KbLAgwd.png)
