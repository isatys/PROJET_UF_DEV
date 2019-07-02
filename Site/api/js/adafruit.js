sendQuery('https://io.adafruit.com/api/v2/isatys/feeds?x-aio-key=30bcd5bfe9294a03afde64ff868e7021');

sendQueryWithCallback(
        'https://io.adafruit.com/api/v2/isatys/feeds?x-aio-key=30bcd5bfe9294a03afde64ff868e7021',
        'GET',
        () => {console.log("ok")}
    );
    function refreshData() {
    sendQueryWithCallback(
            'https://io.adafruit.com/api/v2/isatys/feeds?x-aio-key=30bcd5bfe9294a03afde64ff868e7021',
            'GET',
            (response) => {
                response = JSON.parse(response);
                console.log("Refreshed");
                console.log(response)

                document.getElementById("value-1").innerHTML = `humidité air :  ${response[5]['last_value']}%`;
                document.getElementById("value-2").innerHTML = `humidité sol : ${response[4]['last_value']}%`;
                document.getElementById("value-3").innerHTML = `température : ${response[3]['last_value']}°C`;

        });
        setTimeout(refreshData, 10000);

    };
    refreshData();