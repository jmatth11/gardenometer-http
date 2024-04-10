# gardenometer-http
Arduino project for monitoring garden metrics. This is the http bridge for the [ESP8266](https://en.wikipedia.org/wiki/ESP8266#:~:text=The%20ESP8266%20is%20a%20low,Manufacturer)

We scan the LAN for our home website and connect to it when found.
From there we just send updates when the main arduino board sends them to us and we handle sending the responses back to the main arduino board.
